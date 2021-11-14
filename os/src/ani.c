#include <ani.h>

#define abs(a) (a >= 0 ? a : -a)

bool ani_transition_frame(ani_transition_t* ani) {
  bool finished = false;

  if (!timer_expired(&ani->_timer)) {
    return finished;
  }

  if (ani->img) {
    td_image_t cut;
    td_img_cut(&ani->_pos, ani->img, &cut);
    td_draw_rect(&ani->_pos, &cut);
  } else {
    td_draw_solid_rect(&ani->_pos, ani->color);
  }

  if (ani->dir == VERTICAL) {
    ani->_pos.y += ani->step;
    finished = ani->_pos.y == ani->_dst;
  } else {
    ani->_pos.x += ani->step;
    finished = ani->_pos.x == ani->_dst;
  }

  if (!finished) {
    timer_start(&ani->_timer, ani->frame_ms);
  }

  return finished;
}

task_desc_t ani_transition_start(ani_transition_t* ani) {
  if (ani->dir == VERTICAL) {
    ani->_pos.width = ani->rect.width;
    ani->_pos.height = abs(ani->step);
    ani->_pos.x = 0;

    if (ani->step < 0) {
      ani->_pos.y = ani->rect.height + ani->step;
      ani->_dst = ani->step;
    } else {
      ani->_pos.y = 0;
      ani->_dst = ani->rect.height;
    }
  } else {
    ani->_pos.width = abs(ani->step);
    ani->_pos.height = ani->rect.height;
    ani->_pos.y = 0;

    if (ani->step < 0) {
      ani->_pos.x = ani->rect.width + ani->step;
      ani->_dst = ani->step;
    } else {
      ani->_pos.x = 0;
      ani->_dst = ani->rect.width;
    }
  }

  timer_start(&ani->_timer, 0);
  return task_start((task_cb_t) ani_transition_frame, ani, false);
}

