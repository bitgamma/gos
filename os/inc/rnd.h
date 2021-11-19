/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __RND__
#define __RND__

#include <stdint.h>

void rnd_seed(uint32_t s0, uint32_t s1);
uint32_t rnd_next(void);

#endif
