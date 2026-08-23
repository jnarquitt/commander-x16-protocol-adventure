#ifndef GAME_CONTENT_H
#define GAME_CONTENT_H

#include <stdint.h>

#define MAP_W 32u
#define MAP_H 18u
#define PARTY_SIZE 3u

typedef struct {
    const char* name;
    const char* role;
    uint8_t max_hp;
    uint8_t color;
    uint8_t attack[4];
    uint8_t defense[4];
    uint8_t influence[4];
} VectorDef;

extern const char* const ash_map[MAP_H];
extern const VectorDef party_defs[PARTY_SIZE];

const char* carrier_name(uint8_t carrier);
const char* exposure_name(uint8_t exposure);

#endif

