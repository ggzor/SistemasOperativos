#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

void crearSemaforo();
void operarSobreSemaforo();
void removerSemaforo();

void operacionesSemaforos() {
  do {
    limpiarPantalla();
    printf("=== Operaciones de semáforos ===\n");
    printf("1. Crear semáforo\n");
    printf("2. Operar sobre semáforo\n");
    printf("3. Remover semáforo\n");
    printf("4. Regresar\n");

    switch (obtenerOpcion()) {
      case 1:
        crearSemaforo();
        break;

      case 2:
        operarSobreSemaforo();
        break;

      case 3:
        removerSemaforo();
        break;

      case 4:
        return;

      default:
        printf("\nNo se reconoció la opción.\n");
        esperarEnter();
    }

  } while (1);
}

void crearSemaforo() {
  int numeroSemaforos, permisos, semid;

  printf("Ingrese el número de semáforos que quiere crear: ");
  scanf("%d%*c", &numeroSemaforos);

  printf("Ingrese los permisos del semáforo (en octal): ");
  scanf("%o%*c", &permisos);

  semid = semget(IPC_PRIVATE, numeroSemaforos, IPC_CREAT | permisos);
  if (semid >= 0) 
    printf("Se creó exitosamente el semáforo con semid %d.\n", semid);
  else 
    printf("Ocurrió un error al crear el semáforo.\n");
  
  esperarEnter();
}

void escribirSemaforo(int semid, struct semid_ds *info);
void leerSemaforo(int semid, struct semid_ds *info);
void operarSemaforo(int semid, struct semid_ds *info);

void operarSobreSemaforo() {
  int semid;
  union semun op;
  struct semid_ds info;
  op.buf = &info;

  limpiarPantalla();
  printf("=== Operar sobre semáforo ===\n");

  printf("Introduzca el semid del semáforo: ");
  scanf("%d%*c", &semid);

  if (semctl(semid, 0, IPC_STAT, op) < 0) {
    printf("No existe algún semáforo con el semid especificado.\n");
    esperarEnter();
    return;
  }

  do {
    limpiarPantalla();
    printf("=== Operar sobre semáforo ===\n");
    printf("\nEl semid del semáforo actual es %d que contiene %d semáforos.\n\n", semid, info.sem_nsems);

    printf("1. Escribir \n");
    printf("2. Leer \n");
    printf("3. Operar \n");
    printf("4. Regresar \n");

    switch (obtenerOpcion()) {
    case 1:
      escribirSemaforo(semid, &info);
      break;
      
    case 2:
      leerSemaforo(semid, &info);
      break;

    case 3:
      operarSemaforo(semid, &info);
      break;

    case 4:
      return;
    default:
      printf("\nNo se reconoció la opción.\n");
      esperarEnter();
    }
  } while (1);
}

void escribirSemaforo(int semid, struct semid_ds *info) {
  int semnum, valor;
  union semun operaciones;

  do {
    limpiarPantalla();
    printf("=== Escritura de semáforo ===\n");
    printf("\nEl semid del semáforo actual es %d que contiene %d semáforos.\n\n", semid, info->sem_nsems);
    printf("1. Escritura única\n");
    printf("2. Escritura múltiple\n");
    printf("3. Regresar\n");

    switch (obtenerOpcion()) {
    case 1:
      printf("Ingrese el número de semáforo que quiere operar: ");
      scanf("%d%*c", &semnum);

      printf("Ingrese el valor al que quiere establecer el semáforo: ");
      scanf("%d%*c", &valor);
      operaciones.val = valor;

      if (semctl(semid, semnum, SETVAL, operaciones) >= 0)
        printf("Se estableció el valor correctamente.\n");
      else
        printf("No se pudo establecer el valor del semáforo.\n");

      esperarEnter();
      break;
    case 2:
      break;
    case 3:
      return;
    default:
      printf("\nNo se reconoció la opción.\n");
      esperarEnter();
    }
  } while (1);
}

void leerSemaforo(int semid, struct semid_ds *info) {
  int semnum, valor;

  do {
    limpiarPantalla();
    printf("=== Lectura de semáforo ===\n");
    printf("\nEl semid del semáforo actual es %d que contiene %d semáforos.\n\n", semid, info->sem_nsems);
    printf("1. Lectura única \n");
    printf("2. Lectura múltiple\n");
    printf("3. Regresar\n");

    switch (obtenerOpcion()) {
    case 1:
      printf("Ingrese el número de semáforo que quiere operar: ");
      scanf("%d%*c", &semnum);

      if ((valor = semctl(semid, semnum, GETVAL)) >= 0)
        printf("El semáforo %d tiene el valor %d.\n", semnum, valor);
      else
        printf("No se pudo obtener el valor del semáforo.\n");

      esperarEnter();
      break;
    case 2:
      
      break;
    case 3:
      return;
    default:
      printf("\nNo se reconoció la opción.\n");
      esperarEnter();
    }
  } while (1);
}

void operarSemaforo(int semid, struct semid_ds *info) {
  int semnum, operacion;
  struct sembuf down = { 0, -1, 0 };
  struct sembuf up = {0, +1, 0 };
  struct sembuf op = { 0, 0, 0 };
  
  do {
    limpiarPantalla();
    printf("=== Operar semáforo ===\n");
    printf("\nEl semid del semáforo actual es %d que contiene %d semáforos.\n\n", semid, info->sem_nsems);
    printf("1. Decrementar\n");
    printf("2. Incrementar\n");
    printf("3. Agregar o restar otra cantidad\n");
    printf("4. Regresar\n");

    switch (obtenerOpcion()) {
    case 1:
      printf("Ingrese el número de semáforo que quiere operar: ");
      scanf("%d%*c", &semnum);
      down.sem_num = (unsigned short) semnum;

      if(semop(semid, &down, 1) >= 0) 
        printf("El decremento fue exitoso.\n");
      else 
        printf("No se pudo decrementar el valor del semáforo.\n");

      esperarEnter();
      break;
    case 2:
      printf("Ingrese el número de semáforo que quiere operar: ");
      scanf("%d%*c", &semnum);
      up.sem_num = (unsigned short) semnum;
      
      if(semop(semid, &up, 1) >= 0) 
        printf("El incremento fue exitoso.\n");
      else 
        printf("No se pudo incrementar el valor del semáforo.\n");

      esperarEnter();
      break;
    case 3:
      printf("Ingrese el número de semáforo que quiere operar: ");
      scanf("%d%*c", &semnum);
      op.sem_num = (unsigned short) semnum;

      printf("Ingrese la cantidad de incremento o decremento que desea agregar: ");
      scanf("%d%*c", &operacion);
      op.sem_op = (short) operacion;

      if(semop(semid, &op, 1) >= 0) 
        printf("La operación fue exitosa.\n");
      else 
        printf("No se pudo incrementar el valor del semáforo.\n");

      esperarEnter();
      break;
    case 4:
      return;
    default:
      printf("\nNo se reconoció la opción.\n");
      esperarEnter();
    }
  } while (1);
}

void removerSemaforo() {
  int semid;

  printf("Ingrese el semid del semáforo a borrar: ");
  scanf("%d%*c", &semid);

  if (semctl(semid, 0, IPC_RMID) >= 0) 
    printf("El semáforo se ha borrado exitosamente\n");
  else 
    printf("Ocurrió un error al borrar el semáforo.\n");
  
  esperarEnter();
}