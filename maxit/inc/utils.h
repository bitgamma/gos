#ifndef __UTILS__
#define __UTILS__

#include <stdint.h>
#include <stdbool.h>
#include <2d.h>
#include <kbd.h>

kbd_event mxt_press_any_key(uint32_t timeout_ms);
void mxt_display_slide(td_image_t* img, bool left);
void mxt_toggle_music();
void mxt_draw_text(td_image_t* text, uint32_t x, uint32_t y, uint32_t clear_ms);

#endif