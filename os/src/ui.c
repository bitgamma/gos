/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ui.h>
#include <timer.h>

static kbd_evt_t _key;

kbd_evt_t ui_last_kbd_event() {
  return _key;
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