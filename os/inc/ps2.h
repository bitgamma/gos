/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __PS2__
#define __PS2__

#include <port.h>

#define PS2_DATA_PORT 0x60
#define PS2_CMD_PORT 0x64

#define PS2_READ_CONFIG 0x20
#define PS2_WRITE_CONFIG 0x60
#define PS2_DISABLE_PORT2 0xA7
#define PS2_ENABLE_PORT2 0xA8
#define PS2_TEST_PORT2 0xA9
#define PS2_TEST_CONTROLLER 0xAA
#define PS2_TEST_PORT1 0xAB
#define PS2_DISABLE_PORT1 0xAD
#define PS2_ENABLE_PORT1 0xAE
#define PS2_WRITE_OUTPUT_PORT 0xC0
#define PS2_WRITE_PORT2 0xD4

#define PS2_CCB_PORT1_INT 0
#define PS2_CCB_PORT2_INT 1
#define PS2_SYSFLAG 2
#define PS2_CCB_PORT1_CLCK 4
#define PS2_CCB_PORT2_CLKC 5
#define PS2_CCB_TRANSLATION 6

#define PS2_OUT_SYSRST 0
#define PS2_OUT_A20GATE 1
#define PS2_OUT_PORT2_CLCK 2
#define PS2_OUT_PORT2_DATA 3
#define PS2_OUT_PORT1_FULL 4
#define PS2_OUT_PORT2_FULL 5
#define PS2_OUT_PORT1_CLCK 6
#define PS2_OUT_PORT1_DATA 7

#define PS2_STS_OUTFULL 0
#define PS2_STS_INFULL 1
#define PS2_STS_SYSFLAG 2
#define PS2_STS_CMDDATA 3
#define PS2_STS_TIMEOUT 6
#define PS2_STS_PARITY 7

#define PS2_REBOOT (~(1 << PS2_OUT_SYSRST))

#define PS2_KBD_SET_LED 0xED
#define PS2_KBD_SCANCODE 0xF0
#define PS2_KBD_IDENTIFY 0xF2
#define PS2_KBD_SET_TYPEMATIC 0xF3
#define PS2_KBD_ENABLE_SCAN 0xF4
#define PS2_KBD_DISABLE_SCAN 0xF5
#define PS2_KBD_RESET 0xFF

#define PS2_KBD_SCANCODE_GET 0
#define PS2_KBD_SCANCODE1 1
#define PS2_KBD_SCANCODE2 2
#define PS2_KBD_SCANCODE3 3

#define PS2_KBD_ACK 0xFA
#define PS2_KBD_NACK 0xFE
#define PS2_KBD_TEST_OK 0xAA

void ps2_init();
void ps2_reboot();

#endif