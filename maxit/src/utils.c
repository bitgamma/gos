#include <utils.h>
#include <timer.h>
#include <kbd.h>
#include <ani.h>
#include <res.h>
#include <task.h>
#include <snd.h>

#define SLIDE_FRAME_MS 10
#define SLIDE_STEP 5

static task_desc_t _music_desc = TDESC_ERR;

void mxt_press_any_key(uint32_t timeout_ms) {
  timer_t expiry;
  kbd_event evt;

  timer_start(&expiry, timeout_ms);

  while(kbd_any_pressed()) {
    kbd_flush();

    if (timer_expired(&expiry)) {
      kbd_stuck();
      return;
    }

    yield();
  }

  while(!timer_expired(&expiry) && !kbd_read(&evt)) {
    yield();
  }
}

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