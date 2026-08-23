#ifndef PROTOCOL_DICE_H
#define PROTOCOL_DICE_H

#include <stdint.h>

#define PROTOCOL_MAX_DICE 8

typedef struct {
    uint8_t sides[PROTOCOL_MAX_DICE];
    uint8_t count;
} ProtocolPool;

typedef struct {
    uint8_t faces[PROTOCOL_MAX_DICE];
    uint8_t count;
    uint8_t total;
} ProtocolResult;

void protocol_seed(uint16_t seed);
void protocol_roll(const ProtocolPool* pool, ProtocolResult* result);
uint8_t protocol_test(const ProtocolPool* pool, uint8_t difficulty);

#endif
