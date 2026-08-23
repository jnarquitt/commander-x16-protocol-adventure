#include <cx16.h>
#include <cbm.h>
#include <stdint.h>
#include "x16_audio.h"

#define PSG_BASE 0x1F9C0UL

static void voice(uint16_t frequency, uint8_t volume, uint8_t waveform) {
    vpoke((uint8_t)(frequency & 0xFFu), PSG_BASE);
    vpoke((uint8_t)(frequency >> 8), PSG_BASE + 1UL);
    vpoke(volume ? (uint8_t)(0xC0u | (volume & 0x3Fu)) : 0u, PSG_BASE + 2UL);
    vpoke(waveform, PSG_BASE + 3UL);
}

static void hold(uint8_t frames) { while (frames--) waitvsync(); }

void audio_init(void) { voice(0u, 0u, 0x3Fu); }
void audio_stop(void) { voice(0u, 0u, 0x3Fu); }

void audio_confirm(void) {
    voice(1181u, 28u, 0xBFu); hold(3u); audio_stop();
}

void audio_success(void) {
    voice(885u, 30u, 0xBFu); hold(4u);
    voice(1181u, 32u, 0xBFu); hold(5u); audio_stop();
}

void audio_failure(void) {
    voice(790u, 28u, 0x7Fu); hold(5u);
    voice(592u, 30u, 0x7Fu); hold(7u); audio_stop();
}

void audio_alarm(void) {
    voice(420u, 34u, 0xFFu); hold(5u);
    voice(760u, 34u, 0xFFu); hold(5u); audio_stop();
}

void audio_step(uint8_t frame) {
    voice(frame ? 150u : 120u, 8u, 0xFFu); hold(1u); audio_stop();
}
