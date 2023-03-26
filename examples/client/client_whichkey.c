#define _GNU_SOURCE

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include "../../client/kbd.h"

int main() {
  if (kbd_init() != 0) {
    return 1;
  }
  struct timespec ts;
  for (;;) {
    if (kbd_keypressed()) {
      clock_gettime(CLOCK_REALTIME, &ts);
      printf("%lld.%.9ld ", (long long)ts.tv_sec, ts.tv_nsec);
      printf("%" PRIu32 "\n", kbd_whichkey());
    }
  }
  kbd_deinit();
}
