all: rom

rom:
	@echo Assembling iNES ROM...
	@ophis HonkeyPong_iNES.oph

debugrom:
	@echo Assembling iNES ROM with debug info...
	@ophis HonkeyPong_iNES_debug.oph -m HonkeyPong_Debug.map

cartridge: rom
	@echo Preparing cartridge ROMs...
	@python tools/nesprep.py bin/HonkeyPong.nes -splitPRG -splitCHR -quiet

test: rom
	@echo Loading emulator...
	@Nestopia bin/HonkeyPong.nes

debug: debugrom
	@echo Loading debugger...
	@Nintendulator bin/HonkeyPong.nes

clean:
	@echo Cleaning up...
	@rm -f HonkeyPong_Debug.map bin/HonkeyPong.nes bin/HonkeyPong.nes.PRG bin/HonkeyPong.nes.CHR
