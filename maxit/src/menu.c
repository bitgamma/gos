#include <stdint.h>
#include <stdbool.h>
#include <2d.h>
#include <structs.h>
#include <res.h>
#include <keyboard.h>

const td_rect_t btn_vscpu  = {255, 392, 290, 65};
const td_rect_t btn_vshuman  = {255, 482, 290, 65};

#define BORDER_SIZE 4
#define BORDER_COLOR 5

void mxt_main_menu(mxt_maxit_t* maxit) {
  mxt_player_type_t player_type = COMPUTER;
  td_set_background(&res_mainmenu);

  td_draw_border_rect(&btn_vscpu, BORDER_COLOR, BORDER_SIZE);
  
  while(maxit->state == MAIN_MENU) {
    kbd_event key;
    if(kbd_read(&key)) {
      if(KBD_IS_RELEASED(key)) {
        continue;
      }
      switch (KBD_SCANCODE(key)) {
        case KBD_KEY_UP:
          td_clear_border_rect(&btn_vshuman, BORDER_SIZE);
          td_draw_border_rect(&btn_vscpu, BORDER_COLOR, BORDER_SIZE);
          player_type = COMPUTER;
          break;
        case KBD_KEY_DOWN:
          td_clear_border_rect(&btn_vscpu, BORDER_SIZE);
          td_draw_border_rect(&btn_vshuman, BORDER_COLOR, BORDER_SIZE);
          player_type = HUMAN;
          break;
        case KBD_KEY_SPACE:
        case KBD_KEY_ENTER:
          maxit->opponent.player_type = player_type;
          maxit->state = GAME;
          break;
        default:
        break;
      }
    }
  }
}

void mxt_win_menu(mxt_maxit_t* maxit) {

}

void mxt_lose_menu(mxt_maxit_t* maxit) {

}

void mxt_congrats(mxt_maxit_t* maxit) {
  
}