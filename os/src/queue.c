/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <queue.h>

#define _queue_push_circular(queue, e, t)                                    \
  if (queue->count == queue->size) {                                         \
    return false;                                                            \
  } else {                                                                   \
    queue->count++;                                                          \
  }                                                                          \
                                                                             \
  ((t *) queue->buffer)[queue->write] = e;                                   \
  queue->write = (queue->write + 1) == queue->size ? 0 : (queue->write + 1); \
                                                                             \
  return true;

#define _queue_read_circular(queue, e, t)                                    \
  if (queue->count == 0) {                                                   \
    return false;                                                            \
  }                                                                          \
                                                                             \
  queue->count--;                                                            \
                                                                             \
  *e = ((t *) queue->buffer)[queue->read];                                   \
  queue->read = (queue->read + 1) == queue->size ? 0 : (queue->read + 1);    \
                                                                             \
  return true;

bool queue_push_circular_uint32(queue_t *queue, uint32_t e) {
  _queue_push_circular(queue, e, uint32_t)
}

bool queue_push_circular_uint16(queue_t *queue, uint16_t e) {
  _queue_push_circular(queue, e, uint16_t)
}

bool queue_push_circular_uint8(queue_t *queue, uint8_t e) {
  _queue_push_circular(queue, e, uint8_t)
}

bool queue_read_circular_uint32(queue_t *queue, uint32_t *e) {
  _queue_read_circular(queue, e, uint32_t);
}

bool queue_read_circular_uint16(queue_t *queue, uint16_t *e) {
  _queue_read_circular(queue, e, uint16_t);
}

bool queue_read_circular_uint8(queue_t *queue, uint8_t *e) {
  _queue_read_circular(queue, e, uint8_t);
}