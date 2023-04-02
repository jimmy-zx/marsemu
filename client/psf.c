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

struct psf1_header *psf_font = NULL;
uint16_t *psf_unicode = NULL;
static int fontfd;
static struct stat statbuf;

int psf_init(const char *psffile) {
  /* read the file */
  if ((fontfd = open(psffile, O_RDONLY)) < 0) {
    perror("open");
    return 1;
  }
  if (fstat(fontfd, &statbuf) < 0) {
    perror("fstat");
    return 1;
  }
  if ((psf_font = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fontfd, 0)) == (void *)-1) {
    perror("mmap");
    return 1;
  }
  /* handle header
   *  We are only handling PSF1.
   *  The following are ignored:
   *   - PSF1_MODEHASSEQ
   *   - PSF1_STARTSEQ
   */
  assert(psf_font->magic[0] == PSF1_MAGIC0);
  assert(psf_font->magic[1] == PSF1_MAGIC1);
  size_t nglyph = 256;
  if (psf_font->mode & PSF1_MODE512) {
    nglyph = 512;
  }
  if (psf_font->mode & PSF1_MODEHASTAB) {  // read the unicode table
    uint16_t *data = (uint16_t *)((char *)psf_font + sizeof(struct psf1_header) + nglyph * psf_font->charsize);
    uint16_t glyph = 0;
    if ((psf_unicode = calloc(UINT16_MAX, sizeof(uint16_t))) == NULL) {
      perror("calloc");
      return 1;
    }
    while (glyph < nglyph) {
      uint16_t c = *data;
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      c = (c >> 8) | (c << 8);
#endif
      if (c == PSF1_SEPARATOR) {
        glyph++;
        data++;
        continue;
      }
#ifdef PSF_DEBUG
      uint8_t lo = c, hi = c >> 8;
      fprintf(stderr, "g=%3hx: ", glyph);
      fprintf(stderr, "lo=%2hhx ", lo);
      if (isprint(lo)) {
        fprintf(stderr, "(%c) ", lo);
      } else {
        fprintf(stderr, "    ");
      }
      fprintf(stderr, "hi=%2hhx ", hi);
      if (isprint(hi)) {
        fprintf(stderr, "(%c) ", hi);
      } else {
        fprintf(stderr, "    ");
      }
      fputc('\n', stderr);
#endif
      psf_unicode[c] = glyph;
      data++;
    }
  }
  return 0;
}

void psf_deinit() {
  if (psf_unicode != NULL) {
    free(psf_unicode);
  }
  if (psf_font != (void *)-1  && psf_font != NULL && munmap(psf_font, statbuf.st_size) == -1) {
    perror("munmap");
  }
  if (fontfd >= 0) {
    close(fontfd);
  }
}

uint8_t *psf_getglyph(uint16_t ch) {
  uint16_t charloc = (psf_unicode == NULL) ? ch : psf_unicode[ch];
  return (uint8_t *)((char *)psf_font + sizeof(struct psf1_header) + charloc * psf_font->charsize);
}

void psf_printfont(uint16_t ch, FILE *stream) {
  uint8_t *glyph = psf_getglyph(ch);
  for (int dy = 0; dy < psf_font->charsize; dy++) {
    for (int dx = 0; dx < 8; dx++) {
      int present;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      present = (*glyph) & (1 << dx);
#else
      present = (*glyph) & (1 << (7 - dx));
#endif
      if (present) {
        fputc('*', stream);
      } else {
        fputc(' ', stream);
      }
    }
    fputc('\n', stream);
    glyph++;
  }
}

int psf_drawfont(uint16_t ch, uint32_t x, uint32_t y, uint32_t color) {
  uint8_t *glyph = psf_getglyph(ch);
  // a glyph consists of (charsize)x8
  for (int dy = 0; dy < psf_font->charsize; dy++) {
    for (int dx = 0; dx < 8; dx++) {
      int present;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      present = (*glyph) & (1 << dx);
#else
      present = (*glyph) & (1 << (7 - dx));
#endif
      if (plot_draw(x + dx, y + dy, present ? color : 0x0) != 0) {
        return 1;
      }
    }
    glyph++;
  }
  return 0;
}
