/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __DMA__
#define __DMA__

#include <stdint.h>
#include <stdbool.h>
#include <mem.h>

#define DMA_BLOCK_IN_TRANSFER 0x01
#define DMA_BLOCK_DIRTY 0x02

#ifndef DMA_BLOCK_COUNT
#define DMA_BLOCK_COUNT 16
#endif

#define DMA_BLOCK_SIZE (DMA_BUFFER_SIZE / DMA_BLOCK_COUNT)

#define DMA16_BUFFER_COUNT ((DMA_BUFFER_SIZE >> 1) - 1)
#define DMA16_BLOCK_COUNT ((DMA_BLOCK_SIZE >> 1) - 1)

#define DMA16_CHANNEL_DISABLE 0x04

#define DMA16_TRANSFER_TEST 0x00
#define DMA16_TRANSFER_P2M (0x01 << 2)
#define DMA16_TRANSFER_M2P (0x02 << 2)
#define DMA16_TRANSFER_AUTO 0x10
#define DMA16_TRANSFER_DOWN 0x20
#define DMA16_TRANSFER_ONDEMAND 0x0
#define DMA16_TRANSFER_SINGLE (0x01 << 6)
#define DMA16_TRANSFER_BLOCK (0x02 << 6)

typedef struct {
  void* data;
  uint8_t status;
} dma_block_t;

typedef enum {
  DMA16_CH5 = 1,
  DMA16_CH6 = 2,
  DMA16_CH7 = 3
} isa_dma_channel_t;

void dma_reset_blocks();
void dma_block_transferred();
dma_block_t* dma_get_block(uint8_t block_id);
uint8_t dma_get_next();
void dma_start_transfer();
void isa_dma_setup(isa_dma_channel_t ch, uint8_t mode);

#endif