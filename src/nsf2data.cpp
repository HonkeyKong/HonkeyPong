#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
//#include <conio.h>	// This isn't really necessary, and unavailable in a POSIX environment.
#include <string.h>


#define OUT_NESASM	0
#define OUT_CA65	1
#define OUT_ASM6	2
#define OUT_OPHIS	3

char DB[8];
char DW[8];
char LL[8];

unsigned char memory[65536];
bool log;
bool change;
int wait;
int duration;

int regs[32];

FILE *out_file;
int out_size;

unsigned char *nsf_data;
int nsf_size;

int nsf_songs;
int nsf_load_adr;
int nsf_init_adr;
int nsf_play_adr;

unsigned char effect_data[256];
int effect_ptr;



void effect_add(unsigned char n)
{
	if(effect_ptr<256) effect_data[effect_ptr]=n;

	++effect_ptr;
}



static inline unsigned char mem_rd(int adr)
{
	if(adr<0x2000) return memory[adr&0x7ff];//RAM

	return memory[adr];//ROM
}



static inline void mem_wr(int adr,unsigned char data)
{
	const unsigned char regmap[32]={0x80,0,0x81,0x82,0x83,0,0x84,0x85,0x86,0,0x87,0x88,0x89,0,0x8a};

	if(adr<0x2000)//RAM
	{
		memory[adr&0x7ff]=data;
		return;
	}

	if(adr>=0x5c00&&adr<0x8000)//extra RAM
	{
		memory[adr]=data;
		return;
	}

	if(adr<0x4018)//APU registers
	{
		if(!log) return;

		if(adr==0x4000||adr==0x4002||adr==0x4003||adr==0x4004||adr==0x4006||adr==0x4007||adr==0x4008||adr==0x400a||adr==0x400b||adr==0x400c||adr==0x400e)//check if the register is allowed
		{
			if(regs[adr-0x4000]!=data)
			{
				if(!change)
				{
					while(wait>=127)
					{
						effect_add(127+1);
						wait-=127;
					}

					if(wait>=0) effect_add(wait+1);

					wait=0;
				}

				effect_add(regmap[adr-0x4000]);
				effect_add(data);

				regs[adr-0x4000]=data;

				change=true;
			}
		}
	}
}



#include "cpu2a03.h"



void convert_effects(int mode)//mode 0 for NTSC, 1 for PAL
{
	int i,j,cnt,col;

	for(j=0;j<nsf_songs;++j)
	{
		fprintf(out_file,"%ssfx_%s_%i:\n",LL,!mode?"ntsc":"pal",j);

		memset(memory,0,65536);
		memcpy(memory+nsf_load_adr,nsf_data+0x80,nsf_size-0x80);

		for(i=0;i<32;i++) regs[i]=-1;

		regs[0x00]=0x30;
		regs[0x04]=0x30;
		regs[0x08]=0x00;
		regs[0x0c]=0x30;

		cpu_reset();
		CPU.A=j;
		CPU.X=mode;
		CPU.PC.hl=nsf_init_adr;
		log=false;

		for(i=0;i<2000;++i) cpu_tick(); //2000 is enough for FT init

		cpu_reset();

		effect_ptr=0;

		log=true;
		cnt=0;
		wait=-1;
		duration=0;

		while(true)
		{
			CPU.PC.hl=nsf_play_adr;
			CPU.jam=false;
			CPU.S=0x80;
			change=false;

			for(i=0;i<30000/4;++i)
			{
				cpu_tick();

				if(CPU.jam) break;
				if(CPU.S>0x80) break;//stack pop without push
			}

			if(!change) ++wait;

			++duration;

			if(wait>5*60) break;

			if(duration>10*60)
			{
				printf("Effect %i is too long, Cxx at end of the effect may be missing\n",j);
				break;
			}
		}

		effect_add(0);//end

		if(effect_ptr<256)
		{
			printf("Effect %i %s %i\n",j,!mode?"NTSC":"PAL ",effect_ptr);
		}
		else
		{
			printf("Effect %i %s is %i bytes, this is too long!\n",j,!mode?"NTSC":"PAL",effect_ptr);
		}

		col=0;

		for(i=0;i<effect_ptr;++i)
		{
			if(!col) fprintf(out_file,"\t%s ",DB);

			fprintf(out_file,"$%2.2x",effect_data[i]);

			++col;

			if(col==16||i==effect_ptr-1)
			{
				fprintf(out_file,"\n");
				col=0;
			}
			else
			{
				fprintf(out_file,",");
			}
		}

		out_size+=effect_ptr;
	}
}



int main(int argc,char *argv[])
{
	char name[1024];
	int i,outtype;
	bool pal,ntsc;
	FILE *file;

	if(argc<2)
	{
		printf("nsf2data converter for FamiTone2 audio library\n");
		printf("by Shiru (shiru@mail.ru) 01'14\n");
		printf("Ophis and Linux/OSX support by HonkeyKong (honkeykong@honkeykong.org), 01'15\n\n");
		printf("Usage: nsf2data filename.nsf [-ca65 or -asm6 or -ophis][-pal or -ntsc]\n");
		return 1;
	}

	outtype=OUT_NESASM;
	pal=true;
	ntsc=true;

	/* _stricmp() isn't available in a POSIX environment,
	   so it has been replaced with strcmp. The only thing
	   to note is that this means command-line switches
	   are now case-sensitive, which isn't a big deal.
	*/
	for(i=1;i<argc;++i)
	{
		if(!strcmp(argv[i],"-ca65")) outtype=OUT_CA65;
		if(!strcmp(argv[i],"-asm6")) outtype=OUT_ASM6;
		if(!strcmp(argv[i],"-ophis")) outtype=OUT_OPHIS;
		if(!strcmp(argv[i],"-pal"))  { pal=true;  ntsc=false; }
		if(!strcmp(argv[i],"-ntsc")) { pal=false; ntsc=true; }
	}

	printf("Output format: ");

	switch(outtype)
	{
	case OUT_NESASM:
		printf("NESASM\n");
		strcpy(DB,".db");
		strcpy(DW,".dw");
		strcpy(LL,".");
		break;

	case OUT_CA65:
		printf("CA65\n");
		strcpy(DB,".byte");
		strcpy(DW,".word");
		strcpy(LL,"@");
		break;

	case OUT_ASM6:
		printf("Asm6\n");
		strcpy(DB,".db");
		strcpy(DW,".dw");
		strcpy(LL,"@");
		break;

	case OUT_OPHIS:
		printf("Ophis\n");
		strcpy(DB,".byte");
		strcpy(DW,".word");
		strcpy(LL,"");
	}

	printf("Output mode: ");

	if(pal&&ntsc) printf("PAL and NTSC\n"); else if(pal) printf("PAL only\n"); else printf("NTSC only\n");

	printf("\n");

	file=fopen(argv[1],"rb");

	if(!file)
	{
		printf("Can't open file\n");
		return 1;
	}

	fseek(file,0,SEEK_END);
	nsf_size=ftell(file);
	fseek(file,0,SEEK_SET);
	nsf_data=(unsigned char*)malloc(nsf_size);
	fread(nsf_data,nsf_size,1,file);
	fclose(file);

	nsf_songs=nsf_data[0x06];

	nsf_load_adr=nsf_data[0x08]+(nsf_data[0x09]<<8);
	nsf_init_adr=nsf_data[0x0a]+(nsf_data[0x0b]<<8);
	nsf_play_adr=nsf_data[0x0c]+(nsf_data[0x0d]<<8);

	if(nsf_songs>64)
	{
		printf("Too many sound effects in the file, only 64 supported\n");
		free(nsf_data);
		return 1;
	}

	for(i=0x70;i<0x78;++i)
	{
		if(nsf_data[i])
		{
			printf("Bankswitching is not supported\n");
			free(nsf_data);
			return 1;
		}
	}

	if(nsf_data[0x7b])
	{
		printf("Expansion chips are not supported\n");
		free(nsf_data);
		return 1;
	}

	printf("%i effects found\n\n",nsf_songs);

	strcpy(name,argv[1]);

	for(i=strlen(name)-1;i>=0;--i)
	{
		if(name[i]=='.')
		{
			name[i]=0;
			break;
		}
	}

	//if(outtype!=OUT_CA65) strcat(name,".asm"); else strcat(name,".s");
	switch(outtype) {
		case OUT_CA65:
			strcat(name, ".s");
			break;
		case OUT_OPHIS:
			strcat(name,".oph");
			break;
		default:
			strcat(name,".asm");
			break;
	}

	out_file=fopen(name,"wt");

	out_size=nsf_songs*4;

	fprintf(out_file,";this file for FamiTone2 libary generated by nsf2data tool\n\n");

	fprintf(out_file,"sounds:\n");

	for(i=0;i<nsf_songs;++i)
	{
		if(pal&&ntsc)
		{
			fprintf(out_file,"\t%s %ssfx_ntsc_%i,%ssfx_pal_%i\n",DW,LL,i,LL,i);
			continue;
		}

		if(ntsc) fprintf(out_file,"\t%s %ssfx_ntsc_%i,%ssfx_ntsc_%i\n",DW,LL,i,LL,i);
		if(pal)  fprintf(out_file,"\t%s %ssfx_pal_%i,%ssfx_pal_%i\n",DW,LL,i,LL,i);
	}

	fprintf(out_file,"\n");

	if(ntsc) convert_effects(0);
	if(pal)  convert_effects(1);

	fclose(out_file);
	free(nsf_data);

	printf("\nTotal data size %i bytes\n",out_size);

	return 0;
}