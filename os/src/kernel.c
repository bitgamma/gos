/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <timer.h>
#include <mem.h>
#include <interrupt.h>
#include <pic.h>
#include <ps2.h>
#include <cmos.h>
#include <rnd.h>
#include <snd.h>
#include <pci.h>
#include <mouse.h>
#include <serial.h>

extern void app_main();

void _seed_rnd() {
  uint32_t s0 = 0, s1 = 0;
  uint32_t* bios_data = (uint32_t *)BIOS_DATA_ADDR;

  for (uint32_t i = 0; i < 128; i+=2) {
    s0 += *(bios_data++);
    s1 += cmos_read(i);
    s1 += *(bios_data++);
    s0 += cmos_read(i+1);
  }

  rnd_seed(s0, s1);
}

void kernel_main() {
  idt_init();
  _seed_rnd();
  pic_init();
  ps2_init();
#ifdef AC97_ENABLED
  pci_scan();
#endif
  snd_init();
  serial_init();
  systick_init();
  idt_enable();

  app_main();

  for(;;) {
    asm volatile ("hlt");
  }
}
