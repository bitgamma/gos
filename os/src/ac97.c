/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ac97.h>
#include <pci.h>
#include <port.h>

#define AC97_NAM_RESET 0x00
#define AC97_NAM_MASTER_VOLUME 0x02
#define AC97_NAM_PCM_VOLUME 0x18

#define AC97_NABM_BDBA 0x10
#define AC97_NABM_CPE 0x14
#define AC97_NABM_LVE 0x15
#define AC97_NABM_TS 0x16
#define AC97_NABM_TC 0x1b
#define AC97_NABM_GCR 0x2c

#define AC97_RESET 0x02

#define AC97_BUFFER_COUNT 2

#define SND_BUF(a) (&_sound_buf[AC97_BUFFER_SIZE * a])

typedef struct __attribute__((packed)) {
  uint16_t* buf;
  uint16_t len;
  uint16_t flags;
} ac97_bdl_t;

static uint16_t _nam_port;
static uint16_t _nabm_port;
static bool _running;

static __attribute__((aligned(2))) uint16_t _sound_buf[AC97_BUFFER_COUNT*AC97_BUFFER_SIZE];
static __attribute__((aligned(4))) ac97_bdl_t _buffer_list[AC97_BUFFER_COUNT] = {
  {SND_BUF(0), ((AC97_BUFFER_SIZE >> 1) -1), 0},
  {SND_BUF(1), ((AC97_BUFFER_SIZE >> 1) -1), 0}
};

bool ac97_init() {
  if(AC97_DEV == 0) {
    return false;
  }

  pci_config_or(AC97_DEV, PCI_HEADER_STATUS_COMMAND, (PCI_COMMAND_BUSMASTER | PCI_COMMAND_PIO));
  _nam_port = pci_config_read(AC97_DEV, PCI_HEADER_BAR0) & 0xfffc;
  _nabm_port = pci_config_read(AC97_DEV, PCI_HEADER_BAR1) & 0xfffc;

  outb(_nabm_port+AC97_NABM_GCR, AC97_RESET);

  outb(_nabm_port+AC97_NABM_TC, AC97_RESET);
  while((inb(_nabm_port+AC97_NABM_TC) & AC97_RESET)) {
    asm volatile ("nop");
  }

  outw(_nam_port+AC97_NAM_RESET, AC97_RESET);
  outw(_nam_port+AC97_NAM_MASTER_VOLUME, 0);
  outw(_nam_port+AC97_NAM_PCM_VOLUME, 0x0808);

  outd(_nabm_port+AC97_NABM_BDBA, (uint32_t)_buffer_list);

  return true;
}

uint16_t* ac97_next_buffer() {
  if(!_running || inb(_nabm_port+AC97_NABM_CPE) == 1) {
    return SND_BUF(0);
  } else {
    return SND_BUF(1);
  }
}

void ac97_play() {
  outb(_nabm_port+AC97_NABM_LVE, (AC97_BUFFER_COUNT - 1));
  outb(_nabm_port+AC97_NABM_TC, 1);
  _running = true;
}

void ac97_stop() {
  outb(_nabm_port+AC97_NABM_TC, 0);
}
