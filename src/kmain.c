#include "kernel.h"

static Kctx kctx_static;

void kmain(void) {
  Kctx* kctx = &kctx_static;
  *kctx = (Kctx){0};

  kinit(kctx);
  kterm_init(kctx);
  klogf("xos boot kctx=%p t=%d", kctx, kctx->boot_time);

  kmem_init(kctx);

  void* p = kmem_page_alloc(kctx);
  klogf("nalloc=%d", kctx->kmem->nalloc);
  kmem_page_free(kctx, p);
  klogf("nalloc=%d", kctx->kmem->nalloc);

  klogs("halt");
  KSPIN();
}

void _putchar(char character) {
  flanterm_write(kctx_static.term.ctx, &character, 1);
}
