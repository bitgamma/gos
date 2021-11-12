#include <snd.h>
#include <mem.h>
#include <opl3.h>
#include <ac97.h>
#include <fmt_dro.h>

#define SND_SOURCES ((snd_source_t**)(SND_SRC_ADDR))

static bool _snd_sink_opl3;
static bool _snd_sink_wav;

void snd_init() {
  _snd_sink_opl3 = opl3_init();
  _snd_sink_wav = ac97_init();

  memset32((uint32_t*)SND_SRC_ADDR, 0, SND_SRC_SIZE);
}

snd_id_t snd_play(snd_source_t* source) {
  uint8_t idx;

  for(idx = 0; idx < SND_SRC_SIZE; idx++) {
    if (SND_SOURCES[idx] == NULL) {
      break;
    }
  }

  if (idx == SND_SRC_SIZE) {
    return SNDID_ERR;
  }

  if (source->type == FMT_DRO && _snd_sink_opl3) {
    if (!fmt_dro_init((fmt_dro_context_t*) source->ctx, source->data, (fmt_dro_hw_t)(opl3_get_type() - 1))) {
      return SNDID_ERR;
    }
  } else if (source->type == FMT_WAV && _snd_sink_wav) {
    // not supported yet
    return SNDID_ERR;
  } else {
    return SNDID_ERR;
  }

  SND_SOURCES[idx] = source;

  return idx;
}

static bool _snd_run(snd_source_t* source) {
  bool finished = false;
  if (source->type == FMT_DRO) {
    finished = fmt_dro_run((fmt_dro_context_t*) source->ctx, opl3_write);
  } else {
    //not supported yet
  }

  return finished && !source->loop;
}

void snd_run() {
  for(uint8_t i = 0; i < SND_SRC_SIZE; i++) {
    if (SND_SOURCES[i] != NULL) {
      if (_snd_run(SND_SOURCES[i])) {
        SND_SOURCES[i] = NULL;
      }
    }
  }
}

void snd_stop(snd_id_t id) {
  SND_SOURCES[id] = NULL;
}