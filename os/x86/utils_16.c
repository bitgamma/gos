#include "const.h"

void load_system() {
  uint32_t to_load = SYSTEM_SIZE;
  uint32_t* system = SYSTEM;

  LBA_TRANSFER->seg_buffer = DISK_READ_BUF_SEG;
  LBA_TRANSFER->addr_low = BOOTLOADER_SIZE;

  while(to_load) {
    LBA_TRANSFER->sector_count = to_load > DISK_BUF_SIZE ? DISK_BUF_SIZE : to_load;
    int13_load();
    LBA_TRANSFER->addr_low += LBA_TRANSFER->sector_count;
    to_load -= LBA_TRANSFER->sector_count;

    uint32_t *barrier = (uint32_t *)(DISK_READ_BUF_ADDR + (LBA_TRANSFER->sector_count * (512/4)));
    uint32_t *tmp = DISK_READ_BUF;
    while(tmp < barrier) {
      *(system++) = *(tmp++);
    }
  }
}
