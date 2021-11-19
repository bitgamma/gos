/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __TASK__
#define __TASK__

#include <stdint.h>
#include <stdbool.h>

#define TDESC_ERR -1
#define TASK_QUEUE_SIZE 16

typedef int16_t task_desc_t;
typedef bool (*task_cb_t)(void*);

task_desc_t task_start(task_cb_t cb, void* ctx, bool repeat);
void* task_get_context(task_desc_t desc);
void task_stop(task_desc_t desc);
void task_wait(task_desc_t desc);
void task_run();

#endif