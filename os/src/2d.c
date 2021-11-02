#include <2d.h>
#include <vbe.h>
#include <mem.h>

static td_image_t *_bg;
static td_rect_t _screen_rect;
static td_rect_t _draw_rect;

static void _td_draw(td_image_t* img) {
  void* fb = VBE_FB + (_draw_rect.y * VBE_PITCH) + (_draw_rect.x * (VBE_BPP >> 3));
  uint32_t line_size = _draw_rect.width * (VBE_BPP >> 3);

  for (uint32_t i = 0; i < _draw_rect.height; i++) {
    memcpy(fb, img, line_size);
    fb += VBE_PITCH;
    img += line_size;
  }
}

void td_set_background(td_image_t *bg, uint32_t width, uint32_t height) {
  _screen_rect.x = ((VBE_WIDTH - width) >> 1);
  _screen_rect.y = ((VBE_HEIGHT - height) >> 1);
  _screen_rect.width = width;
  _screen_rect.height = height;
  _bg = bg;

  _draw_rect.x = _screen_rect.x;
  _draw_rect.y = _screen_rect.y;
  _draw_rect.width = _screen_rect.width;
  _draw_rect.height = _screen_rect.height;

  _td_draw(_bg);
}

void td_draw_rect(td_rect_t* rect, td_image_t* img) {
  _draw_rect.x = rect->x + _screen_rect.x;
  _draw_rect.y = rect->y + _screen_rect.y;
  _draw_rect.width = rect->width;
  _draw_rect.height = rect->height;

  _td_draw(img);
}

void td_clear_rect(td_rect_t* rect) {
  td_draw_rect(rect, _bg);
}