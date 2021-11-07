#include <ai.h>
#include <bitarray.h>
#include <mem.h>
#include <rnd.h>

// assume BOARD_SIZE <= 32
static bitarray_t _moves[BOARD_SIZE][1];

#define MINF INT16_MIN
#define PINF INT16_MAX

int16_t max(int16_t a, int16_t b) {
  return a > b ? a : b;
}

int16_t min(int16_t a, int16_t b) {
  return a < b ? a : b;
}

static inline bool coin() {
  return rnd_next() & 1;
}

void mxt_ai_init(mxt_maxit_t* maxit) {
  memset32((uint32_t *)_moves, 0xffffffff, BOARD_SIZE);
  ba_clear(_moves[maxit->game.board.cursor_row], maxit->game.board.cursor_column);
}

uint8_t mxt_ai_get_depth(mxt_maxit_t* maxit) {
  uint8_t scaler;

  switch(maxit->game.ai_mode) {
    case EASY:
      scaler = 5;
      break;
    case NORMAL:
      scaler = 4;
      break;
    case HARD:
      scaler = 3;
      break;
  }

  uint8_t base_depth = 1 + (maxit->game.level / scaler);
  uint8_t genius_factor = maxit->game.level % scaler;

  if ((genius_factor + (rnd_next() % scaler)) >= scaler) {
    return base_depth + 1;
  } else {
    return base_depth;
  }
}

int16_t mxt_ai_minmaxi(mxt_maxit_t* maxit, uint8_t row_col, int16_t score, uint8_t depth, int16_t alpha, int16_t beta, bool maximize, uint8_t* max_row) {
  if (depth == 0) {
    return score;
  }

  if (maximize) {
    int16_t val = MINF;

    for (int i = 0; i < BOARD_SIZE; i++) {
      if (!ba_get(_moves[i], row_col)) {
        continue;
      }

      ba_clear(_moves[i], row_col);
      int16_t t = mxt_ai_minmaxi(maxit, i, score + maxit->game.board.board[i][row_col], depth - 1, alpha, beta, false, SCRATCH_BUFFER);
      if ((t > val) || ((t == val) && coin())) {
        val = t;
        *max_row = i;
      }
      ba_set(_moves[i], row_col);
      alpha = max(alpha, val);
      if (val >= beta) {
        break;
      }
    }

    return val == MINF ? score : val;
  } else {
    int16_t val = PINF;

    for (int i = 0; i < BOARD_SIZE; i++) {
      if (!ba_get(_moves[row_col], i)) {
        continue;
      }
      ba_clear(_moves[row_col], i);
      val = min(val, mxt_ai_minmaxi(maxit, i, score - maxit->game.board.board[row_col][i], depth - 1, alpha, beta, true, SCRATCH_BUFFER));
      ba_set(_moves[row_col], i);
      beta = min(beta, val);
      if (val <= alpha) {
        break;
      }
    }

    return val == PINF ? score : val;
  }
}

void mxt_ai_move(mxt_maxit_t* maxit) {
  ba_clear(_moves[maxit->game.board.cursor_row], maxit->game.board.cursor_column);
  mxt_ai_minmaxi(maxit, maxit->game.board.cursor_column, 0, mxt_ai_get_depth(maxit), MINF, PINF, true, &maxit->game.board.cursor_row);
  ba_clear(_moves[maxit->game.board.cursor_row], maxit->game.board.cursor_column);
}