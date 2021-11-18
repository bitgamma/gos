#ifndef __SND__
#define __SND__

#include <stdint.h>
#include <stdbool.h>
#include <task.h>

typedef void snd_source_t;

void snd_init();
task_desc_t snd_play(snd_source_t* source, bool loop);
void snd_stop(task_desc_t snd);

inline void snd_pause(task_desc_t snd) {
  task_stop(snd);
}

#endif