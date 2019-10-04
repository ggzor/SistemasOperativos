#include <stdio.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};

int main() {
  int semid = semget(0, 4, 0666 | IPC_CREAT);

  short valores[] = { 5, 4, 3, 2 };
  union semun un;

  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = 2;
  op.sem_flg = 0;

  un.array = valores;

  if (semid >= 0)
    if (semctl(semid, 0, SETALL, un) >= 0)
      if (semop(semid, &op, 1) >= 0)
        printf("Ok\n");

  return 0;
}