/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <kbd.h>
#include <ps2.h>
#include <mem.h>
#include <port.h>
#include <queue.h>
#include <dbg.h>

#define KBD_PS2_EXTENDED0 0xe000
#define KBD_PS2_EXTENDED1 0xe100
#ifdef PS2_USE_SCANCODE2
#define KBD_PS2_RELEASED 0xf000
#else
#define KBD_PS2_RELEASED 0x80
#endif
#define KBD_MAX_KEYS 512
#define KBD_BUF_SIZE 128

static uint32_t _kbd_buf[KBD_BUF_SIZE];
static uint32_t _partial_scancode = 0;
static queue_t _input_queue = (queue_t) { 0, 0, KBD_BUF_SIZE, _kbd_buf};
static bitarray_t _kbd_state[BA_SIZE(KBD_MAX_KEYS)];

kbd_evt_t _kbd_ps2_remap(uint32_t scancode) {
  kbd_evt_t evt = 0;

  if ((scancode & KBD_PS2_RELEASED) == KBD_PS2_RELEASED) {
    evt = (1 << KBD_RELEASED);
#ifdef PS2_USE_SCANCODE2
    scancode = (scancode >> 16) | (scancode & 0xff);
#else
    scancode &= ~KBD_PS2_RELEASED;
#endif
  }

  if ((scancode & KBD_PS2_EXTENDED1) == KBD_PS2_EXTENDED1) {
    evt |= (scancode & 0x7f) | 0x100;
  } else if ((scancode & KBD_PS2_EXTENDED0) == KBD_PS2_EXTENDED0) {
    evt |= (scancode & 0x7f) | 0x80;
  } else {
    // assumes that the only scancode with bit 7 set is 0x83 (F7) which conflicts with the undefiined 0xE003
    evt |= (scancode & 0xff);
  }

  return evt;
}

void kbd_ps2_rcv() {
  _partial_scancode |= inb(PS2_DATA_PORT);

  if (_partial_scancode == PS2_DEV_ACK) {
    dbg_log_string("kbd: received unexpected ACK\n");
    _partial_scancode = 0;
#ifdef PS2_USE_SCANCODE2
  } else if ((_partial_scancode & 0xe0) == 0xe0) {
#else
  } else if (_partial_scancode == 0xe0 || _partial_scancode == 0xe1) {
#endif
    _partial_scancode <<= 8;
  } else {
    queue_push_circular_overwrite_uint32(&_input_queue, _partial_scancode);
    _partial_scancode = 0;
  }
}

bool kbd_read(kbd_evt_t *evt) {
  uint32_t scancode;
  if (!queue_read_circular_uint32(&_input_queue, &scancode)) {
    return false;
  }

  *evt = _kbd_ps2_remap(scancode);

  *evt |= (ba_get(_kbd_state, KBD_KEY_LALT) | ba_get(_kbd_state, KBD_KEY_RALT)) << KBD_ALT;
  *evt |= (ba_get(_kbd_state, KBD_KEY_LSHIFT) | ba_get(_kbd_state, KBD_KEY_RSHIFT)) << KBD_SHIFT;
  *evt |= (ba_get(_kbd_state, KBD_KEY_LCTRL) | ba_get(_kbd_state, KBD_KEY_RCTRL)) << KBD_CTRL;

  if (KBD_IS_RELEASED(*evt)) {
    ba_clear(_kbd_state, KBD_SCANCODE(*evt));
  } else {
    *evt |= ba_get(_kbd_state, KBD_SCANCODE(*evt)) << KBD_REPEAT;
    ba_set(_kbd_state, KBD_SCANCODE(*evt));
  }

  return true;
}

void kbd_flush() {
  kbd_evt_t e;
  while(kbd_read(&e));
}

bool kbd_any_pressed() {
  for (uint32_t i = 0; i < BA_SIZE(KBD_MAX_KEYS); i++) {
    if (_kbd_state[i]) {
      return true;
    }
  }

  return false;
}

void kbd_stuck() {
  memset32(_kbd_state, 0, BA_SIZE(KBD_MAX_KEYS));
}