#include "const.h"

int vbe_select_mode() {
  uint16_t *mode_list = (uint16_t *)LINEAR_ADDR(VBE_INFO->video_modes);
  uint16_t video_mode;

  while((video_mode = *mode_list++) != VBE_EOL) {
    vbe_get_mode_info(video_mode);

    if ((VBE_MODE_INFO->width == SCREEN_WIDTH) &&
      (VBE_MODE_INFO->height == SCREEN_HEIGHT) &&
      (VBE_MODE_INFO->bpp == SCREEN_BPP) &&
      ((VBE_MODE_INFO->attributes & VBE_FB_MASK) == VBE_FB_MASK)) {
      vbe_set_mode(video_mode);
      return 1;
    }
  }

  return 0;
}

int load_system() {
  return 1;
}
