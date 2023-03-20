#include "../../client/plot.h"
#include "../../client/psf.h"

int main() {
  plot_init();
  psf_init("./bin/Lat15-Terminus16.psf");
  psf_drawfont('C', 0, 0, 0xffffff);
  psf_deinit();
  plot_deinit();
}
