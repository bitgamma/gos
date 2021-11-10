#include <fmt_dro.h>
#include <opl3.h>
#include <dbg.h>

#define DBRA 0x41524244

bool fmt_dro_init(fmt_dro_context_t* ctx, void* data) {
  ctx->hdr = (fmt_dro_hdr_t *) data;
  ctx->codemap = (fmt_dro_codemap_t* )(data + sizeof(fmt_dro_hdr_t));
  ctx->data = (fmt_dro_cmd_t *)(&ctx->codemap[ctx->hdr->cm_len]);
  ctx->current_cmd = 0;
  timer_start(&ctx->timer, 0);

  if (ctx->hdr->signature[0] != DBRA || ctx->hdr->version_major != 2) {
    dbg_log_string("fmt_dro: unsupported file format\n");
    return false;
  }

  return true;
}

bool fmt_dro_run(fmt_dro_context_t* ctx) {
  bool finished = false;

  while (timer_expired(&ctx->timer)) {
    fmt_dro_cmd_t cmd = ctx->data[ctx->current_cmd++];

    if (FMT_DRO_REG(cmd) == ctx->hdr->short_delay_code) {
      timer_start(&ctx->timer, (FMT_DRO_DATA(cmd) + 1));
    } else if (FMT_DRO_REG(cmd) == ctx->hdr->long_delay_code) {
      timer_start(&ctx->timer, ((FMT_DRO_DATA(cmd) + 1) << 8));
    } else {
      uint16_t reg = ctx->codemap[FMT_DRO_REG(cmd)];
      reg |= FMT_DRO_CHIPSEL(cmd);
      opl3_write(reg, FMT_DRO_DATA(cmd));
    }

    if (ctx->current_cmd == ctx->hdr->length_pairs) {
      if (ctx->loop) {
        ctx->current_cmd = 0;
      }

      finished = true;
    }
  }
  return finished;
}