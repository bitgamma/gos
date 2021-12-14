/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __STATS__
#define __STATS__

#include <stdint.h>
#include <serial.h>

typedef uint32_t stat_id_t;

typedef enum __attribute__((packed)) {
  STAT_NONE,
  STAT_DELTA,
  STAT_ABSOLUTE,
} stat_value_type_t;

void stats_send(serial_desc_t sd, stat_id_t id, stat_value_type_t value_type, int32_t value);
#endif