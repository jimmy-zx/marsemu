#include "../client/plot.h"
#include "../client/psf.h"

int main() {
  if (plot_init() != 0) {
    return 1;
  }
  if (psf_init("./bin/Lat15-Terminus16.psf") != 0) {
    return 1;
  }
  psf_plot('C', 0, 0, 0xffffff);
  psf_deinit();
  plot_deinit();
}
