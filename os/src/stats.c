/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stats.h>

#define STAT_HEADER 0xa0

void stats_send(serial_desc_t sd, stat_id_t id, stat_value_type_t value_type, int32_t value) {
  if (sd == SERIAL_NODESC) {
    return;
  }

  serial_write(sd, STAT_HEADER);
  serial_write(sd, (id & 0xff));
  serial_write(sd, ((id >> 8) & 0xff));
  serial_write(sd, ((id >> 16) & 0xff));
  serial_write(sd, ((id >> 24) & 0xff));
  serial_write(sd, value_type);

  if (value_type != STAT_NONE) {
    serial_write(sd, (value & 0xff));
    serial_write(sd, ((value >> 8) & 0xff));
    serial_write(sd, ((value >> 16) & 0xff));
    serial_write(sd, ((value >> 24) & 0xff));
  }
}