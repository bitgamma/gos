#ifndef __2D__
#define __2D__

#include <stdint.h>

typedef void td_image_t;
typedef void td_rle_image_t;

typedef struct {
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
} td_rect_t;

void td_set_background(td_image_t *bg, uint32_t width, uint32_t height);
void td_draw_rect(td_rect_t* rect, td_image_t *img);
void td_draw_sprite(td_rect_t* rect, td_rle_image_t *sprite);
void td_clear_rect(td_rect_t* rect);

#endif