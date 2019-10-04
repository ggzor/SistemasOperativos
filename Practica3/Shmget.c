#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main() {
  int *p;
  int shmid = shmget(0, sizeof(int), IPC_PRIVATE | IPC_CREAT | 0666);
  p = (int *) shmat(shmid, NULL, 0);
  *p = 10;
  printf("%d\n", shmid);
  printf("%d\n", *p);
}