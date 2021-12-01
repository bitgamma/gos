/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <utils.h>
#include <timer.h>
#include <ani.h>
#include <res.h>
#include <task.h>
#include <snd.h>

#define SLIDE_FRAME_MS 10
#define SLIDE_STEP 5

static task_desc_t _music_desc = TDESC_ERR;

void mxt_display_slide(td_image_t* img, bool to_left) {
  ani_transition_t ani;

  memset(&ani, 0, sizeof(ani_transition_t));
  ani.dir = HORIZONTAL;
  ani.frame_ms = SLIDE_FRAME_MS;
  ani.step = to_left ? SLIDE_STEP : -SLIDE_STEP;
  ani.rect.width = res_mainmenu.width;
  ani.rect.height = res_mainmenu.height;

  task_wait(ani_transition_start(&ani));

  if (img) {
    ani.img = img;
    ani.step = -ani.step;
    task_wait(ani_transition_start(&ani));
  }

}

void mxt_toggle_music() {
  if (_music_desc == TDESC_ERR) {
    _music_desc = snd_play(&res_music, true);
  } else {
    snd_stop(_music_desc);
    _music_desc = TDESC_ERR;
  }
}

void mxt_draw_text(td_image_t* text, uint32_t x, uint32_t y, uint32_t clear_ms) {
  td_rect_t rect = {x, y, text->width, text->height};
  td_draw_sprite(&rect, text);

  if (clear_ms) {
    sleep(clear_ms);
    td_clear_rect(&rect);
  }
}