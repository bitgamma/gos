/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <snd.h>
#include <mem.h>
#include <opl3.h>
#include <sb16.h>
#include <fmt_dro.h>
#include <fmt_pcm.h>
#include <dma.h>

typedef enum {
  NOPCM, SB16, AC97, HDA
} snd_pcm_sink_t;

static bool _snd_sink_opl3;
static snd_pcm_sink_t _snd_sink_pcm;

static uint8_t _active_pcm_sources = 0;

void snd_init() {
  _snd_sink_opl3 = opl3_init();
  _snd_sink_pcm = sb16_init() ? SB16 : NOPCM;
}

static void _snd_pcm_start(fmt_pcm_context_t* pcm) {
  dma_reset_blocks();
  fmt_pcm_init(pcm);
  fmt_pcm_run(pcm);

  if (_snd_sink_pcm == SB16) {
    // assume all PCM audio will have the same format
    sb16_transfer_start(pcm->rate, pcm->channels == 1);
  }
}

static void _snd_pcm_stop() {
  if (!--_active_pcm_sources) {
    if (_snd_sink_pcm == SB16) {
      sb16_transfer_stop();
    }
  }
}

static bool _snd_pcm_run(fmt_pcm_context_t* pcm) {
  if (fmt_pcm_run(pcm)) {
    _snd_pcm_stop();
    return true;
  }

  return false;
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
  } else if (fmt_pcm_detect(ctx)) {
    if (stop) {
      fmt_pcm_stop((fmt_pcm_context_t*) ctx);
    }

    _snd_pcm_stop();
  }
}

task_desc_t snd_play(snd_source_t* source, bool loop) {
  task_cb_t cb;

  if (_snd_sink_opl3 && fmt_dro_detect(source)) {
    fmt_dro_init((fmt_dro_context_t*) source, opl3_write, (fmt_dro_hw_t)(opl3_get_type() - 1));
    cb = (task_cb_t) fmt_dro_run;
  } else if (_snd_sink_pcm && fmt_pcm_detect(source)) {
    if (!_active_pcm_sources++) {
      _snd_pcm_start((fmt_pcm_context_t*)source);
    } else {
      fmt_pcm_init((fmt_pcm_context_t*)source);
    }

    cb = loop ? (task_cb_t) fmt_pcm_run : (task_cb_t) _snd_pcm_run;
  } else {
    return TDESC_ERR;
  }

  return task_start(cb, source, loop);
}

void snd_stop(task_desc_t snd) {
  _snd_off(snd, true);
}

void snd_pause(task_desc_t snd) {
  _snd_off(snd, false);
}