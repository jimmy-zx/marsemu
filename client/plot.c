#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "../xdefs.h"
#include "plot.h"

static int shmid;
char *plot_mem = NULL;

int plot_init() {
  key_t key = ftok(SHM_VFBPATH, SHM_PROJID);
  if (key == -1) {
    perror("ftok");
    return 1;
  }
#ifdef PLOT_DEBUG
  printf("%llx\n", (long long)key);
  printf("%llx\n", (long long)kWindowBufferSize);
#endif
  if ((shmid = shmget(key, kWindowBufferSize, 0660)) < 0) {
    perror("shmget");
    return 1;
  }
  if ((plot_mem = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    return 1;
  }
#ifdef PLOT_DEBUG
  fprintf(stderr, "Initialized plot\n");
#endif
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
#ifdef PLOT_DEBUG
  fprintf(stderr, "(%" PRIu32 ", %" PRIu32 ")=%" PRIx32 "\n",x, y, color);
#endif
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  color = ((color >> 24) & 0xff) | ((color << 8) & 0xff0000) |
          ((color >> 8) & 0xff00) | ((color << 24) & 0xff000000);
#endif
  *(uint32_t *)(plot_mem + x * kPixelBytes + y * kWidth * kPixelBytes) = color;
  return 0;
}
