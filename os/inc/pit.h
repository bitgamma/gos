/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __PIT__
#define __PIT__

#include <timer.h>

#define PIT_CH0_DATA_PORT 0x40
#define PIT_CH2_DATA_PORT 0x42
#define PIT_CMD_PORT 0x43

#define PIT_CH2_GATE_PORT 0x61

#define PIT_CH0_SEL 0x00
#define PIT_CH2_SEL 0x80

#define PIT_ACC_LATCH 0x00
#define PIT_ACC_LO 0x10
#define PIT_ACC_HI 0x20
#define PIT_ACC_LOHI 0x30

#define PIT_MODE_INT 0x00
#define PIT_MODE_ONESHOT 0x02
#define PIT_MODE_RATEGEN 0x04
#define PIT_MODE_SWG 0x06
#define PIT_MODE_SOFT_STROBE 0x08
#define PIT_MODE_HARD_STROBE 0x0A

#define PIT_OSC_HZ 1193182
#define PIT_DIV (PIT_OSC_HZ / (1000/TIMER_RES_MS))

void pit_init();

#endif