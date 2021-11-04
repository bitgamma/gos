#include <2d.h>
#include <vbe.h>
#include <mem.h>

typedef struct {
  td_image_t bg;
  uint32_t x_off;
  uint32_t y_off;
} td_context_t;

static td_context_t _ctx;

inline static void* _td_fb_at(const td_rect_t* rect) {
  return VBE_FB + ((rect->y + _ctx.y_off) * VBE_PITCH) + ((rect->x + _ctx.x_off) * VBE_PIXELWIDTH);
}

inline static uint32_t _td_line_size(const td_rect_t* rect) {
  return rect->width * VBE_PIXELWIDTH;
}

void td_set_background(td_image_t* bg) {
  _ctx.bg.width = bg->width;
  _ctx.bg.height = bg->height;
  _ctx.bg.data = bg->data;

  _ctx.x_off = ((VBE_WIDTH - _ctx.bg.width) >> 1);
  _ctx.y_off = ((VBE_HEIGHT - _ctx.bg.height) >> 1);
  td_rect_t bg_rect = (td_rect_t) {0, 0, _ctx.bg.width, _ctx.bg.height};

  td_draw_rect(&bg_rect, bg);
}

void td_draw_rect(const td_rect_t* rect, td_image_t* img) {
  void* fb = _td_fb_at(rect);
  void* data = img->data;
  uint32_t line_size = _td_line_size(rect);
  uint32_t skip_size = img->width * VBE_PIXELWIDTH;

  for (uint32_t i = 0; i < rect->height; i++) {
    memcpy(fb, data, line_size);
    fb += VBE_PITCH;
    data += skip_size;
  }
}

void td_draw_solid_rect(const td_rect_t* rect, td_color_index_t color) {
  void* fb = _td_fb_at(rect);
  uint32_t line_size = _td_line_size(rect);

  for (uint32_t i = 0; i < rect->height; i++) {
    memset(fb, color, line_size);
    fb += VBE_PITCH;
  }
}

void td_clear_rect(const td_rect_t* rect) {
  void* data = _ctx.bg.data;
  _ctx.bg.data += (rect->y * (_ctx.bg.width * VBE_PIXELWIDTH)) + (rect->x * VBE_PIXELWIDTH);
  td_draw_rect(rect, &_ctx.bg);
  _ctx.bg.data = data;
}

void td_draw_border_rect(const td_rect_t* rect, td_color_index_t color, uint32_t border_size) {
  td_rect_t horizontal_bar = (td_rect_t) {rect->x, rect->y, rect->width, border_size};
  td_draw_solid_rect(&horizontal_bar, color);
  horizontal_bar.y += (rect->height - border_size);
  td_draw_solid_rect(&horizontal_bar, color);

  td_rect_t vertical_bar = (td_rect_t) {rect->x, (rect->y + border_size), border_size, (rect->height - border_size)};
  td_draw_solid_rect(&vertical_bar, color);
  vertical_bar.x += (rect->width - border_size);
  td_draw_solid_rect(&vertical_bar, color);
}

void td_clear_border_rect(const td_rect_t* rect, uint32_t border_size) {
  td_rect_t horizontal_bar = (td_rect_t) {rect->x, rect->y, rect->width, border_size};
  td_clear_rect(&horizontal_bar);
  horizontal_bar.y += (rect->height - border_size);
  td_clear_rect(&horizontal_bar);

  td_rect_t vertical_bar = (td_rect_t) {rect->x, (rect->y + border_size), border_size, (rect->height - border_size)};
  td_clear_rect(&vertical_bar);
  vertical_bar.x += (rect->width - border_size);
  td_clear_rect(&vertical_bar);
}