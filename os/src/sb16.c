/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sb16.h>
#include <port.h>
#include <dbg.h>
#include <dma.h>
#include <pic.h>

#define SB16_BASE_PORT 0x220

#define SB16_MIXER_CMD_PORT 0x4
#define SB16_MIXER_DATA_PORT 0x5
#define SB16_RESET_PORT 0x6
#define SB16_READ_PORT 0xa
#define SB16_CMD_DATA_PORT 0xc
#define SB16_CMD_STS_PORT SB16_CMD_DATA_PORT
#define SB16_READ_STS_PORT 0xe
#define SB16_IRQ_ACK16_PORT 0xf

#define SB16_MIXER_SET_IRQ 0x80
#define SB16_MIXER_SET_DMA 0x81

#define SB16_MIXER_MASTER_LEFT 0x30
#define SB16_MIXER_MASTER_RIGHT 0x31
#define SB16_MIXER_VOICE_LEFT 0x32
#define SB16_MIXER_VOICE_RIGHT 0x33

#ifdef SB16_VOLUME
#define _SB16_VOLUME (SB16_VOLUME << 3)
#endif

#define SB16_CMD_SET_RATE 0x41
#define SB16_CMD_DMA16_TRANSFER 0xb6
#define SB16_CMD_DMA16_OFF 0xd9
#define SB16_CMD_VERSION 0xe1

#define SB16_RESET_ACK 0xaa
#define SB16_DSP_VERSION 4
#define SB16_IRQ5 0x02
#define SB16_DMA5 0x20
#define SB16_MONO 0x10
#define SB16_STEREO 0x30

#define SB_TIMEOUT 1000

bool sb16_cmd(uint8_t data) {
	for (uint16_t i = 0; i < SB_TIMEOUT; i++) {
		if ((inb(SB16_BASE_PORT+SB16_CMD_STS_PORT) & 0x80) == 0) {
			outb(SB16_BASE_PORT+SB16_CMD_DATA_PORT, data);
			return true;
		}

    iowait();
  }

  return false;
}

bool sb16_read(uint8_t* data) {
	for (uint16_t i = 0; i < SB_TIMEOUT; i++) {
		if ((inb(SB16_BASE_PORT+SB16_READ_STS_PORT) & 0x80)) {
			*data = inb(SB16_BASE_PORT+SB16_READ_PORT);
			return true;
		}

    iowait();
  }

  return false;
}

bool sb16_init() {
	outb(SB16_BASE_PORT+SB16_RESET_PORT, 1);
	iowait();
  iowait();
  iowait();
	outb(SB16_BASE_PORT+SB16_RESET_PORT, 0);

  uint8_t ack;
  sb16_read(&ack);

  if (ack != SB16_RESET_ACK) {
    dbg_log_string("sb16: not detected\n");
    return false;
  }

  bool ok = sb16_cmd(SB16_CMD_VERSION);

  uint8_t version;
  ok &= sb16_read(&version);
  ok &= version == SB16_DSP_VERSION;
  ok &= sb16_read(&version);

  if (ok) {
    outb(SB16_BASE_PORT+SB16_MIXER_CMD_PORT, SB16_MIXER_SET_IRQ);
    outb(SB16_BASE_PORT+SB16_MIXER_DATA_PORT, SB16_IRQ5);

    outb(SB16_BASE_PORT+SB16_MIXER_CMD_PORT, SB16_MIXER_SET_DMA);
    outb(SB16_BASE_PORT+SB16_MIXER_DATA_PORT, SB16_DMA5);
#ifdef SB16_VOLUME
    outb(SB16_BASE_PORT+SB16_MIXER_CMD_PORT, SB16_MIXER_MASTER_LEFT);
    outb(SB16_BASE_PORT+SB16_MIXER_DATA_PORT, _SB16_VOLUME);

    outb(SB16_BASE_PORT+SB16_MIXER_CMD_PORT, SB16_MIXER_MASTER_RIGHT);
    outb(SB16_BASE_PORT+SB16_MIXER_DATA_PORT, _SB16_VOLUME);

    outb(SB16_BASE_PORT+SB16_MIXER_CMD_PORT, SB16_MIXER_VOICE_LEFT);
    outb(SB16_BASE_PORT+SB16_MIXER_DATA_PORT, _SB16_VOLUME);

    outb(SB16_BASE_PORT+SB16_MIXER_CMD_PORT, SB16_MIXER_VOICE_RIGHT);
    outb(SB16_BASE_PORT+SB16_MIXER_DATA_PORT, _SB16_VOLUME);
#endif
    pic_enable_irq(PIC_LPT2);
    dbg_log_string("sb16: detected\n");
  } else {
    dbg_log_string("sb16: unsupported version\n");
  }

	return ok;
}

void sb16_transfer_start(uint32_t rate, bool mono) {
  isa_dma_setup(DMA16_CH5, DMA16_TRANSFER_AUTO | DMA16_TRANSFER_M2P | DMA16_TRANSFER_SINGLE);
  sb16_cmd(SB16_CMD_SET_RATE);
  sb16_cmd(rate >> 8);
  sb16_cmd(rate & 0xff);
  sb16_cmd(SB16_CMD_DMA16_TRANSFER);
  sb16_cmd(mono ? SB16_MONO : SB16_STEREO);
  sb16_cmd(DMA16_BLOCK_COUNT & 0xff);
  sb16_cmd(DMA16_BLOCK_COUNT >> 8);
  dma_start_transfer();
}

void sb16_transfer_stop() {
  sb16_cmd(SB16_CMD_DMA16_OFF);
}

void sb16_transfer_finished() {
  dma_block_transfered();
  inb(SB16_BASE_PORT+SB16_IRQ_ACK16_PORT);
}