#include <stdint.h>
#include "../../client/plot.h"
#include "../../xdefs.h"

int main() {
  plot_init();
  for (uint32_t x = 0; x < kWidth; x++) {
    for (uint32_t y = 0; y < kHeight; y++) {
      plot_draw(x, y, 0);
    }
  }
  plot_deinit();
}
