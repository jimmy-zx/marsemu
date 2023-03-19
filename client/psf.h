#ifndef PSF_H_
#define PSF_H_

#include <stdint.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define PSF1_FONT_MAGIC 0x0436
#else
#define PSF1_FONT_MAGIC 0x3604
#endif


typedef struct {
  uint16_t magic;        // Magic bytes for idnetiifcation.
  uint8_t fontMode;      // PSF font mode
  uint8_t characterSize; // PSF character size.
} PSF1_Header;

#define PSF_FONT_MAGIC 0x864ab572

typedef struct {
  uint32_t magic;         /* magic bytes to identify PSF */
  uint32_t version;       /* zero */
  uint32_t headersize;    /* offset of bitmaps in file, 32 */
  uint32_t flags;         /* 0 if there's no unicode table */
  uint32_t numglyph;      /* number of glyphs */
  uint32_t bytesperglyph; /* size of each glyph */
  uint32_t height;        /* height in pixels */
  uint32_t width;         /* width in pixels */
} PSF_font;

int psf_init(const char *psffile);
void psf_plot(int ch, int x, int y, uint32_t color);
void psf_deinit();

#endif  // PSF_H_
