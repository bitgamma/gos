#ifndef __FMT_DRO__
#define __FMT_DRO__

#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

#define DBRA 0x41524244
#define FMT_DRO_DATA(a) (a >> 8)
#define FMT_DRO_REG(a) (a & 0x7f)
#define FMT_DRO_CHIPSEL(a) ((a & 0x80) << 1)

typedef enum __attribute__((packed)) {
  FMT_DRO_OPL2,
  FMT_DRO_DUAL_OPL2,
  FMT_DRO_OPL3
} fmt_dro_hw_t;

typedef struct __attribute__((packed)) {
  uint32_t signature[2];
  uint16_t version_major;
  uint16_t version_minor;
  uint32_t length_pairs;
  uint32_t length_ms;
  fmt_dro_hw_t hw_type;
  uint8_t format;
  uint8_t compression;
  uint8_t short_delay_code;
  uint8_t long_delay_code;
  uint8_t cm_len;
} fmt_dro_hdr_t;

typedef uint8_t fmt_dro_codemap_t;
typedef uint16_t fmt_dro_cmd_t;
typedef void (*opl3_write_t)(uint16_t, uint8_t);

typedef struct {
  void* data;
  fmt_dro_codemap_t* codemap;
  fmt_dro_cmd_t* cmds;
  uint32_t current_cmd;
  bool as_is;
  timer_t timer;
  opl3_write_t opl3_write;
} fmt_dro_context_t;

__attribute__((always_inline)) inline bool fmt_dro_detect(void* ctx) {
  fmt_dro_hdr_t* hdr = (fmt_dro_hdr_t*)((fmt_dro_context_t *)ctx)->data;
  return (hdr->signature[0] == DBRA) && (hdr->version_major == 2);
}

void fmt_dro_init(fmt_dro_context_t* ctx, opl3_write_t opl3_write, fmt_dro_hw_t hwtype);
bool fmt_dro_run(fmt_dro_context_t* ctx);

inline void fmt_dro_stop(fmt_dro_context_t* ctx) {
  ctx->timer = 0;
  ctx->current_cmd = 0;
}

#endif