/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <queue.h>

bool queue_push_circular_uint32(queue_t *queue, uint32_t e) {
  if (queue->count == queue->size) {
    return false;
  } else {
    queue->count++;
  }

  ((uint32_t *) queue->buffer)[queue->write] = e;
  queue->write = (queue->write + 1) == queue->size ? 0 : (queue->write + 1);

  return true;
}

bool queue_read_circular_uint32(queue_t *queue, uint32_t *e) {
  if (queue->count == 0) {
    return false;
  }

  queue->count--;

  *e = ((uint32_t *) queue->buffer)[queue->read];
  queue->read = (queue->read + 1) == queue->size ? 0 : (queue->read + 1);

  return true;
}