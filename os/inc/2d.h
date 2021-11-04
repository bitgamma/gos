#ifndef __2D__
#define __2D__

#include <stdint.h>

typedef struct {
  uint32_t width;
  uint32_t height;
  void* data;
} td_image_t;

typedef uint8_t td_color_index_t;

typedef struct {
  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
} td_rect_t;

void td_set_background(td_image_t* bg);
void td_draw_rect(const td_rect_t* rect, td_image_t* img);
void td_draw_solid_rect(const td_rect_t* rect, td_color_index_t color);
void td_clear_rect(const td_rect_t* rect);
void td_draw_border_rect(const td_rect_t* rect, td_color_index_t color, uint32_t border_size);
void td_clear_border_rect(const td_rect_t* rect, uint32_t border_size);

#endif