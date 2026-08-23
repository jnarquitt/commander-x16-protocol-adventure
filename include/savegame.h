#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <stdint.h>

#define SAVE_VERSION 1u

typedef struct {
    uint8_t magic[4];
    uint8_t version;
    uint8_t px;
    uint8_t py;
    uint8_t party_hp[3];
    uint8_t nanogel;
    uint8_t rescued;
    uint8_t carbon;
    uint8_t telegram;
    uint8_t quill_met;
    uint8_t carrier;
    uint8_t exposure;
    uint8_t fire_clock;
    uint8_t little_giant;
    uint8_t venn_done;
    uint8_t checksum;
} SaveData;

uint8_t savegame_write(SaveData* data);
uint8_t savegame_read(SaveData* data);

#endif

