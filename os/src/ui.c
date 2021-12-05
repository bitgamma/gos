/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ui.h>
#include <timer.h>
#include <vbe.h>

#define UI_MOUSE_MOV_TIMER 100
#define UI_MOUSE_MOV_THRESHOLD 14

static kbd_evt_t _key;
static mouse_evt_t _mouse;
static ui_cursor_t _cursor;

static timer_t _mov_timer_x;
static timer_t _mov_timer_y;

static bool _mouse_mov_to_arrow;

ui_cursor_t* ui_cursor() {
  return &_cursor;
}

kbd_evt_t ui_last_kbd_event() {
  return _key;
}

mouse_evt_t ui_last_mouse_event() {
  return _mouse;
}

void ui_set_convert_mouse_movement(bool b) {
  _mouse_mov_to_arrow = b;
  _cursor.x = (VBE_WIDTH >> 1);
  _cursor.y = (VBE_HEIGHT >> 1);
}

static inline int16_t ui_bound(int16_t d, int16_t max) {
  if (d < 0) {
    return 0;
  } else if (d > max) {
    return max;
  } else {
    return d;
  }
}

static ui_evt_t ui_handle_mouse_mov_x() {
  if (!(_mouse_mov_to_arrow && timer_expired(&_mov_timer_x))) {
    _cursor.x = ui_bound((_cursor.x + _mouse.data), VBE_WIDTH);
    return UI_NONE;
  }

  timer_start(&_mov_timer_x, UI_MOUSE_MOV_TIMER);

  ui_evt_t res;
  if (_cursor.x > ((VBE_WIDTH >> 1) + UI_MOUSE_MOV_THRESHOLD)) {
    res = UI_RIGHT;
  } else if (_cursor.x < ((VBE_WIDTH >> 1) - UI_MOUSE_MOV_THRESHOLD)) {
    res = UI_LEFT;
  } else {
    res = UI_NONE;
  }

  _cursor.x = (VBE_WIDTH >> 1);
  return res;
}

static ui_evt_t ui_handle_mouse_mov_y() {
  if (!(_mouse_mov_to_arrow && timer_expired(&_mov_timer_y))) {
    _cursor.y = ui_bound((_cursor.y - _mouse.data), VBE_HEIGHT);
    return UI_NONE;
  }

  timer_start(&_mov_timer_y, UI_MOUSE_MOV_TIMER);

  ui_evt_t res;
  if (_cursor.y > ((VBE_HEIGHT >> 1) + UI_MOUSE_MOV_THRESHOLD)) {
    res = UI_DOWN;
  } else if (_cursor.y < ((VBE_HEIGHT >> 1) - UI_MOUSE_MOV_THRESHOLD)) {
    res = UI_UP;
  } else {
    res = UI_NONE;
  }

  _cursor.y = (VBE_HEIGHT >> 1);
  return res;
}


ui_evt_t ui_read_event() {
  if (kbd_read(&_key)) {
    if (KBD_IS_RELEASED(_key)) {
      return UI_KEYUP;
    }

    switch (KBD_SCANCODE(_key)) {
      case KBD_KEY_UP:
      case KBD_KEY_W:
      case KBD_KEY_NUM_8:
        return UI_UP;
      case KBD_KEY_DOWN:
      case KBD_KEY_S:
      case KBD_KEY_NUM_2:
        return UI_DOWN;
      case KBD_KEY_LEFT:
      case KBD_KEY_A:
      case KBD_KEY_NUM_4:
        return UI_LEFT;
      case KBD_KEY_RIGHT:
      case KBD_KEY_D:
      case KBD_KEY_NUM_6:
        return UI_RIGHT;
      case KBD_KEY_ENTER:
      case KBD_KEY_SPACE:
      case KBD_KEY_NUM_ENTER:
        return UI_CONFIRM;
      case KBD_KEY_ESC:
      case KBD_KEY_BACKSPACE:
        return UI_CANCEL;
      case KBD_KEY_M:
        return UI_MUTE;
      default:
        return UI_KEYDOWN;
    }
  } else if (mouse_read(&_mouse)) {
    switch (_mouse.type) {
      case MOUSE_DOWN:
        switch(_mouse.data) {
          case MOUSE_LEFT:
            return UI_CONFIRM;
          case MOUSE_RIGHT:
            return UI_CANCEL;
          case MOUSE_MIDDLE:
            return UI_MUTE;
        }
      case MOUSE_UP:
        return UI_NONE;
      case MOUSE_MOVED_X:
        return ui_handle_mouse_mov_x();
      case MOUSE_MOVED_Y:
        return ui_handle_mouse_mov_y();
    }
  }

  return UI_NONE;
}

ui_evt_t ui_poll_event(uint32_t timeout_ms) {
  timer_t expiry;
  timer_start(&expiry, timeout_ms);

  while(kbd_any_pressed()) {
    kbd_flush();

    if (timer_expired(&expiry)) {
      kbd_stuck();
      return UI_NONE;
    }

    yield();
  }

  while(!timer_expired(&expiry)) {
    ui_evt_t evt;

    if ((evt = ui_read_event()) != UI_NONE) {
      return evt;
    }

    yield();
  }

  return UI_NONE;
}