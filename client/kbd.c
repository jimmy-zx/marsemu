#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../xdefs.h"

#include "kbd.h"

static int shmid;
char *kbd_mem = NULL;

int kbd_init() {
  key_t key = ftok(SHM_KBDPATH, SHM_PROJID);
  if (key == -1) {
    perror("ftok");
    return 1;
  }
  if ((shmid = shmget(key, 2 * sizeof(char), 0660)) < 0) {
    perror("shmget");
    return 1;
  }
  if ((kbd_mem = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    return 1;
  }
#ifdef KBD_DEBUG
  fprintf(stderr, "Initialized kbd\n");
#endif
  return 0;
}

void kbd_deinit() {
  if (kbd_mem != (void *)-1 && kbd_mem != NULL && shmdt(kbd_mem) == -1) {
    perror("shmdt");
  }
}

uint32_t kbd_keypressed() {
  return kbd_mem[0];
}

uint32_t kbd_whichkey() {
  return kbd_mem[1];
}
