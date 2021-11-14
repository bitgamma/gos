#include <snd.h>
#include <mem.h>
#include <opl3.h>
#include <ac97.h>
#include <fmt_dro.h>

static bool _snd_sink_opl3;
static bool _snd_sink_wav;

void snd_init() {
  _snd_sink_opl3 = opl3_init();
#if AC97_ENABLED
  _snd_sink_wav = ac97_init();
#endif
}

task_desc_t snd_play(snd_type_t type, snd_source_t* source, bool loop) {
  task_cb_t cb;

  if (type == FMT_DRO && _snd_sink_opl3) {
    if (!fmt_dro_init((fmt_dro_context_t*) source, opl3_write, (fmt_dro_hw_t)(opl3_get_type() - 1))) {
      return TDESC_ERR;
    }

    cb = (task_cb_t) fmt_dro_run;
  } else if (type == FMT_WAV && _snd_sink_wav) {
    return TDESC_ERR;
  } else {
    return TDESC_ERR;
  }

  return task_start(cb, source, loop);
}
