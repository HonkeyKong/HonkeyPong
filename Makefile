all: rom

rom:
	@echo Assembling iNES ROM...
	@[[ -d bin ]] || mkdir bin
	@ophis HonkeyPong_iNES.oph

debugrom:
	@echo Assembling iNES ROM with debug info...
	@[[ -d bin ]] || mkdir bin
	@ophis HonkeyPong_iNES_debug.oph -m HonkeyPong_Debug.map

cartridge: rom
	@echo Preparing cartridge ROMs...
	@python tools/nesprep.py bin/HonkeyPong.nes -splitPRG -splitCHR -quiet

cartridge-256: rom
	@echo Preparing NROM-256 cartridge ROMs...
	@python tools/nesprep.py bin/HonkeyPong.nes -inflatePRG=0.25 -splitCHR -quiet

test: rom
	@echo Loading emulator...
	@Nestopia bin/HonkeyPong.nes

debug: debugrom
	@echo Loading debugger...
	@Nintendulator bin/HonkeyPong.nes

clean:
	@echo Cleaning up...
	@rm -f HonkeyPong_Debug.map bin/HonkeyPong.nes bin/HonkeyPong.nes.PRG bin/HonkeyPong.nes.CHR bin/HonkeyPong.nes.PRG.bin bin/HonkeyPong.nes.CHR.bin
