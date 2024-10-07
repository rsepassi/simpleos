#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "stdmem.h"

#include "limine.h"

#include "flanterm.h"
#include "flanterm_backends/fb.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define KSPIN() do { for (;;) __asm__ ("hlt"); } while(0)
#define KASSERT(cond) do { if (!(cond)) KSPIN(); } while(0)
#define KCHECK(cond, fmt, ...) do { if (!(cond)) { klogf(fmt, ##__VA_ARGS__); KSPIN(); } } while(0)

#define KERNEL_STACK_SZ (2 << 20)
#define KERNEL_LOG_BUF_SZ (2 << 10)

typedef struct Kmem_s Kmem;

typedef struct {
  uint8_t* bytes;
  uint64_t len;
} Kstr;
#define kstr(s) ((Kstr){.bytes = (uint8_t*)s, .len = strlen(s)})

typedef struct {
  struct flanterm_context* ctx;
} Kterm;

typedef struct {
  // Boot context
  int64_t boot_time;
  struct limine_framebuffer* fb;
  struct limine_smp_response* smp;
  uint64_t hhdm_offset;
  struct limine_memmap_response* mmap;
  void* rsdp;
  void* efisys;

  // Subsystems
  Kterm term;
  Kmem* kmem;
} Kctx;

void kinit(Kctx*);

void kterm_init(Kctx*);
void kterm_write(Kctx* kctx, Kstr s);
void kterm_printf(Kctx* kctx, char* fmt, ...);
void kterm_vprintf(Kctx* kctx, char* fmt, va_list);

#define klog(s) klog2(kctx, s, __FILENAME__, __LINE__)
#define klogs(s) klog(kstr(s))
#define klogf(fmt, ...) klogf2(kctx, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
void klog2(Kctx*, Kstr s, const char* fname, size_t line);
void klogf2(Kctx* kctx, const char* fname, size_t line, const char* fmt, ...);

void kmem_init(Kctx*);

#define FB_RGB(r, g, b) (((r) << kctx->fb->red_mask_shift) + \
    ((g) << kctx->fb->green_mask_shift) + \
    ((b) << kctx->fb->blue_mask_shift))
#define FB_RED FB_RGB(255, 0, 0)
#define FB_BLUE FB_RGB(0, 0, 255)
#define FB_GREEN FB_RGB(0, 255, 0)
void kfb_paint(Kctx*, uint32_t color);
