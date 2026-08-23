TARGET := protocol
BUILD := build
SRC := src/main.c src/protocol_dice.c src/game_content.c src/x16_audio.c src/savegame.c src/generated_portraits.c
CC := cl65
CFLAGS := -t cx16 -Oirs -I include
LDFLAGS := -m $(BUILD)/PROTOCOL.map -Ln $(BUILD)/PROTOCOL.lbl

.PHONY: all assets run clean

all: $(BUILD)/PROTOCOL.PRG

assets:
	python3 tools/build_portraits.py

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/PROTOCOL.PRG: $(SRC) include/protocol_dice.h include/game_content.h include/x16_audio.h include/savegame.h include/generated_portraits.h | $(BUILD)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

run: all
	@test -n "$(EMULATOR)" || (echo "Set EMULATOR=/path/to/x16emu" && exit 1)
	$(EMULATOR) -prg $(BUILD)/PROTOCOL.PRG -run

clean:
	rm -f $(BUILD)/*.PRG $(BUILD)/*.o
