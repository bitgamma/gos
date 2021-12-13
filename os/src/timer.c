/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <timer.h>
#include <pit.h>
#include <task.h>
#include <snd.h>
#include <serial.h>

void systick_init() {
  SYSTICK = 0;
  pit_init();
}

void yield() {
  task_run();
  snd_run();
  serial_run();
}

void sleep(uint32_t ms) {
  timer_t timer;
  timer_start(&timer, ms);

  while(!timer_expired(&timer)) {
    yield();
  }
}