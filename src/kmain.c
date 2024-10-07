#include "kernel.h"

static Kctx kctx_static;

void kmain(void) {
  Kctx* kctx = &kctx_static;

  kinit(kctx);
  kterm_init(kctx);
  klogf("xos boot t=%d", kctx->boot_time);

  klog_init(kctx);
  kmem_init(kctx);

  klogs("halt");
  KASSERT(false);
}

void _putchar(char character) {
  flanterm_write(kctx_static.term.ctx, &character, 1);
}
