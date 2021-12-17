/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <2d.h>
#include <res.h>
#include <structs.h>
#include <menu.h>
#include <game.h>
#include <utils.h>

static mxt_maxit_t maxit;

void run(mxt_maxit_t* maxit) {
  for(;;) {
    switch(maxit->state) {
      case MAIN_MENU:
        mxt_main_menu(maxit);
        break;
      case DIFFICULTY_MENU:
        mxt_difficulty_menu(maxit);
        break;
      case GAME:
        mxt_game(maxit);
        break;
      case WIN_MENU:
        mxt_win_menu(maxit);
        break;
      case LOSE_MENU:
        mxt_lose_menu(maxit);
        break;
      case CONGRATS:
        mxt_congrats(maxit);
        break;
      default:
        break;
    }
  }
}

void app_main() {
  res_init();

  maxit.state = MAIN_MENU;
  maxit.board_positive[0] = &res_positive_01;
  maxit.board_positive[1] = &res_positive_02;
  maxit.board_positive[2] = &res_positive_03;
  maxit.board_positive[3] = &res_positive_04;
  maxit.board_positive[4] = &res_positive_05;
  maxit.board_positive[5] = &res_positive_06;
  maxit.board_positive[6] = &res_positive_07;
  maxit.board_positive[7] = &res_positive_08;
  maxit.board_positive[8] = &res_positive_09;
  maxit.board_positive[9] = &res_positive_10;
  maxit.board_positive[10] = &res_positive_11;
  maxit.board_negative[0] = &res_negative_11;
  maxit.board_negative[1] = &res_negative_10;
  maxit.board_negative[2] = &res_negative_09;
  maxit.board_negative[3] = &res_negative_08;
  maxit.board_negative[4] = &res_negative_07;
  maxit.board_negative[5] = &res_negative_06;
  maxit.board_negative[6] = &res_negative_05;
  maxit.board_negative[7] = &res_negative_04;
  maxit.board_negative[8] = &res_negative_03;
  maxit.board_negative[9] = &res_negative_02;
  maxit.board_negative[10] = &res_negative_01;
  maxit.score_digits[0] = &res_digits_00;
  maxit.score_digits[1] = &res_digits_01;
  maxit.score_digits[2] = &res_digits_02;
  maxit.score_digits[3] = &res_digits_03;
  maxit.score_digits[4] = &res_digits_04;
  maxit.score_digits[5] = &res_digits_05;
  maxit.score_digits[6] = &res_digits_06;
  maxit.score_digits[7] = &res_digits_07;
  maxit.score_digits[8] = &res_digits_08;
  maxit.score_digits[9] = &res_digits_09;

  maxit.level_bgs[0] = &res_level01;
  maxit.level_bgs[1] = &res_level02;
  maxit.level_bgs[2] = &res_level03;
  maxit.level_bgs[3] = &res_level04;
  maxit.level_bgs[4] = &res_level05;
#ifndef DEMO
  maxit.level_bgs[5] = &res_level06;
  maxit.level_bgs[6] = &res_level07;
  maxit.level_bgs[7] = &res_level08;
  maxit.level_bgs[8] = &res_level09;
  maxit.level_bgs[9] = &res_level10;
#endif

  maxit.level_wins[0] = &res_level01_win;
  maxit.level_wins[1] = &res_level02_win;
  maxit.level_wins[2] = &res_level03_win;
  maxit.level_wins[3] = &res_level04_win;
  maxit.level_wins[4] = &res_level05_win;
#ifndef DEMO
  maxit.level_wins[5] = &res_level06_win;
  maxit.level_wins[6] = &res_level07_win;
  maxit.level_wins[7] = &res_level08_win;
  maxit.level_wins[8] = &res_level09_win;
  maxit.level_wins[9] = &res_level10_win;
#endif

  mxt_toggle_music();
  maxit.serial = serial_open(SERIAL_COM1, BAUD_19200);

  run(&maxit);
}