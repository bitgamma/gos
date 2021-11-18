#ifndef __UTILS__
#define __UTILS__

#include <stdint.h>
#include <stdbool.h>
#include <2d.h>

void mxt_press_any_key(uint32_t timeout_ms);
void mxt_display_slide(td_image_t* img, bool left);
void mxt_toggle_music();

#endif