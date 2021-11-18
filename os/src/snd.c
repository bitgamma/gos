#include <snd.h>
#include <mem.h>
#include <opl3.h>
#include <ac97.h>
#include <fmt_dro.h>

static bool _snd_sink_opl3;
#if AC97_ENABLED
static bool _snd_sink_wav;
#endif

void snd_init() {
  _snd_sink_opl3 = opl3_init();
#if AC97_ENABLED
  _snd_sink_wav = ac97_init();
#endif
}

task_desc_t snd_play(snd_source_t* source, bool loop) {
  task_cb_t cb;

  if (_snd_sink_opl3 && fmt_dro_detect(source)) {
    fmt_dro_init((fmt_dro_context_t*) source, opl3_write, (fmt_dro_hw_t)(opl3_get_type() - 1));
    cb = (task_cb_t) fmt_dro_run;
  } else {
    return TDESC_ERR;
  }

  return task_start(cb, source, loop);
}

static void _snd_off(task_desc_t snd, bool stop) {
  void* ctx = task_get_context(snd);

  if (!ctx) {
    return;
  }

  task_stop(snd);

  if (fmt_dro_detect(ctx)) {
    if (stop) {
      fmt_dro_stop((fmt_dro_context_t*) ctx);
    }

    opl3_mute();
  }
}

void snd_stop(task_desc_t snd) {
  _snd_off(snd, true);
}

void snd_pause(task_desc_t snd) {
  _snd_off(snd, false);
}