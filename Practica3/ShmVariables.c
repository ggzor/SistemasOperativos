#include <stdio.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main() {
  int pid;

  int *p;
  int shmid = shmget(0, sizeof(int), IPC_CREAT | 0666);
  p = (int *) shmat(shmid, NULL, 0);
  *p = 0;

  pid = fork();

  if (pid == 0)
    while (1) {
      printf("P.H. %d\n", (*p)++);
      sleep(1);
    }
  else
    while (1) {
      printf("P.P. %d\n", --(*p));
      sleep(1);
    }
}