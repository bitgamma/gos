#ifndef __FMT_DRO__
#define __FMT_DRO__

#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

#define FMT_DRO_DATA(a) (a >> 8)
#define FMT_DRO_REG(a) (a & 0xff)

typedef struct __attribute__((packed)) {
  uint8_t signature[8];
  uint16_t version_major;
  uint16_t version_minor;
  uint32_t length_pairs;
  uint32_t length_ms;
  uint8_t hw_type;
  uint8_t format;
  uint8_t compression;
  uint8_t short_delay_code;
  uint8_t long_delay_code;
  uint8_t cm_len;
} fmt_dro_hdr_t;

typedef uint8_t fmt_dro_codemap_t;
typedef uint16_t fmt_dro_cmd_t;

typedef struct {
  fmt_dro_hdr_t* hdr;
  fmt_dro_codemap_t* codemap;
  fmt_dro_cmd_t* data;
  uint32_t current_cmd;
  bool loop;
  timer_t timer;
} fmt_dro_context_t;

bool fmt_dro_init(fmt_dro_context_t* ctx, void* data);
bool fmt_dro_run(fmt_dro_context_t* ctx);

#endif