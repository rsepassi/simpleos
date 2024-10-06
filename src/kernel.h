#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "limine.h"

#define KASSERT(cond) do { if (!(cond)) { for (;;) __asm__ ("hlt"); } } while(0)

typedef struct {
  struct limine_framebuffer* fb;
  struct limine_smp_response* smp;
  uint64_t hhdm_offset;
  struct limine_memmap_response* mmap;
  void* rsdp;
  void* efisys;
  int64_t boot_time;
} Kctx;

void kinit(Kctx*);

#define FB_RGB(r, g, b) (((r) << 16) + ((g) << 8) + (b))
void kfb_paint(Kctx*, uint32_t color);
