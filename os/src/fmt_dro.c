#include <fmt_dro.h>
#include <dbg.h>

#define DBRA 0x41524244

bool fmt_dro_init(fmt_dro_context_t* ctx, opl3_write_t opl3_write, fmt_dro_hw_t hwtype) {
  fmt_dro_hdr_t* hdr = (fmt_dro_hdr_t *)ctx->data;

  ctx->codemap = (fmt_dro_codemap_t* )(ctx->data + sizeof(fmt_dro_hdr_t));
  ctx->cmds = (fmt_dro_cmd_t *)(&ctx->codemap[hdr->cm_len]);
  ctx->current_cmd = 0;
  ctx->as_is = (hwtype >= hdr->hw_type);
  ctx->opl3_write = opl3_write;

  timer_start(&ctx->timer, 0);

  if (hdr->signature[0] != DBRA || hdr->version_major != 2) {
    dbg_log_string("fmt_dro: unsupported file format\n");
    return false;
  }

  return true;
}

bool fmt_dro_run(fmt_dro_context_t* ctx) {
  bool finished = false;

  while (timer_expired(&ctx->timer)) {
    fmt_dro_cmd_t cmd = ctx->cmds[ctx->current_cmd++];
    fmt_dro_hdr_t* hdr = (fmt_dro_hdr_t *)ctx->data;

    if (FMT_DRO_REG(cmd) == hdr->short_delay_code) {
      timer_start(&ctx->timer, (FMT_DRO_DATA(cmd) + 1));
    } else if (FMT_DRO_REG(cmd) == hdr->long_delay_code) {
      timer_start(&ctx->timer, ((FMT_DRO_DATA(cmd) + 1) << 8));
    } else if (((cmd & 0x80) == 0) || ctx->as_is) {
      uint16_t reg = ctx->codemap[FMT_DRO_REG(cmd)];
      reg |= FMT_DRO_CHIPSEL(cmd);
      ctx->opl3_write(reg, FMT_DRO_DATA(cmd));
    }

    if (ctx->current_cmd == hdr->length_pairs) {
      ctx->current_cmd = 0;
      finished = true;
    }
  }

  return finished;
}