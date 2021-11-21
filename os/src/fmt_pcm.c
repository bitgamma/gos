/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fmt_pcm.h>
#include <dma.h>

void fmt_pcm_run(fmt_pcm_context_t* ctx) {
  dma_block_t* buf = dma_get_current();

  if (buf == NULL) {
    return;
  }

  uint8_t* data = (uint8_t*) ctx->data;

  if (buf->status & DMA_BLOCK_DIRTY) {
    //TODO: mix
  } else {
    uint32_t remaining = ctx->length - ctx->position;
    if (remaining < DMA_BLOCK_SIZE) {
      memcpy(buf->data, &data[ctx->position], remaining);
      void* silence = &data[remaining];
      remaining = DMA_BLOCK_SIZE - remaining;
      memset(silence, 0, remaining);
      ctx->position = 0;
    } else {
      memcpy(buf->data, &data[ctx->position], DMA_BLOCK_SIZE);
      ctx->position += DMA_BLOCK_SIZE;
      if (ctx->position >= ctx->length) {
        ctx->position = 0;
      }
    }
  }

  buf->status |= DMA_BLOCK_DIRTY;
}