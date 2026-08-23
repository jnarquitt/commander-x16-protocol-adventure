#include <cx16.h>
#include <cbm.h>
#include <stdint.h>
#include "x16_audio.h"

#define PSG_BASE 0x1F9C0UL

static uint8_t music_on = 0u, music_clock = 0u, music_note = 0u;

static void voice(uint8_t id, uint16_t frequency, uint8_t volume, uint8_t waveform) {
    unsigned long address = PSG_BASE + ((unsigned long)id * 4UL);
    vpoke((uint8_t)(frequency & 0xFFu), address);
    vpoke((uint8_t)(frequency >> 8), address + 1UL);
    vpoke(volume ? (uint8_t)(0xC0u | (volume & 0x3Fu)) : 0u, address + 2UL);
    vpoke(waveform, address + 3UL);
}

static void hold(uint8_t frames) { while (frames--) waitvsync(); }

void audio_init(void) { voice(0u, 0u, 0u, 0x3Fu); voice(1u, 0u, 0u, 0xBFu); voice(2u, 0u, 0u, 0x7Fu); }
void audio_stop(void) { voice(0u, 0u, 0u, 0x3Fu); }

void audio_confirm(void) {
    voice(0u, 1181u, 28u, 0xBFu); hold(3u); audio_stop();
}

void audio_success(void) {
    voice(0u, 885u, 30u, 0xBFu); hold(4u);
    voice(0u, 1181u, 32u, 0xBFu); hold(5u); audio_stop();
}

void audio_failure(void) {
    voice(0u, 790u, 28u, 0x7Fu); hold(5u);
    voice(0u, 592u, 30u, 0x7Fu); hold(7u); audio_stop();
}

void audio_alarm(void) {
    voice(0u, 420u, 34u, 0xFFu); hold(5u);
    voice(0u, 760u, 34u, 0xFFu); hold(5u); audio_stop();
}

void audio_step(uint8_t frame) {
    voice(0u, frame ? 150u : 120u, 8u, 0xFFu); hold(1u); audio_stop();
}

void audio_music_start(void) {
    music_on = 1u; music_clock = 0u; music_note = 0u;
}

void audio_music_tick(void) {
    static const uint16_t melody[8] = {395u,498u,592u,498u,443u,527u,664u,527u};
    static const uint16_t bass[8] = {198u,198u,222u,222u,166u,166u,198u,198u};
    if (!music_on) return;
    if (++music_clock < 24u) return;
    music_clock = 0u; music_note = (uint8_t)((music_note + 1u) & 7u);
    voice(1u, melody[music_note], 13u, 0xBFu);
    voice(2u, bass[music_note], 9u, 0x7Fu);
}

void audio_music_stop(void) {
    music_on = 0u; voice(1u, 0u, 0u, 0xBFu); voice(2u, 0u, 0u, 0x7Fu);
}
