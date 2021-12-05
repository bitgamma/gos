/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __MOUSE__
#define __MOUSE__

#include <stdint.h>
#include <stdbool.h>

typedef enum __attribute__((packed)) {
  MOUSE_DOWN, MOUSE_UP, MOUSE_MOVED_X, MOUSE_MOVED_Y
} mouse_evt_type_t;

typedef enum __attribute__((packed)) {
  MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE
} mouse_btn_t;

typedef struct __attribute__((packed)) {
  mouse_evt_type_t type;
  uint8_t flags;
  int16_t data;
} mouse_evt_t;

void mouse_ps2_rcv();
bool mouse_read(mouse_evt_t* evt);

#endif