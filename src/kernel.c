// https://github.com/limine-bootloader/limine/blob/v8.x/PROTOCOL.md
#include "kernel.h"

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

// Set the base revision to 2, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".requests")))
static volatile struct limine_boot_time_request kbootreq_boottime = {
  .id = LIMINE_BOOT_TIME_REQUEST,
  .revision = 0,
};

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request kbootreq_fb = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_firmware_type_request kbootreq_firmware = {
  .id = LIMINE_FIRMWARE_TYPE_REQUEST,
  .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_stack_size_request kbootreq_ss = {
  .id = LIMINE_STACK_SIZE_REQUEST,
  .revision = 0,
  .stack_size = KERNEL_STACK_SZ,
};

__attribute__((used, section(".requests")))
static volatile struct limine_hhdm_request kbootreq_hhdm = {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 0,
};

__attribute__((used, section(".requests")))
static volatile struct limine_smp_request kbootreq_smp = {
  .id = LIMINE_SMP_REQUEST,
  .revision = 0,
#if defined(__x86_64__)
  .flags = 1,  // x2apic
#endif
};

__attribute__((used, section(".requests")))
static volatile struct limine_rsdp_request kbootreq_rsdp = {
  .id = LIMINE_RSDP_REQUEST,
  .revision = 0,
};

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request kbootreq_mmap = {
  .id = LIMINE_MEMMAP_REQUEST,
  .revision = 0,
};

__attribute__((used, section(".requests")))
static volatile struct limine_efi_system_table_request kbootreq_efisys = {
  .id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
  .revision = 0,
};

void kinit(Kctx* kctx) {
  KASSERT(LIMINE_BASE_REVISION_SUPPORTED);

  KASSERT(kbootreq_boottime.response != NULL);
  kctx->boot_time = kbootreq_boottime.response->boot_time;

  KASSERT(kbootreq_fb.response != NULL &&
          kbootreq_fb.response->framebuffer_count >= 1);
  kctx->fb = kbootreq_fb.response->framebuffers[0];

  KASSERT(kbootreq_firmware.response != NULL);
  KASSERT(kbootreq_firmware.response->firmware_type == LIMINE_FIRMWARE_TYPE_UEFI64);

  KASSERT(kbootreq_hhdm.response != NULL);
  kctx->hhdm_offset = kbootreq_hhdm.response->offset;

  KASSERT(kbootreq_smp.response != NULL);
  kctx->smp = kbootreq_smp.response;

  KASSERT(kbootreq_mmap.response != NULL);
  kctx->mmap = kbootreq_mmap.response;

  KASSERT(kbootreq_rsdp.response != NULL);
  kctx->rsdp = kbootreq_rsdp.response->address;

  KASSERT(kbootreq_efisys.response != NULL);
  KASSERT(kbootreq_efisys.response->address);
  kctx->efisys = kctx->hhdm_offset + kbootreq_efisys.response->address;

  kfb_paint(kctx, FB_GREEN);
}

void kfb_paint(Kctx* kctx, uint32_t color) {
  uint32_t* fb_base = (uint32_t*)kctx->fb->address;
  size_t w = kctx->fb->width;
  size_t h = kctx->fb->height;
  size_t stride = kctx->fb->pitch / 4;

  for (size_t y = 0; y < h; ++y) {
    for (size_t x = 0; x < w; ++x) {
      fb_base[y*stride+x] = color;
    }
  }
}
