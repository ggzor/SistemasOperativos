#include <stdio.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define N 5

#define LEFT  ((N + i - 1) % N)
#define RIGHT ((N + i + 1) % N)

#define MUTEX    -1

#define THINKING  0
#define HUNGRY    1
#define EATING    2

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};  

void filosofo(int);

int *estado;
int semidFilosofos, semidMutex;

int main (int argc, char **argv) {
  int i;
  unsigned short iniciales[N];
  union semun operaciones;

  estado = shmat(shmget(IPC_PRIVATE, sizeof(int) * N, IPC_CREAT | 0666), 0, 0);
  for (int i = 0; i < N; i++) {
    estado[i] = THINKING;
    iniciales[i] = 1;
  }

  semidFilosofos = semget(IPC_PRIVATE, N, IPC_CREAT | 0666);
  semidMutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

  operaciones.array = iniciales;
  semctl(semidFilosofos, 0, SETVAL, operaciones);

  operaciones.val = 1;
  semctl(semidMutex, 0, SETVAL, operaciones);
}

void think() {
  printf("Pinche ardillita, piencha.");
}

void eat() {
  printf("Paquetito gana.");
} 

void dejarTenedores(int);
void tomarTenedores(int);
void prueba(int);

void filosofo(int i) {
  while (1) {
    think();
    tomarTenedores(i);
    eat();
    dejarTenedores(i);
  }
}

void up(int semaforo) {
  struct sembuf op = { 
    .sem_num = 0, 
    .sem_op = 1, 
    .sem_flg = 0 
  };

  if (semaforo == MUTEX) {
    semop(semidMutex, &op, 1);
  } else {
    op.sem_num = semaforo;
    semop(semidFilosofos, &op, 1);
  }
}

void down(int semaforo) {
  struct sembuf op = { 
    .sem_num = 0, 
    .sem_op = -1, 
    .sem_flg = 0 
  };

  if (semaforo == MUTEX) {
    semop(semidMutex, &op, 1);
  } else {
    op.sem_num = semaforo;
    semop(semidFilosofos, &op, 1);
  }
}

void dejarTenedores(int i) {
  down(MUTEX);
  estado[i] = THINKING;
  prueba(LEFT);
  prueba(RIGHT);
  up(MUTEX);
}

void tomarTenedores(int i) {
  down(MUTEX);
  estado[i] = HUNGRY;
  prueba(i);
  up(MUTEX);
  down(i);
}

void prueba(int i) {
  if ((estado[i] == HUNGRY) &&
    (estado[LEFT] != EATING) &&
    (estado[RIGHT] != EATING)) {
    estado[i] = EATING;
    up[]
  }
}