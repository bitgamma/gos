/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <queue.h>

void queue_push_circular_overwrite_uint32(queue_t *queue, uint32_t e) {
  if (queue->write == queue->size) {
    queue->write = 0;
  }

  ((uint32_t *) queue->buffer)[queue->write++] = e;

  if (queue->write == queue->read) {
    queue->read++;
  }
}

bool queue_read_circular_uint32(queue_t *queue, uint32_t *e) {
  if (queue->read == queue->write) {
    return false;
  } else if (queue->read >= queue->size) {
    queue->read = 0;
  }

  *e = ((uint32_t *) queue->buffer)[queue->read++];
  return true;
}