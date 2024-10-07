#include <stdarg.h>

#include "kernel.h"

static inline void logprefix(Kctx* kctx, const char* fname, size_t line) {
  kterm_printf(kctx, "[%s:%d] ", fname, line);
}

void klog2(Kctx* kctx, Kstr s, const char* fname, size_t line) {
  logprefix(kctx, fname, line);
  kterm_write(kctx, s);
  kterm_write(kctx, kstr("\n"));
}

void klogf2(Kctx* kctx, const char* fname, size_t line, const char* fmt, ...) {
  logprefix(kctx, fname, line);

	va_list ap;
	va_start(ap, fmt);
  kterm_vprintf(kctx, fmt, ap);
	va_end(ap);

  kterm_write(kctx, kstr("\n"));
}
