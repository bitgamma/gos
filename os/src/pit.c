/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <pit.h>
#include <port.h>


void pit_init() {
  outb(PIT_CMD_PORT, PIT_CH0_SEL | PIT_ACC_LOHI | PIT_MODE_SWG);
  outb(PIT_CH0_DATA_PORT, (PIT_1MS_DIV & 0xff));
	outb(PIT_CH0_DATA_PORT, ((PIT_1MS_DIV >> 8) & 0xff));
}