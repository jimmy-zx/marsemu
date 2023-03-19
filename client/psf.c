#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "plot.h"
#include "psf.h"

static int fontfd;
static uint16_t *unicode;
static PSF1_Header *font;
static struct stat statbuf;

int psf_init(const char *psffile) {
  if ((fontfd = open(psffile, O_RDONLY)) < 0) {
    perror("open");
    return 1;
  }
  if (fstat(fontfd, &statbuf) < 0) {
    perror("fstat");
    return 1;
  }
  if ((font = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fontfd, 0)) == (void *)-1) {
    perror("mmap");
    return 1;
  }
  assert(font->magic == PSF1_FONT_MAGIC);
  assert(font->fontMode == 0x2);

  uint16_t *data = (uint16_t *)((char *)font + sizeof(PSF1_Header) + 256 * font->characterSize);
  uint16_t glyph = 0;
  if ((unicode = calloc(UINT16_MAX, sizeof(uint16_t))) == NULL) {
    perror("calloc");
    return 1;
  }
  while (glyph < 256) {
    uint16_t c = *data;
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    c = (c >> 8) | (c << 8);
#endif
    if (c == 0xFFFF) {
      glyph++;
      data++;
      continue;
    }
    uint8_t lo = c, hi = c >> 8;
    printf("g=%3hx: ", glyph);
    printf("lo=%2hhx ", lo);
    if (isprint(lo)) {
      printf("(%c) ", lo);
    } else {
      printf("    ");
    }
    printf("hi=%2hhx ", hi);
    if (isprint(hi)) {
      printf("(%c) ", hi);
    } else {
      printf("    ");
    }
    printf("\n");
    unicode[c] = glyph;
    data++;
  }
  return 0;
}

void psf_plot(int ch, int x, int y, uint32_t color) {
  // 8x16
  uint8_t *glyph = (uint8_t *)((char *)font + sizeof(PSF1_Header) + unicode[ch] * font->characterSize);
  for (int dy = 0; dy < font->characterSize; dy++) {
    for (int dx = 0; dx < 8; dx++) {
      int present;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      present = (*glyph) & (1 << dx);
#else
      present = (*glyph) & (1 << (7 - dx));
#endif
      if (present) {
        printf("*");
      } else {
        printf(" ");
      }
      plot(x + dx, y + dy, present ? color : 0x0);
    }
    printf("\n");
    glyph++;
  }
}

void psf_deinit() {
  free(unicode);
  if (munmap(font, statbuf.st_size) == -1) {
    perror("munmap");
  }
  close(fontfd);
}
