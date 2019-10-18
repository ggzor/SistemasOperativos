#include <stdio.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define LEFT  (5+i-1)%5 
#define RIGHT (5+i+1)%5 
#define MUTEX   -1
#define THINKING 0
#define HUNGRY   1
#define EATING   2


union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};  

void filosofo(int);
void dejarTenedores(int);
void tomarTenedores(int);
void prueba(int);

int main (int argc, char **argv) {
  int numFilosofos = 5, semidFilosofos, mutex;
  int iniciales = {1,1,1,1,1}, estado[numFilosofos];
  union semun operaciones;


  semidFilosofos = semget(IPC_PRIVATE, numFilosofos, IPC_CREAT | 0666);
  mutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

  operaciones.array = iniciales;
  semctl(semidFilosofos, 0, SETVAL, operaciones);

 
}

void think() {
  printf("Pinche ardillita, piencha.");
}

void eat() {
  printf("Paquetito gana.");
} 

void filosofo(int i) {
  while (1) {
    tomarTenedores(i);
    eat();
    dejarTenedores(i);
  }
}

void down() {

}
void up() {

}
void dejarTenedores(int i) {
  down();
  estado[i] = THINKING;
  prueba(LEFT);
  prueba(RIGHT);
  up();
}

void tomarTenedores(int i) {
  down();
  estado[i] = HUNGRY;
  prueba(i);
  up();

}
void prueba(int i) {

}