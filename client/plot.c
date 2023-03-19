#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "../xdefs.h"
#include "plot.h"

static int shmid;
char *plot_mem = NULL;

int plot_init() {
  key_t key = ftok(MEM_KEYPATH, MEM_KEYID);
  if ((shmid = shmget(key, kWindowBufferSize, 0666)) < 0) {
    perror("shmget");
    return 1;
  }
  if ((plot_mem = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    return 1;
  }
  return 0;
}

void plot_deinit() {
  if (plot_mem != (void *)-1 && plot_mem != NULL && shmdt(plot_mem) == -1) {
    perror("shmdt");
  }
}

int plot_draw(uint32_t x, uint32_t y, uint32_t color) {
#ifndef PLOT_NOCHECK
  if (x >= (uint32_t)kWidth || y >= (uint32_t)kHeight) {
    return 1;
  }
#endif
  *(uint32_t *)(plot_mem + x * kPixelBytes + y * kWidth * kPixelBytes) = color;
  return 0;
}
