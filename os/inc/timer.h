/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __TIMER__
#define __TIMER__

#include <stdint.h>
#include <stdbool.h>
#include <mem.h>
#include <task.h>

#ifndef TIMER_RES_MS
#define TIMER_RES_MS 1
#endif

typedef uint32_t timer_t;

inline void timer_start(timer_t *timer, uint32_t ms) {
  *timer = SYSTICK + ms;
}

inline bool timer_expired(timer_t *timer) {
  return SYSTICK >= *timer;
}

inline void yield() {
  task_run();
}

void systick_init();
void sleep(uint32_t ms);

#endif