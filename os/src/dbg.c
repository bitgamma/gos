/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <dbg.h>
#include <port.h>

#define DEBUG_PORT 0xe9

static const char hexlet[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static inline void _dbg_log_nibble(uint8_t byte) {
  _dbg_log_char(hexlet[byte]);
}

static inline void _dbg_log_hex(uint8_t byte) {
  _dbg_log_nibble(byte >> 4);
  _dbg_log_nibble(byte & 0x0f);
}

static inline void _dbg_log_hex_prefix() {
  _dbg_log_char('0');
  _dbg_log_char('x');
}

void _dbg_log_char(char ch) {
  outb(DEBUG_PORT, ch);
}

void _dbg_log_uint8(uint8_t i) {
  _dbg_log_hex_prefix();
  _dbg_log_hex(i);
  _dbg_log_char('\n');
}

void _dbg_log_uint16(uint16_t i) {
  _dbg_log_hex_prefix();
  _dbg_log_hex(i >> 8);
  _dbg_log_hex(i & 0xff);
  _dbg_log_char('\n');
}

void _dbg_log_uint32(uint32_t i) {
  _dbg_log_hex_prefix();
  _dbg_log_hex(i >> 24);
  _dbg_log_hex(i >> 16);
  _dbg_log_hex(i >> 8);
  _dbg_log_hex(i & 0xff);
  _dbg_log_char('\n');
}

void _dbg_log_string(char *str) {
  while(*str != '\0') {
    _dbg_log_char(*(str++));
  }
}