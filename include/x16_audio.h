#ifndef X16_AUDIO_H
#define X16_AUDIO_H

#include <stdint.h>

void audio_init(void);
void audio_stop(void);
void audio_confirm(void);
void audio_success(void);
void audio_failure(void);
void audio_alarm(void);
void audio_step(uint8_t frame);
void audio_music_start(void);
void audio_music_tick(void);
void audio_music_stop(void);

#endif
