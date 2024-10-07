// Physical memory allocator

#include <stdint.h>

#include "kernel.h"

#define KPAGE_SIZE 4096

#define DIVCEIL(x, y) ((x) + (y) - 1) / (y)

// We'll keep 1 bit per page
struct Kmem_s {
  uint64_t npages;
  uint64_t metadata_npages;
  uint8_t* bitmap;
};

static inline void pagei_alloc(Kmem* kmem, size_t i) {
  KASSERT(i < kmem->npages);
  kmem->bitmap[i / 8] |= (1 << (i % 8));
}

void kmem_init(Kctx* kctx) {
  int metaidx = 0;
  size_t total_bytes = 0;
  size_t max_seglen = 0;

  for (int i = 0; i < kctx->mmap->entry_count; ++i) {
    struct limine_memmap_entry* entry = kctx->mmap->entries[i];
    if (entry->type != LIMINE_MEMMAP_USABLE) continue;
    if (entry->length > max_seglen) {
      metaidx = i;
      max_seglen = entry->length;
    }
    total_bytes += entry->length;
  }

  // We'll use the first n pages for the memory subsystem metadata
  size_t npages = total_bytes / KPAGE_SIZE;
  size_t bitmap_nbytes = DIVCEIL(npages, 8);
  size_t metadata_nbytes = sizeof(Kmem) + bitmap_nbytes;
  size_t metadata_npages = DIVCEIL(metadata_nbytes, KPAGE_SIZE);
  KASSERT(max_seglen >= metadata_nbytes);

  Kmem* kmem = (Kmem*)(
      kctx->hhdm_offset +
      kctx->mmap->entries[metaidx]->base);
  kmem->npages = npages;
  kmem->metadata_npages = metadata_npages;
  kmem->bitmap = (uint8_t*)&kmem->bitmap;
  memset(kmem->bitmap, 0, bitmap_nbytes);

  // TODO: reenable
  //for (size_t i = 0; i < metadata_npages; ++i) pagei_alloc(kmem, i);

  kctx->kmem = kmem;
}
