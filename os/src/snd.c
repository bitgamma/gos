#include <snd.h>
#include <mem.h>
#include <opl3.h>
#include <ac97.h>
#include <fmt_dro.h>

#define SND_SRC_SIZE 4

static snd_source_t* _snd_sources[SND_SRC_SIZE];
static bool _snd_sink_opl3;
static bool _snd_sink_wav;

void snd_init() {
  _snd_sink_opl3 = opl3_init();
#if AC97_ENABLED
  _snd_sink_wav = ac97_init();
#endif
}

snd_id_t snd_play(snd_source_t* source) {
  uint8_t idx;

  for(idx = 0; idx < SND_SRC_SIZE; idx++) {
    if (_snd_sources[idx] == NULL) {
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

  _snd_sources[idx] = source;

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
    if (_snd_sources[i] != NULL) {
      if (_snd_run(_snd_sources[i])) {
        _snd_sources[i] = NULL;
      }
    }
  }
}

void snd_stop(snd_id_t id) {
  _snd_sources[id] = NULL;
}