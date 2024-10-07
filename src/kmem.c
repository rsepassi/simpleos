// Physical memory allocator

#include "kernel.h"

#define KPAGE_SZ (4 << 10)

#define DIVCEIL(x, y) ((x) + (y) - 1) / (y)

#define page_inuse(i) ((kmem->bitmap[(i) / 8] & (1 << ((i) % 8))) > 0)

static inline void pagei_alloc(Kmem* kmem, size_t i) {
  KASSERT(i < kmem->npages);
  ++kmem->nalloc;
  kmem->bitmap[i / 8] |= (1 << (i % 8));
}

static inline void pagei_free(Kmem* kmem, size_t i) {
  KASSERT(i < kmem->npages);
  --kmem->nalloc;
  kmem->bitmap[i / 8] &= ~(1 << (i % 8));
}

void kmem_init(Kctx* kctx) {
  int seg0_idx = 0;
  uint64_t total_bytes = 0;
  uint64_t total_pages = 0;
  uint64_t max_seglen = 0;

  for (int i = 0; i < kctx->mmap->entry_count; ++i) {
    struct limine_memmap_entry* entry = kctx->mmap->entries[i];
    if (entry->type != LIMINE_MEMMAP_USABLE) continue;
    if (entry->length > max_seglen) {
      seg0_idx = i;
      max_seglen = entry->length;
    }
    total_bytes += entry->length;
    total_pages += entry->length / KPAGE_SZ;
  }

  // We'll use the first n pages for the memory subsystem metadata
  uint64_t bitmap_nbytes = DIVCEIL(total_pages, 8);
  uint64_t metadata_nbytes = sizeof(Kmem) + bitmap_nbytes;
  uint64_t metadata_npages = DIVCEIL(metadata_nbytes, KPAGE_SZ);

  Kmem* kmem = (Kmem*)(
      kctx->hhdm_offset +
      kctx->mmap->entries[seg0_idx]->base);
  *kmem = (Kmem){0};
  kmem->seg0_idx = seg0_idx;
  kmem->npages = total_pages;
  kmem->metadata_npages = metadata_npages;
  kmem->bitmap = (uint8_t*)&kmem->bitmap_data;

  klogf("memory subsystem kmem=%p\n  nentries=%d pagesz=%d, npages=%llu n=%llu\n  hhdm=%p, seg0=%p, nseg0=%llu\n  nmetapages=%u",
      kmem, kctx->mmap->entry_count, KPAGE_SZ, total_pages, total_bytes,
      kctx->hhdm_offset, kctx->mmap->entries[seg0_idx]->base, max_seglen,
      metadata_npages);
  KASSERT(max_seglen >= metadata_nbytes);

  memset(kmem->bitmap, 0, bitmap_nbytes);
  for (size_t i = 0; i < metadata_npages; ++i) pagei_alloc(kmem, i);

  kctx->kmem = kmem;
}

void* kmem_page_alloc(Kctx* kctx) {
  Kmem* kmem = kctx->kmem;

  // Allocate the next free page
  uint64_t i = 0;
  for (; i < kmem->npages && page_inuse(i); ++i) {}
  KCHECK(!page_inuse(i), "OOM");
  pagei_alloc(kmem, i);

  // Is it in seg0?
  struct limine_memmap_entry* entry = kctx->mmap->entries[kmem->seg0_idx];
  if (i < entry->length / KPAGE_SZ) {
    return (void*)(kctx->hhdm_offset + entry->base + (i * KPAGE_SZ));
  }

  // Otherwise, iterate through the memmap entries
  for (int j = 0; j < kctx->mmap->entry_count; ++j) {
    if (j == kmem->seg0_idx) continue;
    struct limine_memmap_entry* entry = kctx->mmap->entries[j];
    if (entry->type != LIMINE_MEMMAP_USABLE) continue;
    uint64_t npages = entry->length / KPAGE_SZ;
    if (i < npages) {
      return (void*)(kctx->hhdm_offset + entry->base + (i * KPAGE_SZ));
    }
    i -= npages;
  }

  KCHECK(0, "unreachable");
}

void kmem_page_free(Kctx* kctx, void* p) {
  Kmem* kmem = kctx->kmem;

  // Map p back to its page idx
  uint64_t a = (uint64_t)p;
  a -= kctx->hhdm_offset;
  uint64_t page = 0;
  for (int i = 0; i < kctx->mmap->entry_count; ++i) {
    struct limine_memmap_entry* entry = kctx->mmap->entries[i];
    if (a >= entry->base && (a - entry->base) < entry->length) {
      page += (a - entry->base) / KPAGE_SZ;
      break;
    }
    page += entry->length / KPAGE_SZ;
  }

  pagei_free(kmem, page);
}
