#include <stdint.h>
#include <stdbool.h>
#include <2d.h>
#include <structs.h>
#include <res.h>
#include <keyboard.h>
#include <timer.h>
#include <snd.h>

const td_rect_t btn_top  = {255, 392, 290, 65};
const td_rect_t btn_bottom  = {255, 482, 290, 65};

#define BORDER_SIZE 4
#define MAIN_BORDER_COLOR 5
#define WIN_BORDER_COLOR 0x24
#define LOSE_BORDER_COLOR 0x77
#define WIN_TIMEOUT 30000
#define SLIDESHOW_TIMEOUT 3000

typedef enum {TOP, BOTTOM} mxt_selected_button_t;

void mxt_press_any_key(uint32_t timeout_ms) {
  timer_t expiry;
  kbd_event evt;


  timer_start(&expiry, timeout_ms);

  while(kbd_any_pressed()) {
    kbd_flush();

    if (timer_expired(&expiry)) {
      kbd_stuck();
      return;
    }

    snd_run();
  }

  while(!timer_expired(&expiry) && !kbd_read(&evt)) {
    snd_run();
  }
}

mxt_selected_button_t mxt_menu(td_image_t* background, td_color_t border_color) {
  td_set_background(background);
  td_draw_border_rect(&btn_top, border_color, BORDER_SIZE);
  mxt_selected_button_t selected = TOP;

  bool running = true;
  while(running) {
    kbd_event key;
    if(kbd_read(&key)) {
      if(KBD_IS_RELEASED(key)) {
        continue;
      }
      switch (KBD_SCANCODE(key)) {
        case KBD_KEY_UP:
          td_clear_border_rect(&btn_bottom, BORDER_SIZE);
          td_draw_border_rect(&btn_top, border_color, BORDER_SIZE);
          selected = TOP;
          break;
        case KBD_KEY_DOWN:
          td_clear_border_rect(&btn_top, BORDER_SIZE);
          td_draw_border_rect(&btn_bottom, border_color, BORDER_SIZE);
          selected = BOTTOM;
          break;
        case KBD_KEY_SPACE:
        case KBD_KEY_ENTER:
          running = false;
          break;
        default:
        break;
      }
    }

    snd_run();
  }

  return selected;
}

void mxt_main_menu(mxt_maxit_t* maxit) {
  if (mxt_menu(&res_mainmenu, MAIN_BORDER_COLOR) == TOP) {
    maxit->opponent.player_type = COMPUTER;
  } else {
    maxit->opponent.player_type = HUMAN;
  }

  maxit->game.level = 0;
  maxit->state = GAME;
}

void mxt_win_menu(mxt_maxit_t* maxit) {
  td_set_background(maxit->level_wins[maxit->game.level++]);
  mxt_press_any_key(WIN_TIMEOUT);

  if (maxit->game.level >= MAX_LEVEL) {
    maxit->state = CONGRATS;
  } else {
    if (mxt_menu(&res_winmenu, WIN_BORDER_COLOR) == TOP) {
      maxit->state = GAME;
    } else {
      maxit->state = MAIN_MENU;
    }
  }
}

void mxt_lose_menu(mxt_maxit_t* maxit) {
  if (mxt_menu(&res_losemenu, LOSE_BORDER_COLOR) == TOP) {
    maxit->state = GAME;
  } else {
    maxit->state = MAIN_MENU;
  }
}

void mxt_congrats(mxt_maxit_t* maxit) {
  // make congratulations image
  //td_set_background(&res_congratulations);
  //mxt_press_any_key(SLIDESHOW_TIMEOUT);

  // add credits on top
  for (int i = 0; i < MAX_LEVEL; i++) {
    td_set_background(maxit->level_wins[i]);
    mxt_press_any_key(SLIDESHOW_TIMEOUT);
  }

  maxit->state = MAIN_MENU;
}