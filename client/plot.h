#ifndef PLOT_H_
#define PLOT_H_

#include <stdint.h>

/**
 * Initializes the plot context.
 * <p>
 * A marsemu server must be already running before this function is called.
 */
int plot_init();

/**
 * De-initializes the plot context.
 * <p>
 * The shared memory is not destroyed.
 */
void plot_deinit();

/**
 * Draw a pixel
 * <p>
 * Will check for bounds for (x, y). Can be disabled by defining PLOT_NOCHECK
 *
 * @param x     the x-coordinate of the pixel
 * @param y     the y-coordinate of the pixel
 * @param color the color of the pixel in 24-bit RGB format
 * @return      0 if the pixel has been successfully drawn, otherwise 1
 */
int plot_draw(uint32_t x, uint32_t y, uint32_t color);

#endif  // PLOT_H_
