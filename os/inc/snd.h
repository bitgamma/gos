#ifndef __SND__
#define __SND__

#include <stdint.h>
#include <stdbool.h>

#define SNDID_ERR -1

typedef enum {
  FMT_DRO,
  FMT_WAV
} snd_type_t;

typedef struct {
  snd_type_t type;
  void* ctx;
  bool loop;
  void* data;
} snd_source_t;

typedef int16_t snd_id_t;

void snd_init();
void snd_run();
void snd_stop(snd_id_t id);
snd_id_t snd_play(snd_source_t* source);

#endif