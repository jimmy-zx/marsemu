#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>
#include "../client/psf.h"

extern struct psf1_header *psf_font;
extern uint16_t *psf_unicode;

int main() {
  psf_init("./bin/Lat15-Terminus16.psf");
  printf("# generated from Lat15-Terminus16.psf\n");
  for (int c = 0; c < 256; c++) {
    if (isprint(c)) {
      printf("char_%d:\t# %c\n", c, c);
      uint8_t *glyph = psf_getglyph(c);
      for (size_t dy = 0; dy < psf_font->charsize; dy++) {
        printf("\t.word\t0x%" PRIx8 "\n", glyph[dy]);
      }
      // psf_printfont(c, stdout);
    } else {
      printf("char_%d:\n", c);
      for (size_t dy = 0; dy < psf_font->charsize; dy++) {
        printf("\t.word\t0x%" PRIx8 "\n", 0);
      }
    }
  }
}
