#ifndef __ANI__
#define __ANI__

#include <stdint.h>
#include <stdbool.h>
#include <timer.h>
#include <2d.h>
#include <task.h>

typedef enum { HORIZONTAL, VERTICAL } ani_direction_t;

typedef struct {
  uint32_t frame_ms;
  int16_t step;
  td_rect_t rect;
  td_image_t *img;
  td_color_t color;
  ani_direction_t dir;
  timer_t _timer;
  td_rect_t _pos;
  uint32_t _dst;
} ani_transition_t;

task_desc_t ani_transition_start(ani_transition_t* ani);

#endif