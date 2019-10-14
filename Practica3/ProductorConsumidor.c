#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define FULL 0
#define EMPTY 1
#define MUTEX 2

// Parámetros de funcionamiento
#define N 20

#define MIN_PRODUCTOR 1
#define MAX_PRODUCTOR 1

#define MIN_CONSUMIDOR 2
#define MAX_CONSUMIDOR 4

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

typedef struct { 
  int inicio;
  int fin;
  int n;
  int max;
  int cola[N]; 
} Estado;


int semid;
Estado *estado;

void consumidor(void), productor(void);

int main () {
  // Inicializar memoria compartida
  int shmid = shmget(IPC_PRIVATE, sizeof(Estado), IPC_CREAT | 0666);
  estado = (Estado *) shmat(shmid, 0, 0);
  estado->inicio = 0;
  estado->fin = 0;
  estado->n = 0;
  estado->max = N;
  
  // Inicializar semáforos  
  union semun valor;
  semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0666);

  valor.val = 0;
  semctl(semid, 0, SETVAL, valor);

  valor.val = N;
  semctl(semid, 1, SETVAL, valor);

  valor.val = 1;
  semctl(semid, 2, SETVAL, valor);
  
  // Crear procesos
  if (fork() == 0) {
    consumidor();
  } else {
    productor();
  }

  return 0;
} 

void up(short semaforo), down(short semaforo);

void agregarElemento(int elemento);
int removerElemento();

int producirElemento();
void productor() {
  int elemento;

  while (1) {
    elemento = producirElemento();
    
    down(EMPTY); 
    
    down(MUTEX);
    agregarElemento(elemento);
    up(MUTEX); 
    
    up(FULL);
  }
}

int producirElemento() {
  int elemento = 1 + rand() % 100;
  int espera = MIN_PRODUCTOR + rand() % (MAX_PRODUCTOR - MIN_PRODUCTOR + 1);
  time_t fin = time(NULL) + espera;
  
  while (time(NULL) < fin);

  printf("🏭  \e[2mSe ha producido\e[0m \e[34m%d\e[0m\n", elemento);
  return elemento;
}

void consumirElemento(int elemento);
void consumidor() {
  int elemento;
  
  while (1) {
    down(FULL);

    down(MUTEX);
    elemento = removerElemento();
    up(MUTEX);
    
    up(EMPTY);

    consumirElemento(elemento);
  }
}

void consumirElemento(int elemento) {
  int espera = MIN_CONSUMIDOR + rand() % (MAX_CONSUMIDOR - MIN_CONSUMIDOR + 1);
  time_t fin = time(NULL) + espera;
  
  printf("⏳  \e[2mSe empezará a consumir\e[0m \e[31m%d\e[0m\n", elemento);
  while (time(NULL) < fin);
  printf("✔️  \e[2mSe ha terminado de consumir\e[0m \e[32;1m%d\e[0m\n", elemento);
}


void up(short semaforo) {
  struct sembuf up = {semaforo, +1, 0};

  if (semop(semid, &up, 1) < 0) {
    printf("No se pudo incrementar el semáforo.\n");
    _exit(-1);
  }
} 

void down(short semaforo) {
  struct sembuf down = {semaforo, -1, 0};
  
  if (semop(semid, &down, 1) < 0) {
    printf("No se pudo decrementar el semáforo.\n");
    _exit(-1);
  }
}

void agregarElemento(int elemento) {
  if (estado->n == estado->max) {
    printf("Se trató de insertar en una cola llena.\n");
    _exit(-1);
  } else {
    estado->cola[estado->inicio] = elemento;
    estado->inicio = (estado->max + (estado->inicio - 1)) % estado->max;
    estado->n++;
  }
}

int removerElemento() {
  int elemento;

  if (estado->n == 0) {
    printf("Se trató de remover de una cola vacía.\n");
    _exit(-1);
  } else {
    elemento = estado->cola[estado->fin];
    estado->fin = (estado->max + (estado->fin - 1)) % estado->max;
    estado->n--;
    return elemento;
  }
}