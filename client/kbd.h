#ifndef KBD_H_
#define KBD_H_

#include <stdint.h>

int kbd_init();
void kbd_deinit();
uint32_t kbd_keypressed();
uint32_t kbd_whichkey();

#endif  // KBD_H_
