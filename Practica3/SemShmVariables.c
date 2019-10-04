#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short  *array;
  struct seminfo  *__buf;
};

int main() {
  int pid, shmid, *p, semid, valor;

  union semun ops = { .val = 1 };
  struct sembuf down = { 0, -1, 0 }, up = { 0, +1, 0 };

  shmid = shmget(0, sizeof(int), IPC_CREAT | 0666);
  p = (int *) shmat(shmid, NULL, 0);
  *p = 0;

  semid = semget(0, 1, IPC_CREAT | 0666);
  semctl(semid, 0, SETVAL, ops);

  pid = fork();

  if (pid == 0) {
    while (1) {
      semop(semid, &down, 1);

      // Región crítica
      (*p)--;
      valor = *p;

      semop(semid, &up, 1);

      printf("P.H. %d\n", valor);
    }
  }
  else {
    while (1) {
      semop(semid, &down, 1);

      // Región crítica
      (*p)++;
      valor = *p;

      semop(semid, &up, 1);

      printf("P.P. %d\n", valor);
    }
  }
}