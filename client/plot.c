#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "../xdefs.h"
#include "plot.h"

static int shmid;
static char *mem;

int plotinit() {
  key_t key = ftok(MEM_KEYPATH, MEM_KEYID);
  if ((shmid = shmget(key, kWindowBufferSize, 0666)) < 0) {
    perror("shmget");
    return 1;
  }
  if ((mem = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    return 1;
  }
  return 0;
}

void plot(int x, int y, uint32_t color) {
  *(uint32_t *)(mem + x * kPixelBytes + y * kWidth * kPixelBytes) = color;
}

void plotdeinit() {
  if (shmdt(mem) == -1) {
    perror("shmdt");
  }
}
