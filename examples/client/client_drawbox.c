#include <stdint.h>

int plot_draw(uint32_t x, uint32_t y, uint32_t color);

void drawbox(uint32_t x, uint32_t y, uint32_t color, uint32_t dx, uint32_t dy) {
  for (uint32_t i = 0; i < dy; i++) {
    for (uint32_t j = 0; j < dx; j++) {
      plot_draw(x + j, y + i, color);
    }
  }
}
