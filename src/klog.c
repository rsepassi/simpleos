#include <stdarg.h>

#include "kernel.h"
#include "printf.h"

static uint8_t logbuf[KERNEL_LOG_BUF_SZ];

static inline void logprefix(Kctx* kctx, char* fname, size_t line) {
  kterm_write(kctx, kstr("["));
  kterm_write(kctx, kstr(fname));
  kterm_write(kctx, kstr(":"));
  int len = snprintf((char*)logbuf, KERNEL_LOG_BUF_SZ, "%d", line);
  kterm_write(kctx, (Kstr){logbuf, len});
  kterm_write(kctx, kstr("] "));
}

void klog_init(Kctx* kctx) {}

void klog2(Kctx* kctx, Kstr s, char* fname, size_t line) {
  logprefix(kctx, fname, line);
  kterm_write(kctx, s);
  kterm_write(kctx, kstr("\n"));
}

void klogf2(Kctx* kctx, char* fname, size_t line, char* fmt, ...) {
  logprefix(kctx, fname, line);

	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf((char*)logbuf, KERNEL_LOG_BUF_SZ, fmt, ap);
	va_end(ap);
  kterm_write(kctx, (Kstr){logbuf, len});
  kterm_write(kctx, kstr("\n"));
}
