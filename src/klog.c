#include "kernel.h"

void klog_init(Kctx* kctx) {}

void klog2(Kctx* kctx, Kstr s, char* fname, size_t line) {
  kterm_write(kctx, kstr("["));
  kterm_write(kctx, kstr(fname));
  kterm_write(kctx, kstr("] "));
  kterm_write(kctx, s);
  kterm_write(kctx, kstr("\n"));
}
