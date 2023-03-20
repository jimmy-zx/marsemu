#include "../../client/plot.h"

int main() {
  if (plotinit() != 0) {
    return 1;
  }
  for (int i = 20; i < 120; i++) {
    plot(20, i, 0xffffff);
  }
  for (int i = 20; i < 120; i++) {
    plot(120, i, 0xffffff);
  }
  for (int i = 20; i < 120; i++) {
    plot(i, 20, 0xffffff);
  }
  for (int i = 20; i < 120; i++) {
    plot(i, 120, 0xffffff);
  }
  plotdeinit();
}
