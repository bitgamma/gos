/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __SND__
#define __SND__

#include <stdint.h>
#include <stdbool.h>
#include <task.h>

typedef void snd_source_t;

void snd_init();
task_desc_t snd_play(snd_source_t* source, bool loop);
void snd_stop(task_desc_t snd);
void snd_pause(task_desc_t snd);

#endif