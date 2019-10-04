#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

void crearMemoriaCompartida();
void escribirMemoriaCompartida();
void leerMemoriaCompartida();
void liberarMemoriaCompartida();

void operacionesMemoriaCompartida() {
  int opcion;

  do {
    limpiarPantalla();
    printf("=== Operaciones de memoria compartida ===\n");
    printf("1. Crear bloque de memoria compartida\n");
    printf("2. Escribir en memoria compartida\n");
    printf("3. Leer memoria compartida\n");
    printf("4. Liberar bloque de memoria compartida\n");
    printf("5. Regresar\n");

    switch (obtenerOpcion()) {
      case 1:
        crearMemoriaCompartida();
        break;

      case 2:
        escribirMemoriaCompartida();
        break;

      case 3:
        leerMemoriaCompartida();
        break;

      case 5:
        return;

      default:
        printf("\nNo se reconoció la opción.\n");
        esperarEnter();
    }
  } while (1);
}

void crearMemoriaCompartida() {
  int tamano, permisos, shmid;

  printf("Introduzca el tamaño del bloque de memoria compartida (en bytes): ");
  scanf("%d%*c", &tamano);

  printf("Ingrese los permisos del semáforo (en octal): ");
  scanf("%o%*c", &permisos);

  // TODO: Asignar memoria compartida

  if (shmid >= 0) 
    printf("Se creó exitosamente el bloque de memoria compartida con shmid %d.\n", shmid);
  else 
    printf("Ocurrió un error al crear el bloque de memoria compartida.\n");
  
  esperarEnter();
}

void escribirMemoriaCompartida() {
  int shmid;
  struct shmid_ds info;
  char *memoria;

  printf("Introduzca el shmid de la memoria compartida: ");
  scanf("%d%*c", &shmid);

  // TODO: Obtener información del bloque de memoria compartida y validar

  // TODO: Adquirir segmento de memoria compartida

  printf("Introduzca una cadena que se escribirá en la memoria compartida (max %d caracteres): ", info.shm_segsz - 1);
  // TODO: Escribir en memoria compartida
}

void leerMemoriaCompartida() {
  int shmid;
  struct shmid_ds info;
  char *memoria;

  printf("Introduzca el shmid de la memoria compartida: ");
  scanf("%d%*c", &shmid);

  // TODO: Obtener información del bloque de memoria compartida y validar

  // TODO: Adquirir segmento de memoria compartida

  // TODO: Colocar caracter a caracter en la pantalla hasta encontrar fin de cadena o maximo de caracteres 
}

void liberarMemoriaCompartida() {  
  int shmid;
  struct shmid_ds info;
  char *memoria;

  printf("Introduzca el shmid de la memoria compartida: ");
  scanf("%d%*c", &shmid);

  // TODO: Obtener información del bloque de memoria compartida y validar

  // TODO: Remover segmento de memoria compartida
}