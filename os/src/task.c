/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <task.h>
#include <mem.h>
#include <timer.h>
typedef struct {
  bool running;
  void* ctx;
  task_cb_t cb;
  bool repeat;
} task_t;

static task_t _tasks[TASK_QUEUE_SIZE];

task_desc_t task_start(task_cb_t cb, void* ctx, bool repeat) {
  uint8_t desc;

  for(desc = 0; desc < TASK_QUEUE_SIZE; desc++) {
    if (!_tasks[desc].running) {
      break;
    }
  }

  if (desc == TASK_QUEUE_SIZE) {
    return TDESC_ERR;
  }

  _tasks[desc].running = true;
  _tasks[desc].cb = cb;
  _tasks[desc].ctx = ctx;
  _tasks[desc].repeat = repeat;

  return desc;
}

void task_run() {
  for(uint8_t i = 0; i < TASK_QUEUE_SIZE; i++) {
    if (_tasks[i].running) {
      if(_tasks[i].cb(_tasks[i].ctx) && !_tasks[i].repeat) {
        _tasks[i].running = false;
        _tasks[i].ctx = NULL;
      }
    }
  }
}

void* task_get_context(task_desc_t desc) {
  return _tasks[desc].ctx;
}

void task_stop(task_desc_t desc) {
  _tasks[desc].running = false;
  _tasks[desc].ctx = NULL;
}

void task_wait(task_desc_t desc) {
  while(_tasks[desc].running) {
    yield();
  }
}
