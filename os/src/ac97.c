#include <ac97.h>
#include <pci.h>

bool ac97_init() {
  if(AC97_DEV == 0) {
    return false;
  }

  return true;
}