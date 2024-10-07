#include <stdarg.h>

#include "kernel.h"
#include "printf.h"

static uint8_t logbuf2[KERNEL_LOG_BUF_SZ];

void kterm_init(Kctx* kctx) {
  Kterm* term = &kctx->term;
  term->ctx = flanterm_fb_init(
      NULL, NULL,  // malloc + free
      kctx->fb->address, kctx->fb->width, kctx->fb->height, kctx->fb->pitch,
      8, 16, // r
      8,  8, // g
      8,  0, // b
      0, // canvas
      0, // ansi_colours
      0, // ansi_bright_colours
      0, // default_bg
      0, // default_fg
      0, // default_bg_bright
      0, // default_fg_bright
      0, // font
      8, // font_width
      16, // font_height
      1, // font_spacing
      0, // font_scale_x
      0, // font_scale_y
      0 // margin
    );
  KASSERT(term->ctx);
}

void kterm_write(Kctx* kctx, Kstr s) {
  flanterm_write(kctx->term.ctx, (const char*)s.bytes, s.len);
}

void kterm_vprintf(Kctx* kctx, char* fmt, va_list ap) {
  int len = vsnprintf((char*)logbuf2, KERNEL_LOG_BUF_SZ, fmt, ap);
  kterm_write(kctx, (Kstr){logbuf2, len});
}

void kterm_printf(Kctx* kctx, char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  kterm_vprintf(kctx, fmt, ap);
  va_end(ap);
}
