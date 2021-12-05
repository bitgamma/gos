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
#define PS2_DISABLE_PORT2 0xa7
#define PS2_ENABLE_PORT2 0xa8
#define PS2_TEST_PORT2 0xa9
#define PS2_TEST_CONTROLLER 0xaa
#define PS2_TEST_PORT1 0xab
#define PS2_DISABLE_PORT1 0xad
#define PS2_ENABLE_PORT1 0xae
#define PS2_WRITE_OUTPUT_PORT 0xc0
#define PS2_WRITE_PORT2 0xd4

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

#define PS2_KBD_SET_LED 0xed
#define PS2_KBD_SCANCODE 0xf0
#define PS2_KBD_SET_TYPEMATIC 0xf3

#define PS2_MOUSE_SET_SAMPLE_RATE 0xf3
#define PS2_MOUSE_SET_RESOLUTION 0xe8
#define PS2_MOUSE_NONLINEAR 0xe7
#define PS2_MOUSE_LINEAR 0xe6

#define PS2_MOUSE_RES_1 0x00
#define PS2_MOUSE_RES_2 0x01
#define PS2_MOUSE_RES_4 0x02
#define PS2_MOUSE_RES_8 0x03

#define PS2_DEV_IDENTIFY 0xf2
#define PS2_DEV_ENABLE_SCAN 0xf4
#define PS2_DEV_DISABLE_SCAN 0xf5
#define PS2_DEV_RESET 0xff

#define PS2_KBD_SCANCODE_GET 0
#define PS2_KBD_SCANCODE1 1
#define PS2_KBD_SCANCODE2 2
#define PS2_KBD_SCANCODE3 3

#define PS2_DEV_ACK 0xfa
#define PS2_DEV_NACK 0xfe
#define PS2_DEV_TEST_OK 0xaa

#define PS2_DEV_STD_MOUSE 0x00
#define PS2_DEV_KBD 0xab
#define PS2_DEV_NACK 0xfe
#define PS2_DEV_TEST_OK 0xaa

extern uint8_t __ps2_mouse_packet_size;

void ps2_init();
void ps2_reboot();

#endif