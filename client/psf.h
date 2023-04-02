#ifndef PSF_H_
#define PSF_H_

#include <stdint.h>
#include <stdio.h>

#define PSF1_MAGIC0     0x36
#define PSF1_MAGIC1     0x04

#define PSF1_MODE512    0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04
#define PSF1_MAXMODE    0x05

#define PSF1_SEPARATOR  0xFFFF
#define PSF1_STARTSEQ   0xFFFE

struct psf1_header {
        unsigned char magic[2];     /* Magic number */
        unsigned char mode;         /* PSF font mode */
        unsigned char charsize;     /* Character size */
};

/**
 * Initializes the psf context from a psf font file.
 * <p>
 * Only PSF1 files are supported.
 *
 * @param psffile an null-terminated string representing the path to
 *                the psf font file
 * @return        0 if successful, 1 if failed
 */
int psf_init(const char *psffile);

/**
 * De-initializes the psf context.
 */
void psf_deinit();

/**
 * Draw a character to the framebuffer
 *
 * @param ch    the unicode representation of the character
 * @param x     the x-coordinate of the top-left corner
 * @param y     the y-coordinate of the top-left corner
 * @param color the color of the character in 24-bit RGB format
 * @return      0 if the character has been successfully drawn, otherwise 1
 */
int psf_drawfont(uint16_t ch, uint32_t x, uint32_t y, uint32_t color);

uint8_t *psf_getglyph(uint16_t ch);
void psf_printfont(uint16_t ch, FILE *stream);

#endif  // PSF_H_
