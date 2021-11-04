#ifndef __BITARRAY__
#define __BITARRAY__

#include <stdint.h>

#define BIT_IS_SET(a, b) ((a & (1 << b)) == (1 << b))

#define BA_SIZE(x) (x >> 5)
#define BA_I(x) BA_SIZE(x)
#define BA_J(x) (x & 0x1f)

typedef uint32_t bitarray_t;
typedef uint8_t bit_t;

__attribute__ ((always_inline)) inline bit_t ba_get(bitarray_t *a, uint32_t i) {
  return (a[BA_I(i)] & (1 << BA_J(i))) >> BA_J(i);
}

__attribute__ ((always_inline)) inline void ba_set(bitarray_t *a, uint32_t i) {
  a[BA_I(i)] |= (1 << BA_J(i));
}

__attribute__ ((always_inline)) inline void ba_clear(bitarray_t *a, uint32_t i) {
  a[BA_I(i)] &= ~(1 << BA_J(i));
}

#endif