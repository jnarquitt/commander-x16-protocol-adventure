TARGET := protocol
BUILD := build
SRC := src/main.c src/protocol_dice.c
CC := cl65
CFLAGS := -t cx16 -Oirs -I include

.PHONY: all run clean

all: $(BUILD)/PROTOCOL.PRG

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/PROTOCOL.PRG: $(SRC) include/protocol_dice.h | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $(SRC)

run: all
	@test -n "$(EMULATOR)" || (echo "Set EMULATOR=/path/to/x16emu" && exit 1)
	$(EMULATOR) -prg $(BUILD)/PROTOCOL.PRG -run

clean:
	rm -f $(BUILD)/*.PRG $(BUILD)/*.o

