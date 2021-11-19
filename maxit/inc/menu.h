/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _MENU_H_
#define _MENU_H_

#include <structs.h>

void mxt_main_menu(mxt_maxit_t* maxit);
void mxt_difficulty_menu(mxt_maxit_t* maxit);
void mxt_win_menu(mxt_maxit_t* maxit);
void mxt_lose_menu(mxt_maxit_t* maxit);
void mxt_congrats(mxt_maxit_t* maxit);

#endif