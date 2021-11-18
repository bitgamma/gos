#include <stdint.h>
#include <stdbool.h>
#include <2d.h>
#include <menu.h>
#include <res.h>
#include <kbd.h>
#include <utils.h>
#include <timer.h>
#include <snd.h>

const td_rect_t btns[3] = {
  {255, 302, 290, 65},
  {255, 392, 290, 65},
  {255, 482, 290, 65}
};

#define BORDER_SIZE 4
#define MAIN_BORDER_COLOR 3
#define WIN_BORDER_COLOR 4
#define LOSE_BORDER_COLOR 0xb0
#define WIN_TIMEOUT 30000
#define SLIDESHOW_TIMEOUT 5000
#define CONGRATS_TIMEOUT (SLIDESHOW_TIMEOUT*2)

#define CONGRATS_X 34
#define CONGRATS_Y 470
#define CONGRATS_CLEAR_MS 2000

#define DEVELOPED_BY_X 621
#define DEVELOPED_BY_Y 354
#define NAME_X 632
#define BITGAMMA_Y 382
#define CHOPPU_Y 412
#define COPYRIGHT_X 22
#define COPYRIGHT_Y 572

typedef enum {TOP, MIDDLE, BOTTOM} mxt_selected_button_t;

mxt_selected_button_t mxt_menu(td_image_t* background, td_color_t border_color, bool has_top) {
  uint8_t top = has_top ? TOP : MIDDLE;
  mxt_selected_button_t selected = MIDDLE;
  td_draw_bg(background);
  td_draw_border_rect(&btns[selected], border_color, BORDER_SIZE);

  bool running = true;
  while(running) {
    kbd_event key;
    if(kbd_read(&key)) {
      if(KBD_IS_RELEASED(key)) {
        continue;
      }
      switch (KBD_SCANCODE(key)) {
        case KBD_KEY_UP:
          if (selected > top) {
            td_clear_border_rect(&btns[selected--], BORDER_SIZE);
            td_draw_border_rect(&btns[selected], border_color, BORDER_SIZE);
            snd_play(&res_sfx_cursor, false);
          }
          break;
        case KBD_KEY_DOWN:
          if (selected < BOTTOM) {
            td_clear_border_rect(&btns[selected++], BORDER_SIZE);
            td_draw_border_rect(&btns[selected], border_color, BORDER_SIZE);
            snd_play(&res_sfx_cursor, false);
          }
          break;
        case KBD_KEY_SPACE:
        case KBD_KEY_ENTER:
          snd_play(&res_sfx_select, false);
          running = false;
          break;
        case KBD_KEY_M:
          mxt_toggle_music();
          break;
        default:
        break;
      }
    }

    yield();
  }

  return selected;
}

void mxt_main_menu(mxt_maxit_t* maxit) {
  if (mxt_menu(&res_mainmenu, MAIN_BORDER_COLOR, false) == MIDDLE) {
    maxit->opponent.player_type = COMPUTER;
    maxit->state = DIFFICULTY_MENU;
  } else {
    maxit->opponent.player_type = HUMAN;
    maxit->state = GAME;
  }

  maxit->game.level = 0;
}

void mxt_difficulty_menu(mxt_maxit_t* maxit) {
  switch(mxt_menu(&res_ai_difficulty, MAIN_BORDER_COLOR, true)) {
    case TOP:
      maxit->game.ai_mode = EASY;
      break;
    case MIDDLE:
      maxit->game.ai_mode = NORMAL;
      break;
    case BOTTOM:
      maxit->game.ai_mode = HARD;
      break;
  }

  maxit->state = GAME;
}

void mxt_win_menu(mxt_maxit_t* maxit) {
  mxt_display_slide(maxit->level_wins[maxit->game.level++], false);
  mxt_press_any_key(WIN_TIMEOUT);
  mxt_display_slide(NULL, true);

  if (maxit->game.level >= MAX_LEVEL) {
    maxit->state = CONGRATS;
  } else {
    if (mxt_menu(&res_winmenu, WIN_BORDER_COLOR, false) == MIDDLE) {
      maxit->state = GAME;
    } else {
      maxit->state = MAIN_MENU;
    }
  }
}

void mxt_lose_menu(mxt_maxit_t* maxit) {
  if (mxt_menu(&res_losemenu, LOSE_BORDER_COLOR, false) == MIDDLE) {
    maxit->state = GAME;
  } else {
    maxit->state = MAIN_MENU;
  }
}

void mxt_congrats(mxt_maxit_t* maxit) {
  maxit->state = MAIN_MENU;

  mxt_display_slide(&res_congrats, true);
  td_set_bg(&res_congrats);
  mxt_draw_text(&res_congrats_text, CONGRATS_X, CONGRATS_Y, CONGRATS_CLEAR_MS);

  if (mxt_press_any_key(CONGRATS_TIMEOUT) == KBD_KEY_ESC) {
    return;
  };

  for (int i = 0; i < MAX_LEVEL; i++) {
    mxt_display_slide(maxit->level_wins[i], (i & 1));
    if (mxt_press_any_key(SLIDESHOW_TIMEOUT) == KBD_KEY_ESC) {
      return;
    }
  }

  mxt_draw_text(&res_developed_by, DEVELOPED_BY_X, DEVELOPED_BY_Y, 0);
  sleep(500);
  mxt_draw_text(&res_bitgamma, NAME_X, BITGAMMA_Y, 0);
  sleep(250);
  mxt_draw_text(&res_choppu, NAME_X, CHOPPU_Y, 0);
  sleep(500);
  mxt_draw_text(&res_copyright, COPYRIGHT_X, COPYRIGHT_Y, 0);
  mxt_press_any_key(SLIDESHOW_TIMEOUT);
  mxt_display_slide(NULL, (MAX_LEVEL & 1));
}