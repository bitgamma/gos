#ifndef __AC97__
#define __AC97__

#include <stdint.h>
#include <stdbool.h>

#define AC97_BUFFER_SIZE 512

bool ac97_init();
uint16_t* ac97_next_buffer();
void ac97_play();
void ac97_stop();

#endif