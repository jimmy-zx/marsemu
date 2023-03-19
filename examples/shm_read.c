#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define HEIGHT 256
#define WIDTH 256

int main() {
  key_t key = ftok("shmfile", 65);
  int shmid = shmget(key, HEIGHT * WIDTH * sizeof(char), 0666 | IPC_CREAT);
  char *buf = (char *) shmat(shmid, NULL, 0);
  printf("%s\n", buf);
  shmdt(buf);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}
