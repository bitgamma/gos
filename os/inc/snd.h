#ifndef __SND__
#define __SND__

#include <stdint.h>

typedef enum {
  FMT_DRO,
  FMT_WAV
} snd_type_t;

typedef struct {
  snd_type_t type;
  uint32_t len;
  void* data;
} snd_source_t;

#endif