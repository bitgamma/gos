#ifndef __SND__
#define __SND__

#include <stdint.h>
#include <stdbool.h>
#include <task.h>

typedef enum {
  FMT_DRO,
  FMT_WAV
} snd_type_t;

typedef void snd_source_t;

void snd_init();
task_desc_t snd_play(snd_type_t type, snd_source_t* source, bool loop);

#endif