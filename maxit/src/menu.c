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

typedef enum {TOP, MIDDLE, BOTTOM} mxt_selected_button_t;

mxt_selected_button_t mxt_menu(td_image_t* background, td_color_t border_color, bool has_top) {
  uint8_t top = has_top ? TOP : MIDDLE;
  mxt_selected_button_t selected = MIDDLE;
  td_set_background(background);
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
  mxt_display_slide(&res_congrats, true);
  mxt_press_any_key(CONGRATS_TIMEOUT);

  for (int i = 0; i < MAX_LEVEL; i++) {
    mxt_display_slide(maxit->level_wins[i], (i & 1));
    mxt_press_any_key(SLIDESHOW_TIMEOUT);
  }

  mxt_display_slide(NULL, (MAX_LEVEL & 1));
  maxit->state = MAIN_MENU;
}