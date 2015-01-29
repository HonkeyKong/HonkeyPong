ifeq ($(OS),Windows_NT)
	TXT_TARGET=tools/text2data.exe
	NSF_TARGET=tools/nsf2data.exe
else
	TXT_TARGET=tools/text2data
	NSF_TARGET=tools/nsf2data
endif

TARGET=bin/HonkeyPong.nes

SOURCES=src/setup.oph \
		src/ppu.oph \
		src/render.oph \
		src/sprites.oph \
		src/input.oph \
		src/engine.oph \
		src/famitone2.oph \
		$(MUSIC_ASM) \
		src/main.oph

MUSIC_SRC=res/snd/HonkeyPong.txt

MUSIC_ASM=res/snd/HonkeyPong.oph

SOUND_SRC=res/snd/Sounds.nsf

SOUND_ASM=res/snd/Sounds.oph

TXT_SRC=src/text2data.cpp

NSF_SRC=src/nsf2data.cpp

MAIN_SRC=HonkeyPong_iNES.oph

SRC_MAP=HonkeyPong_Debug.map

BINARIES=$(SRC_MAP) \
		$(TARGET) \
		$(TARGET).PRG \
		$(TARGET).CHR \
		$(TARGET).PRG.bin \
		$(TARGET).CHR.bin

CXX=g++

all: soundtools sound rom

rom: $(TARGET)

$(TARGET): $(SOURCES)
	@echo Assembling iNES ROM...
	@mkdir -p bin
	@ophis -o $@ $(MAIN_SRC)

debugrom: $(SOURCES)
	@echo Assembling iNES ROM with debug info...
	@mkdir -p bin
	@ophis -o $(TARGET) $(MAIN_SRC) -m $(SRC_MAP)

debug: debugrom
	@echo Loading debugger...
	@Nintendulator $(TARGET)

soundtools: $(SND_SRC)
	@echo Building text2data...
	@$(CXX) -o $(TXT_TARGET) $(TXT_SRC)
	@echo Building nsf2data...
	@$(CXX) -o $(NSF_TARGET) $(NSF_SRC)

.PHONY: sound
sound: $(MUSIC_SRC)
	@echo Generating music data...
	@$(TXT_TARGET) $(MUSIC_SRC) -ophis
	@echo Generating sound data...
	@$(NSF_TARGET) $(SOUND_SRC) -ophis

.PHONY: clean-sound
clean-sound:
	@echo Cleaning music...
	@rm -f $(MUSIC_ASM)
	@echo Cleaning sound...
	@rm -f $(SOUND_ASM)

.PHONY: clean-soundtools
clean-soundtools:
	@echo Cleaning sound tools...
	@rm -f $(TXT_TARGET) $(NSF_TARGET)

.PHONY: cartridge
cartridge: rom
	@echo Preparing cartridge ROMs...
	@python tools/nesprep.py $(TARGET) -splitPRG -splitCHR -quiet

.PHONY: cartrdige-256
cartridge-256: rom
	@echo Preparing NROM-256 cartridge ROMs...
	@python tools/nesprep.py $(TARGET) -inflatePRG=0.25 -splitCHR -quiet

.PHONY: test
test: rom
	@echo Loading emulator...
	@Nestopia $(TARGET)

.PHONY: clean
clean:
	@echo Cleaning ROM...
	@rm -f $(BINARIES)

.PHONY: clean-all
clean-all: clean clean-sound clean-soundtools