/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __AI__
#define __AI__

#include <structs.h>

void mxt_ai_init(mxt_maxit_t* maxit);
uint8_t mxt_ai_move(mxt_maxit_t* maxit);

#endif