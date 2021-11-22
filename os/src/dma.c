/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <dma.h>
#include <dbg.h>
#include <port.h>

#define DMA16_CH5_PAGE_PORT 0x8b
#define DMA16_CH6_PAGE_PORT 0x89
#define DMA16_CH7_PAGE_PORT 0x8a

#define DMA16_CH5_ADDR_PORT 0xc4
#define DMA16_CH5_COUNT_PORT 0xc6
#define DMA16_CH6_ADDR_PORT 0xc8
#define DMA16_CH6_COUNT_PORT 0xca
#define DMA16_CH7_ADDR_PORT 0xcc
#define DMA16_CH7_COUNT_PORT 0xce

#define DMA16_CHANNEL_MASK_PORT 0xd4
#define DMA16_MODE_PORT 0xd6
#define DMA16_RESET_PORT 0xd8
#define DMA16_MASTER_RESET_PORT 0xda
#define DMA16_MASK_RESET_PORT 0xdc
#define DMA16_MULTIMASK_PORT 0xde

static dma_block_t _dma_blocks[DMA_BLOCK_COUNT];
static int8_t _in_transfer;

void dma_reset_blocks() {
  _in_transfer = -1;

  for (uint8_t i = 0; i < DMA_BLOCK_COUNT; i++) {
    _dma_blocks[i].data = (void*)(DMA_BUFFER_ADDR + (i * DMA_BLOCK_SIZE));
    _dma_blocks[i].status = 0;
  }
}

void dma_block_transfered() {
  _dma_blocks[_in_transfer].status = 0;
  _in_transfer = (_in_transfer + 1) % DMA_BLOCK_COUNT;
  _dma_blocks[_in_transfer].status |= DMA_BLOCK_IN_TRANSFER;
}

dma_block_t* dma_get_block(uint8_t block_id) {
  return &_dma_blocks[block_id];
}

uint8_t dma_get_next() {
  return (_in_transfer + 1) % DMA_BLOCK_COUNT;
}

void dma_start_transfer() {
  _in_transfer = 0;
  _dma_blocks[_in_transfer].status |= DMA_BLOCK_IN_TRANSFER;
}

void isa_dma_setup(isa_dma_channel_t ch, uint8_t mode) {
  outb(DMA16_CHANNEL_MASK_PORT, ch | DMA16_CHANNEL_DISABLE);
  outb(DMA16_RESET_PORT, 1);
  outb(DMA16_MODE_PORT, mode | ch);

  uint16_t page_port;
  uint16_t addr_port;
  uint16_t count_port;

  switch(ch) {
    case DMA16_CH5:
      page_port = DMA16_CH5_PAGE_PORT;
      addr_port = DMA16_CH5_ADDR_PORT;
      count_port = DMA16_CH5_COUNT_PORT;
      break;
    case DMA16_CH6:
      page_port = DMA16_CH6_PAGE_PORT;
      addr_port = DMA16_CH6_ADDR_PORT;
      count_port = DMA16_CH6_COUNT_PORT;
      break;
    case DMA16_CH7:
      page_port = DMA16_CH7_PAGE_PORT;
      addr_port = DMA16_CH7_ADDR_PORT;
      count_port = DMA16_CH7_COUNT_PORT;
      break;
    default:
      dbg_log_string("dma: invalid isa dma channel\n");
      return;
  }

  outb(page_port, ((DMA_BUFFER_ADDR >> 16) & 0xff));
  outb(addr_port, ((DMA_BUFFER_ADDR >> 1) & 0xff));
  outb(addr_port, ((DMA_BUFFER_ADDR >> 9) & 0xff));
  outb(count_port, (DMA16_BUFFER_COUNT & 0xff));
  outb(count_port, ((DMA16_BUFFER_COUNT >> 8) & 0xff));
  outb(DMA16_CHANNEL_MASK_PORT, ch);
}