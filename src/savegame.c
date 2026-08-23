#include <cbm.h>
#include <stdint.h>
#include "savegame.h"

#define SAVE_LFN 2u
#define SAVE_DEVICE 8u

static uint8_t checksum(const SaveData* data) {
    const uint8_t* bytes = (const uint8_t*)data;
    uint8_t sum = 0xA7u;
    uint8_t i;
    for (i = 0u; i < (uint8_t)(sizeof(SaveData) - 1u); ++i) sum = (uint8_t)((sum << 1) | (sum >> 7)) ^ bytes[i];
    return sum;
}

uint8_t savegame_write(SaveData* data) {
    int written;
    data->checksum = checksum(data);
    if (cbm_open(SAVE_LFN, SAVE_DEVICE, CBM_WRITE, "@0:P7ASH.SAV,S,W")) return 0u;
    written = cbm_write(SAVE_LFN, data, sizeof(SaveData));
    cbm_close(SAVE_LFN);
    return written == sizeof(SaveData);
}

uint8_t savegame_read(SaveData* data) {
    int read;
    if (cbm_open(SAVE_LFN, SAVE_DEVICE, CBM_READ, "0:P7ASH.SAV,S,R")) return 0u;
    read = cbm_read(SAVE_LFN, data, sizeof(SaveData));
    cbm_close(SAVE_LFN);
    if (read != sizeof(SaveData)) return 0u;
    if (data->magic[0] != 'P' || data->magic[1] != '7' || data->magic[2] != 'A' || data->magic[3] != 'S') return 0u;
    if (data->version != SAVE_VERSION) return 0u;
    return data->checksum == checksum(data);
}

