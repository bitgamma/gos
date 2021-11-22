/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fmt_pcm.h>
#include <dma.h>

bool fmt_pcm_run(fmt_pcm_context_t* ctx) {
  dma_block_t* buf = dma_get_current();
  bool finished = false;

  if (buf == NULL) {
    return finished;
  }

  uint32_t remaining = ctx->length - ctx->position;

  if (buf->status & DMA_BLOCK_DIRTY) {
    int16_t* data = (int16_t*) ctx->data;
    int16_t* dst = (int16_t*) buf->data;
    remaining = (remaining > DMA16_BLOCK_COUNT ? DMA_BLOCK_COUNT : remaining) >> 1;

    for (uint32_t i = 0; i < remaining; i++) {
      int32_t mixed = data[i] + dst[i];

      if (mixed < INT16_MIN) {
        mixed = INT16_MIN;
      } else if (mixed > INT16_MAX) {
        mixed = INT16_MAX;
      }

      dst[i] = mixed;
    }

    ctx->position += (remaining << 1);
  } else {
    uint8_t* data = (uint8_t*) ctx->data;
    uint8_t* dst = (uint8_t*) buf->data;

    if (remaining < DMA_BLOCK_SIZE) {
      memcpy(dst, &data[ctx->position], remaining);
      dst += remaining;
      remaining = DMA_BLOCK_SIZE - remaining;
      memset(dst, 0, remaining);
      ctx->position = 0;
      finished = true;
    } else {
      memcpy(buf->data, &data[ctx->position], DMA_BLOCK_SIZE);
      ctx->position += DMA_BLOCK_SIZE;
    }
  }

  if (ctx->position >= ctx->length) {
    ctx->position = 0;
    finished = true;
  }

  buf->status |= DMA_BLOCK_DIRTY;
  return finished;
}