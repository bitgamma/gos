/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __SB16__
#define __SB16__

#include <stdint.h>
#include <stdbool.h>
#include <config.h>

bool sb16_init();
void sb16_transfer_finished();
void sb16_transfer_start(uint32_t rate, bool mono);
void sb16_transfer_stop();

#endif