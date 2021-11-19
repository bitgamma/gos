/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <mem.h>

void memset(void* dst, uint8_t val, uint32_t size) {
  uint32_t rest = size & 0x03;
  uint32_t count = size >> 2;
  uint32_t val32 = (val << 24 | val << 16 | val << 8 | val);

  memset32((uint32_t *) dst, val32, count);
  dst += (count << 2);

  while(rest--) {
    *(((uint8_t *)dst++)) = val;
  }
}

void memcpy(void* dst, void* src, uint32_t size) {
  uint32_t rest = size & 0x03;
  uint32_t count = size >> 2;

  memcpy32((uint32_t *) dst, (uint32_t *)src, count);
  dst += (count << 2);
  src += (count << 2);

  while(rest--) {
    *(((uint8_t *)dst++)) = *(((uint8_t *)src++));
  }
}