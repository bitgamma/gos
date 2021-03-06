/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __UI__
#define __UI__

#include <kbd.h>
#include <mouse.h>

typedef enum {
  UI_NONE, UI_KEYUP, UI_KEYDOWN, UI_UP, UI_DOWN, UI_RIGHT, UI_LEFT, UI_CONFIRM, UI_CANCEL, UI_MUTE
} ui_evt_t;

typedef struct {
  uint16_t x;
  uint16_t y;
} ui_cursor_t;

ui_evt_t ui_read_event();
ui_evt_t ui_poll_event(uint32_t timeout_ms);

kbd_evt_t ui_last_kbd_event();
mouse_evt_t ui_last_mouse_event();
ui_cursor_t* ui_cursor();

void ui_set_convert_mouse_movement(bool b);

#endif