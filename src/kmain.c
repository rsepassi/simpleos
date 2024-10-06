#include "kernel.h"

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the -e link
// argument.
void kmain(void) {
  Kctx kctx = {0};
  kinit(&kctx);
  kfb_paint(&kctx, FB_RGB(0, 255, 0));
  KASSERT(false);
}
