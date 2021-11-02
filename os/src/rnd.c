#include <stdint.h>

static inline uint32_t rotl(uint32_t x, uint8_t k) {
  asm ("rold %0, %1" : "+r" (x) : "i" (k));
  return x;
}

static uint32_t s[2];

void rnd_seed(uint32_t s0, uint32_t s1) {
  s[0] = s0;
  s[1] = s1;
}

uint32_t rnd_next(void) {
	const uint32_t s0 = s[0];
	uint32_t s1 = s[1];
	const uint32_t result = s0 * 0x9E3779BB;

	s1 ^= s0;
	s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
	s[1] = rotl(s1, 13); // c

	return result;
}