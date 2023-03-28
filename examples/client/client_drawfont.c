#include "../../client/plot.h"
#include "../../client/psf.h"

int main() {
  plot_init();
  psf_init("./bin/Lat15-Terminus16.psf");
  psf_drawfont('M', 0, 0, 0x00ffffff);
  psf_deinit();
  plot_deinit();
}
