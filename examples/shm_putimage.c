#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "xdefs.h"

int main() {
  key_t key = ftok("shmfile", 65);
  int shmid = shmget(key, kWindowBufferSize, 0666);
  if (shmid < 0) {
    perror("shmget");
    return 1;
  }
  char *mem = shmat(shmid, NULL, 0);
  if ((intptr_t) mem == -1) {
    perror("shmat");
    return 1;
  }

  int pitch = kWidth * kPixelBytes;
  for (int y = 0; y < kHeight; y++) {
    char *row = mem + (y * pitch);
    for (int x = 0; x < kWidth; x++) {
      unsigned int *p = (unsigned int *) (row + (x * kPixelBytes));
      if (x % 16 && y % 16) {
        *p = 0xffffffff;
      } else {
        *p = 0;
      }
    }
  }

  if (shmdt(mem) == -1) {
    perror("shmdt");
    return 1;
  }
  return 0;
}
