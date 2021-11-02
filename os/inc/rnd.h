#ifndef __RND__
#define __RND__

#include <stdint.h>

void rnd_seed(uint32_t s0, uint32_t s1);
uint32_t rnd_next(void);

#endif
