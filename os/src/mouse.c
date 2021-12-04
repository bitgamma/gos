/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <mouse.h>
#include <ps2.h>

static uint8_t _packet_count = 3;
static uint32_t _partial_packet;

void mouse_ps2_rcv() {
  _partial_packet = (_partial_packet << 8) | inb(PS2_DATA_PORT);

  if (!--_packet_count) {
    _partial_packet = 0;
    _packet_count = 3;
  }
}