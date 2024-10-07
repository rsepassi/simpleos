#include "kernel.h"

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
  flanterm_write(kctx->term.ctx, s.bytes, s.len);
}
