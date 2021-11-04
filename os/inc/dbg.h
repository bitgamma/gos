#ifndef __DBG__
#define __DBG__

#include <stdint.h>

#ifdef DEBUG
#define dbg_log_char(a) _dbg_log_char(a)
#define dbg_log_uint8(a) _dbg_log_uint8(a)
#define dbg_log_uint16(a) _dbg_log_uint16(a)
#define dbg_log_uint32(a) _dbg_log_uint32(a)
#define dbg_log_string(a) _dbg_log_string(a)
#else
#define dbg_log_char(a)
#define dbg_log_uint8(a)
#define dbg_log_uint16(a)
#define dbg_log_uint32(a)
#define dbg_log_string(a)
#endif

void _dbg_log_char(char ch);
void _dbg_log_uint8(uint8_t i);
void _dbg_log_uint16(uint16_t i);
void _dbg_log_uint32(uint32_t i);
void _dbg_log_string(char *str);

#endif