#include "protocol_dice.h"

static uint16_t rng_state = 0x4D3Bu;

void protocol_seed(uint16_t seed) {
    rng_state = seed ? seed : 0x4D3Bu;
}

static uint8_t random_byte(void) {
    uint16_t x = rng_state;
    x ^= (uint16_t)(x << 7);
    x ^= (uint16_t)(x >> 9);
    x ^= (uint16_t)(x << 8);
    rng_state = x;
    return (uint8_t)(x & 0xFFu);
}

static uint8_t roll_die(uint8_t sides) {
    uint8_t limit;
    uint8_t value;
    if (sides < 2u) return 0u;
    limit = (uint8_t)(255u - (255u % sides));
    do { value = random_byte(); } while (value >= limit);
    return (uint8_t)((value % sides) + 1u);
}

ProtocolResult protocol_roll(const ProtocolPool* pool) {
    ProtocolResult result;
    uint8_t i;
    result.count = pool->count > PROTOCOL_MAX_DICE ? PROTOCOL_MAX_DICE : pool->count;
    result.total = 0u;
    for (i = 0u; i < result.count; ++i) {
        result.faces[i] = roll_die(pool->sides[i]);
        if (result.faces[i] >= 4u) result.total = (uint8_t)(result.total + result.faces[i]);
    }
    return result;
}

uint8_t protocol_test(const ProtocolPool* pool, uint8_t difficulty) {
    return protocol_roll(pool).total >= difficulty;
}

