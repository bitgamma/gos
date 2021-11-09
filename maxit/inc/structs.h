#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdint.h>
#include <stdbool.h>
#include <2d.h>

#define BOARD_SIZE    8
#define DIGITS_COUNT  11
#define MAX_LEVEL 5

typedef enum {
	MAIN_MENU, GAME, WIN_MENU, LOSE_MENU, CONGRATS
} mxt_state_t;

typedef enum {
	HUMAN, COMPUTER
} mxt_player_type_t;

typedef enum {
	EASY, NORMAL, HARD
} mxt_ai_mode_t;

typedef enum {
	WIN, LOSE
} mxt_game_result_t;

typedef enum {
	PLAYER_1, PLAYER_2
} mxt_active_player_t;

typedef struct {
  int8_t board[BOARD_SIZE][BOARD_SIZE];
  mxt_active_player_t active_player;
  uint8_t cursor_column;
  uint8_t cursor_row;
} mxt_board_t;

typedef struct {
  int32_t score;
  uint32_t wins;
	mxt_player_type_t player_type;
} mxt_player_t;

typedef struct {
	uint8_t level;
  mxt_ai_mode_t ai_mode;
  mxt_board_t board;
  bool finished;
} mxt_game_t;

typedef struct {
  mxt_state_t state;
  mxt_game_t game;
  mxt_player_t player;
  mxt_player_t opponent;
  bool maxit_run;
  td_image_t* board_positive[DIGITS_COUNT];
  td_image_t* board_negative[DIGITS_COUNT];
  td_image_t* score_digits[10];
  td_image_t* level_bgs[MAX_LEVEL];
  td_image_t* level_wins[MAX_LEVEL + 1];
} mxt_maxit_t;

#endif