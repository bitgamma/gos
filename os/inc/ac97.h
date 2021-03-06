/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __AC97__
#define __AC97__

#include <stdint.h>
#include <stdbool.h>

#define AC97_BUFFER_SIZE 512

bool ac97_init();
uint16_t* ac97_next_buffer();
void ac97_play();
void ac97_stop();

#endif