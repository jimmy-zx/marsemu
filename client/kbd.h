#ifndef KBD_H_
#define KBD_H_

#include <stdint.h>

/**
 * Initializes the keyboard context.
 * <p>
 * A marsemu server must be already running before this function is called.
 */
int kbd_init();

/**
 * De-initializes the keyboard context.
 */
void kbd_deinit();

/**
 * Checks whether a key is pressed.
 */
uint32_t kbd_keypressed();

/**
 * Returns the keycode of the key pressed.
 */
uint32_t kbd_whichkey();

#endif  // KBD_H_
