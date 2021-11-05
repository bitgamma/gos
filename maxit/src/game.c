#include <stdint.h>
#include <stdbool.h>
#include <2d.h>
#include <structs.h>
#include <res.h>
#include <rnd.h>
#include <keyboard.h>

#define PADDING 9
#define CURSOR_COLOR 0x2a
#define CURSOR_WIDTH  6
#define SCORE_X  652
#define SCORE_PY  370
#define SCORE_OY 505

static int8_t mxt_rnd_to_board(uint8_t rnd) {
  int8_t res = rnd % 22;
  res -= 11;

  return res == 0 ? 11 : res;
}

static void mxt_calc_position(td_rect_t* rect, uint8_t i, uint8_t j, uint8_t border_width) {
  rect->x = PADDING + ((res_positive_01.width + PADDING + 1) * j) - border_width;
  rect->y = PADDING + ((res_positive_01.height + PADDING + 1) * i) - border_width;
  rect->width = res_positive_01.width + (border_width * 2);
  rect->height = res_positive_01.height + (border_width * 2);
}

static void mxt_generate_cursor(mxt_maxit_t* maxit) {
  uint32_t rand = rnd_next();
  maxit->game.board.cursor_column = ((rand >> 8) & 0xff) % 8;
  maxit->game.board.cursor_row = (rand & 0xff) % 8;
  maxit->game.board.board[maxit->game.board.cursor_row][maxit->game.board.cursor_column] = 0;
}

static void mxt_draw_cursor(mxt_maxit_t* maxit, uint8_t old_row, uint8_t old_column) {
  td_rect_t cursor;
  mxt_calc_position(&cursor, old_row, old_column, CURSOR_WIDTH);
  td_clear_border_rect(&cursor, CURSOR_WIDTH);
  mxt_calc_position(&cursor, maxit->game.board.cursor_row, maxit->game.board.cursor_column, CURSOR_WIDTH);
  td_draw_border_rect(&cursor, CURSOR_COLOR, CURSOR_WIDTH);
}

static void mxt_game_init(mxt_maxit_t* maxit) {
  maxit->game.finished = false;
  maxit->player.score = 500;
  maxit->opponent.score = 500;
  for(int i = 0; i < BOARD_SIZE; i++){
    for(int j = 0; j < BOARD_SIZE; j+=4) {
      uint32_t rand = rnd_next();
      maxit->game.board.board[i][j] = mxt_rnd_to_board(rand & 0xff);
      maxit->game.board.board[i][j+1] = mxt_rnd_to_board((rand >> 8) & 0xff);
      maxit->game.board.board[i][j+2] = mxt_rnd_to_board((rand >> 16) & 0xff);
      maxit->game.board.board[i][j+3] = mxt_rnd_to_board((rand >> 24) & 0xff);
    }
  }

  mxt_generate_cursor(maxit);
}

static void mxt_draw_board(mxt_maxit_t* maxit) {
  td_rect_t cell = {PADDING, PADDING, res_positive_01.width, res_positive_01.height};
  for(int i = 0; i < BOARD_SIZE; i++){
    for(int j = 0; j < BOARD_SIZE; j++) {
      int8_t num = maxit->game.board.board[i][j];

      if (num == 0) {
        td_rect_t cursor = {cell.x - CURSOR_WIDTH, cell.y - CURSOR_WIDTH, cell.width + (CURSOR_WIDTH * 2), cell.height + (CURSOR_WIDTH * 2)};
        td_draw_border_rect(&cursor, CURSOR_COLOR, CURSOR_WIDTH);
      } else if(num < 0) {
        td_draw_rect(&cell, maxit->board_negative[num + 11]);
      } else {
        td_draw_rect(&cell, maxit->board_positive[num - 1]);
      }
      cell.x = cell.x + res_positive_01.width + PADDING + 1;
    }
    cell.x = PADDING;
    cell.y = cell.y + res_positive_01.height + PADDING + 1;
  }
}

static void mxt_draw_score_digit(mxt_maxit_t* maxit, int32_t score, td_rect_t* rect) {
  td_draw_rect(rect, maxit->score_digits[(score / 100) % 10]);
  rect->x = rect->x + res_digits_00.width;
  td_draw_rect(rect, maxit->score_digits[(score / 10) % 10]);
  rect->x = rect->x + res_digits_00.width;
  td_draw_rect(rect, maxit->score_digits[(score) % 10]);
  rect->x = rect->x + res_digits_00.width;
}

static void mxt_draw_score(mxt_maxit_t* maxit) {
  td_rect_t score_digit = {SCORE_X, SCORE_PY, res_digits_00.width, res_digits_00.height};
  mxt_draw_score_digit(maxit, maxit->player.score, &score_digit);
  score_digit.x = SCORE_X;
  score_digit.y = SCORE_OY;
  mxt_draw_score_digit(maxit, maxit->opponent.score, &score_digit);
}

static void mxt_left_pressed(mxt_maxit_t* maxit) {
  if(maxit->game.board.active_player != PLAYER_1) {
    return;
  }

  uint8_t old_column = maxit->game.board.cursor_column;

  bool moved = false;
  while(maxit->game.board.cursor_column != 0) {
    if (maxit->game.board.board[maxit->game.board.cursor_row][--maxit->game.board.cursor_column] != 0) {
      mxt_draw_cursor(maxit, maxit->game.board.cursor_row, old_column);
      moved = true;
      break;
    }
  }

  if (!moved) {
    maxit->game.board.cursor_column = old_column;
  }
}

static void mxt_right_pressed(mxt_maxit_t* maxit) {
  if(maxit->game.board.active_player != PLAYER_1) {
    return;
  }

  uint8_t old_column = maxit->game.board.cursor_column;

  bool moved = false;
  while(maxit->game.board.cursor_column != (BOARD_SIZE - 1)) {
    if (maxit->game.board.board[maxit->game.board.cursor_row][++maxit->game.board.cursor_column] != 0) {
      mxt_draw_cursor(maxit, maxit->game.board.cursor_row, old_column);
      moved = true;
      break;
    }
  }

  if (!moved) {
    maxit->game.board.cursor_column = old_column;
  }
}

static void mxt_up_pressed(mxt_maxit_t* maxit) {
  if(maxit->game.board.active_player != PLAYER_2) {
    return;
  }

  uint8_t old_row = maxit->game.board.cursor_row;

  bool moved = false;
  while(maxit->game.board.cursor_row != 0) {
    if (maxit->game.board.board[--maxit->game.board.cursor_row][maxit->game.board.cursor_column] != 0) {
      mxt_draw_cursor(maxit, old_row, maxit->game.board.cursor_column);
      moved = true;
      break;
    }
  }

  if (!moved) {
    maxit->game.board.cursor_row = old_row;
  }
}

static void mxt_down_pressed(mxt_maxit_t* maxit) {
  if(maxit->game.board.active_player != PLAYER_2) {
    return;
  }

  uint8_t old_row = maxit->game.board.cursor_row;

  bool moved = false;
  while(maxit->game.board.cursor_row != (BOARD_SIZE - 1)) {
    if (maxit->game.board.board[++maxit->game.board.cursor_row][maxit->game.board.cursor_column] != 0) {
      mxt_draw_cursor(maxit, old_row, maxit->game.board.cursor_column);
      moved = true;
      break;
    }
  }

  if (!moved) {
    maxit->game.board.cursor_row = old_row;
  }
}

static void mxt_enter_pressed(mxt_maxit_t* maxit) {
  if(maxit->game.board.board[maxit->game.board.cursor_row][maxit->game.board.cursor_column] == 0) {
    return;
  }

  maxit->game.finished = true;

  if(maxit->game.board.active_player == PLAYER_1) {
    maxit->player.score += maxit->game.board.board[maxit->game.board.cursor_row][maxit->game.board.cursor_column];
    for (uint8_t i = 0; i < BOARD_SIZE; i++) {
      if(maxit->game.board.board[i][maxit->game.board.cursor_column] != 0) {
        maxit->game.finished = false;
        break;
      }
    }
    maxit->game.board.active_player = PLAYER_2;
  } else {
    maxit->opponent.score += maxit->game.board.board[maxit->game.board.cursor_row][maxit->game.board.cursor_column];
    for (uint8_t j = 0; j < BOARD_SIZE; j++) {
      if(maxit->game.board.board[maxit->game.board.cursor_row][j] != 0) {
        maxit->game.finished = false;
        break;
      }
    }
    maxit->game.board.active_player = PLAYER_1;
  }

  maxit->game.board.board[maxit->game.board.cursor_row][maxit->game.board.cursor_column] = 0;

  td_rect_t cell;
  mxt_calc_position(&cell, maxit->game.board.cursor_row, maxit->game.board.cursor_column, 0);
  td_clear_rect(&cell);
  mxt_draw_score(maxit);
}

static void mxt_run_game(mxt_maxit_t* maxit) {
  kbd_event key;
  for(;;) {
    if(kbd_read(&key)) {
      if(KBD_IS_RELEASED(key)) {
        continue;
      }
      switch (KBD_SCANCODE(key)) {
        case KBD_KEY_LEFT:
          mxt_left_pressed(maxit);
          break;
        case KBD_KEY_RIGHT:
          mxt_right_pressed(maxit);
          break;
        case KBD_KEY_UP:
          mxt_up_pressed(maxit);
          break;
        case KBD_KEY_DOWN:
          mxt_down_pressed(maxit);
          break;
        case KBD_KEY_SPACE:
        case KBD_KEY_ENTER:
          mxt_enter_pressed(maxit);
          break;
        default:
        break;
      }
    }
  }
}

void mxt_game(mxt_maxit_t* maxit) {
  td_set_background(maxit->level_bgs[maxit->game.level]);
  mxt_game_init(maxit);
  mxt_draw_board(maxit);
  mxt_draw_score(maxit);
  mxt_run_game(maxit);
}