TARGET=bin/HonkeyPong.nes

SOURCES=src/setup.oph \
		src/ppu.oph \
		src/render.oph \
		src/sprites.oph \
		src/input.oph \
		src/engine.oph \
		src/famitone2.oph \
		src/main.oph

SND_SRC=tools/text2data/text2data.cpp

SND_TARGET_WIN=tools/text2data/text2data.exe

SND_TARGET_UNIX=tools/text2data/text2data

MAIN_SRC=HonkeyPong_iNES.oph

SRC_MAP=HonkeyPong_Debug.map

BINARIES=$(SRC_MAP) \
		$(TARGET) \
		$(TARGET).PRG \
		$(TARGET).CHR \
		$(TARGET).PRG.bin \
		$(TARGET).CHR.bin

CXX=g++

all: $(TARGET)

$(TARGET): $(SOURCES)
	@echo Assembling iNES ROM...
	@[[ -d bin ]] || mkdir bin
	@ophis -o $@ $(MAIN_SRC)

debugrom: $(SOURCES)
	@echo Assembling iNES ROM with debug info...
	@[[ -d bin ]] || mkdir bin
	@ophis -o $(TARGET) $(MAIN_SRC) -m $(SRC_MAP)

debug: debugrom
	@echo Loading debugger...
	@Nintendulator $(TARGET)

sndwin: $(SND_SRC)
	@echo Building text2data...
	@$(CXX) -o $(SND_TARGET_WIN) $(SND_SRC)

sndunix: $(SND_SRC)
	@echo Building text2data...
	@$(CXX) -o $(SND_TARGET_UNIX) $(SND_SRC)

.PHONY: cleansnd
cleansnd:
	@echo Cleaning sound tool...
	@rm -f $(SND_TARGET_UNIX) $(SND_TARGET_WIN)

.PHONY: cartridge
cartridge: $(TARGET)
	@echo Preparing cartridge ROMs...
	@python tools/nesprep.py $(TARGET) -splitPRG -splitCHR -quiet

.PHONY: cartrdige-256
cartridge-256: $(TARGET)
	@echo Preparing NROM-256 cartridge ROMs...
	@python tools/nesprep.py $(TARGET) -inflatePRG=0.25 -splitCHR -quiet

.PHONY: test
test: $(TARGET)
	@echo Loading emulator...
	@Nestopia $(TARGET)

.PHONY: clean
clean:
	@echo Cleaning up...
	@rm -f $(BINARIES)