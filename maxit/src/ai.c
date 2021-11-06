#include <ai.h>

void mxt_ai_move(mxt_maxit_t* maxit) {
  int8_t max_row_val = -12;
  uint8_t max_row = 0;

  for (uint8_t i = 0; i < BOARD_SIZE; i++) {
    int8_t tmp = maxit->game.board.board[i][maxit->game.board.cursor_column];
    if ((tmp > max_row_val) && tmp != 0) {
      max_row_val = tmp;
      max_row = i;
    }
  }

  maxit->game.board.cursor_row = max_row;
}