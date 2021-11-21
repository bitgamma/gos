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

#define SB16_BASE_PORT 0x220

#define SB16_MIXER_CMD_PORT 0x4
#define SB16_MIXER_DATA_PORT 0x5
#define SB16_RESET_PORT 0x6
#define SB16_READ_PORT 0xa
#define SB16_CMD_DATA_PORT 0xc
#define SB16_CMD_STS_PORT SB16_CMD_DATA_PORT
#define SB16_READ_STS_PORT 0xe
#define SB16_IRQ_ACK16_PORT 0xf

#define SB16_CMD_VERSION 0xe1


#define SB16_RESET_ACK 0xaa
#define SB16_DSP_VERSION 4

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
    dbg_log_string("sb16 found: 0\n");
    return false;
  }

  bool ok;

  ok = sb16_cmd(SB16_CMD_VERSION);

  uint8_t version;
  ok &= sb16_read(&version);
  ok &= version == SB16_DSP_VERSION;
  ok &= sb16_read(&version);

  dbg_log_string("sb16 found: ");
  dbg_log_uint8(ok);

	return ok;
}
