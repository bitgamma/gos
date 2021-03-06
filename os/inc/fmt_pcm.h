/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __FMT_PCM__
#define __FMT_PCM__

#include <stdint.h>
#include <stdbool.h>
#include <dma.h>

#define PCMF 0x464d4350

typedef struct {
  uint32_t signature;
  uint32_t rate;
  uint32_t length;
  uint8_t channels;
  void* data;
  uint32_t position;
  uint8_t block_id;
} fmt_pcm_context_t;

inline void fmt_pcm_init(fmt_pcm_context_t* ctx) {
  ctx->block_id = dma_get_next();
}

inline bool fmt_pcm_detect(void* ctx) {
  return ((fmt_pcm_context_t *)ctx)->signature == PCMF;
}

inline void fmt_pcm_stop(fmt_pcm_context_t* ctx) {
  ctx->position = 0;
}

bool fmt_pcm_run(fmt_pcm_context_t* ctx);

#endif