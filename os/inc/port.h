#ifndef __PORT__
#define __PORT__

#include <stdint.h>

inline void outb(uint16_t port, uint8_t val) {
  asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

inline void outw(uint16_t port, uint16_t val) {
  asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

inline void outd(uint16_t port, uint32_t val) {
  asm volatile ( "outd %0, %1" : : "a"(val), "Nd"(port) );
}

inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline uint16_t inw(uint16_t port) {
  uint16_t ret;
  asm volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline uint32_t ind(uint16_t port) {
  uint32_t ret;
  asm volatile ( "ind %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline void iowait(void) {
  outb(0x80, 0);
}

#endif