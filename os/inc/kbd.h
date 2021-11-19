/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __KBD__
#define __KBD__

#include <stdint.h>
#include <stdbool.h>
#include <bitarray.h>

#define KBD_RELEASED 15
#define KBD_REPEAT 14
#define KBD_CTRL 13
#define KBD_SHIFT 12
#define KBD_ALT 11

#define KBD_IS_RELEASED(e) BIT_IS_SET(e, KBD_RELEASED)
#define KBD_IS_REPEAT(e) BIT_IS_SET(e, KBD_REPEAT)
#define KBD_ALT_MODIFIER(e) BIT_IS_SET(e, KBD_ALT)
#define KBD_SHIFT_MODIFIER(e) BIT_IS_SET(e, KBD_SHIFT)
#define KBD_CTRL_MODIFIER(e) BIT_IS_SET(e, KBD_CTRL)

#define KBD_SCANCODE(k) (k & 0x1ff)

#ifdef PS2_USE_SCANCODE2
#define KBD_KEY_LALT 0x11
#define KBD_KEY_LSHIFT 0x12
#define KBD_KEY_LCTRL 0x14

#define KBD_KEY_RALT 0x91
#define KBD_KEY_RSHIFT 0x59
#define KBD_KEY_RCTRL 0x94

#define KBD_KEY_LEFT 0xeb
#define KBD_KEY_DOWN 0xf2
#define KBD_KEY_RIGHT 0xf4
#define KBD_KEY_UP 0xf5

#define KBD_KEY_ENTER 0x5a
#define KBD_KEY_SPACE 0x29
#define KBD_KEY_ESC 0x76

#define KBD_KEY_NUM_0 0x70
#define KBD_KEY_NUM_1 0x69
#define KBD_KEY_NUM_2 0x72
#define KBD_KEY_NUM_3 0x7a
#define KBD_KEY_NUM_4 0x6b
#define KBD_KEY_NUM_5 0x73
#define KBD_KEY_NUM_6 0x74
#define KBD_KEY_NUM_7 0x6c
#define KBD_KEY_NUM_8 0x75
#define KBD_KEY_NUM_9 0x7d

#define KBD_KEY_M 0x3a
#else
#define KBD_KEY_LALT 0x38
#define KBD_KEY_LSHIFT 0x2a
#define KBD_KEY_LCTRL 0x1d

#define KBD_KEY_RALT 0xb8
#define KBD_KEY_RSHIFT 0x36
#define KBD_KEY_RCTRL 0x9d

#define KBD_KEY_LEFT 0xcb
#define KBD_KEY_DOWN 0xd0
#define KBD_KEY_RIGHT 0xcd
#define KBD_KEY_UP 0xc8

#define KBD_KEY_ENTER 0x1c
#define KBD_KEY_SPACE 0x39
#define KBD_KEY_ESC 0x01

#define KBD_KEY_NUM_0 0x52
#define KBD_KEY_NUM_1 0x4f
#define KBD_KEY_NUM_2 0x50
#define KBD_KEY_NUM_3 0x51
#define KBD_KEY_NUM_4 0x4b
#define KBD_KEY_NUM_5 0x4c
#define KBD_KEY_NUM_6 0x4d
#define KBD_KEY_NUM_7 0x47
#define KBD_KEY_NUM_8 0x48
#define KBD_KEY_NUM_9 0x49

#define KBD_KEY_M 0x32
#endif

typedef uint16_t kbd_event;

void kbd_ps2_rcv();
bool kbd_read(kbd_event *evt);
void kbd_flush();
bool kbd_any_pressed();
void kbd_stuck();

#endif