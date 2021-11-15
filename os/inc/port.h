#ifndef __PORT__
#define __PORT__

#include <stdint.h>

#define POST_PORT 0x80

inline void outb(uint16_t port, uint8_t val) {
  asm volatile ("outb %0, %1" : : "Nd"(port), "a"(val));
}

inline void outw(uint16_t port, uint16_t val) {
  asm volatile ("outw %0, %1" : : "Nd"(port), "a"(val));
}

inline void outd(uint16_t port, uint32_t val) {
  asm volatile ("outd %0, %1" : :"Nd"(port), "a"(val));
}

inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile ("inb %0, %1" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline uint16_t inw(uint16_t port) {
  uint16_t ret;
  asm volatile ("inw %0, %1" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline uint32_t ind(uint16_t port) {
  uint32_t ret;
  asm volatile ("ind %0, %1" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline void iowait(void) {
  outb(POST_PORT, 0);
}

#endif