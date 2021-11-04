#include <2d.h>
#include <res.h>
#include <structs.h>
#include <menu.h>
#include <game.h>

void app_main() {
  mxt_maxit_t maxit;
  maxit.state = MAIN_MENU;
  res_init();
  for(;;) {
    switch(maxit.state) {
      case MAIN_MENU:
        mxt_main_menu(&maxit);
        break;
      case GAME:
        mxt_game(&maxit);
        break;
      case WIN_MENU:
        mxt_win_menu(&maxit);
        break;
      case LOSE_MENU:
        mxt_lose_menu(&maxit);
        break;
      case CONGRATS:
        mxt_congrats(&maxit);
        break;
      default:
        break;
    }
  }
}