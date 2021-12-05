/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <mouse.h>
#include <ps2.h>
#include <queue.h>
#include <dbg.h>

#define MOUSE_LBTN 24
#define MOUSE_RBTN 25
#define MOUSE_MBTN 26
#define MOUSE_X_SIGN 28
#define MOUSE_Y_SIGN 29
#define MOUSE_X_OVER 30
#define MOUSE_Y_OVER 31
#define MOUSE_VALIDATION ((1 << MOUSE_X_OVER) | (1 << MOUSE_Y_OVER))

#define MOUSE_BUF_SIZE 64
#define MOUSE_PACKET_TIMER 2

static uint8_t _part_count;
static uint32_t _partial_packet;

static __attribute__((aligned(4))) uint32_t _mouse_buf[MOUSE_BUF_SIZE];
static queue_t _input_queue = (queue_t) { 0, 0, 0, MOUSE_BUF_SIZE, _mouse_buf};

typedef enum {
 FETCH, CHECK_LEFT, CHECK_MIDDLE, CHECK_RIGHT, CHECK_X, CHECK_Y
} mouse_read_t;

static mouse_read_t _read_state;
static uint32_t _current_packet;
static uint8_t _btn_state;

void mouse_ps2_rcv() {
  _partial_packet = (_partial_packet << 8) | inb(PS2_DATA_PORT);

  if ((_partial_packet == PS2_DEV_ACK) && (_part_count == 0)) {
    _partial_packet = 0;
    dbg_log_string("mouse: received unexpected ACK\n");
    return;
  }

  if (++_part_count == __ps2_mouse_packet_size) {
    if (__ps2_mouse_packet_size == 3) {
      _partial_packet = (_partial_packet << 8);
    }

    if ((_partial_packet & MOUSE_VALIDATION) == 0) {
      queue_push_circular_uint32(&_input_queue, _partial_packet);
    }

    _partial_packet = 0;
    _part_count = 0;
  }
}

__attribute__((always_inline)) static inline bool mouse_check_btn(mouse_evt_t* evt, uint8_t mask, mouse_btn_t btn) {
  bool down = (_current_packet & (1 << mask)) != 0;
  bool prev_down = (_btn_state & (1 << btn)) != 0;

  if (down && !prev_down) {
    evt->type = MOUSE_DOWN;
    _btn_state |= (1 << btn);
  } else if (!down && prev_down) {
    evt->type = MOUSE_UP;
    _btn_state &= ~(1 << btn);
  } else {
    return false;
  }

  evt->data = btn;
  return true;
}

__attribute__((always_inline)) static inline bool mouse_check_mov(mouse_evt_t* evt, uint8_t mask, uint8_t shift, mouse_evt_type_t type) {
  int16_t mov = (_current_packet >> shift) & 0xff;

  if (mov) {
    if (_current_packet & (1 << mask)) {
      evt->data = mov | 0xff00;
    } else {
      evt->data = mov;
    }

    evt->type = type;
    return true;
  }

  return false;
}

bool mouse_read(mouse_evt_t* evt) {
  switch (_read_state) {
  case FETCH:
    if (!queue_read_circular_uint32(&_input_queue, &_current_packet)) {
      return false;
    }
    _read_state = CHECK_LEFT;
    break;
  case CHECK_LEFT:
    _read_state = CHECK_RIGHT;
    if (mouse_check_btn(evt, MOUSE_LBTN, MOUSE_LEFT)) {
      return true;
    }
    break;
  case CHECK_RIGHT:
    _read_state = CHECK_MIDDLE;
    if (mouse_check_btn(evt, MOUSE_RBTN, MOUSE_RIGHT)) {
      return true;
    }
    break;
  case CHECK_MIDDLE:
    _read_state = CHECK_X;
    if (mouse_check_btn(evt, MOUSE_MBTN, MOUSE_MIDDLE)) {
      return true;
    }
    break;
  case CHECK_X:
    _read_state = CHECK_Y;
    if (mouse_check_mov(evt, MOUSE_X_SIGN, 16, MOUSE_MOVED_X)) {
      return true;
    }
    break;
  case CHECK_Y:
    _read_state = FETCH;
    if (mouse_check_mov(evt, MOUSE_Y_SIGN, 8, MOUSE_MOVED_Y)) {
      return true;
    }
    break;
  }

  return mouse_read(evt);
}
