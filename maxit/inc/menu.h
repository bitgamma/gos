#ifndef _MENU_H_
#define _MENU_H_

#include <structs.h>

void mxt_press_any_key(uint32_t timeout_ms);

void mxt_main_menu(mxt_maxit_t* maxit);
void mxt_win_menu(mxt_maxit_t* maxit);
void mxt_lose_menu(mxt_maxit_t* maxit);
void mxt_congrats(mxt_maxit_t* maxit);

#endif