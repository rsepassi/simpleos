#include "kernel.h"

static Kctx kctx_static;

void kmain(void) {
  Kctx* kctx = &kctx_static;

  kinit(kctx);
  kterm_init(kctx);
  klogs("xos initializing...");

  klog_init(kctx);
  kmem_init(kctx);

  klogs("halt");
  KASSERT(false);
}
