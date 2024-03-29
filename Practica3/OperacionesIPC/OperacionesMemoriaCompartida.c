#include "Operaciones.h"
#include "ParserOpciones.h"
#include "Utilerias.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

void ayudaMemoria() {
  printf(
    "Uso: ./OperacionesIPC shm COMANDO [OPCIONES...]\n\n"
    "Comandos:\n"
    "  crear NBYTES [PERMS=0666]\n"
    "     Crear nuevo bloque de memoria compartida"
         "de tamaño NBYTES\n"
    "  \n"
    "  escribir SHMID\n"
    "     Establecer los datos de un bloque de memoria "
         "compartida a partir de la entrada estandar\n"
    "  \n"
    "  leer SHMID\n"
    "     Leer los datos de un bloque de memoria compartida "
         "hasta encontrar un final de cadena \n"
    "  \n"
    "  liberar SHMID\n"
    "     Liberar el bloque de memoria compartida con "
         "el shmid especificado\n"
    "\n"
    "  help\n"
    "     Muestra este mensaje de ayuda\n"
  );  
}

int operacionesMemoriaCompartida(Parser *parser) {
  char *comando;
  char *comandos[] = { "crear", "escribir", "leer", "liberar", "help"};
  struct shmid_ds info;
  int tamano, permisos = 0666, shmid, i;
  char *memoria;

  if (siguienteComando(parser, &comando)) {
    switch (indiceDeCadena(comandos, 5, comando))
    {
    case 0:
      if (siguienteEntero(parser, &tamano)) {
        siguienteEntero(parser, &permisos); 
        
        shmid = shmget(IPC_PRIVATE, tamano, IPC_CREAT| permisos);
        
        if (shmid >= 0) {
          printf("Se creó exitosamente el bloque de memoria compartida con shmid %d.\n", shmid);
          return 0;
        } else {
          printf("Ocurrió un error al crear el bloque de memoria compartida.\n");
        } 
      } else {
        printf("No se proporcionó el número de bytes.\n");
      }
      return -1;
    
    case 1:
      if (siguienteEntero(parser, &shmid)) {
        if (shmctl(shmid, IPC_STAT, &info) >= 0) {
          memoria = shmat(shmid, 0, 0);
          fgets(memoria, info.shm_segsz, stdin);
          printf("\nSe escribieron los datos correctamente.\n");          
          return 0;
        } else {
          printf("No existe un bloque de memoria con el shmid especificado.\n");
        }
      } else {
        printf("No se proporcionó el shmid del bloque de memoria.\n");
      }
      return -1;
    
    case 2:
      if (siguienteEntero(parser, &shmid)) {
        if (shmctl(shmid, IPC_STAT, &info) >= 0) {
          memoria = shmat(shmid, 0, 0);
          
          for (i = 0; memoria[i] != '\0' && i < info.shm_segsz; i++)
            printf("%c", memoria[i]);
            
          return 0;
        } else {
          printf("No existe un bloque de memoria con el shmid especificado.\n");
        }
      } else {
        printf("No se proporcionó el shmid del bloque de memoria.\n");
      }
      return -1;

    case 3:
      if(siguienteEntero(parser, &shmid)) {
        if (shmctl(shmid, IPC_STAT, &info) >= 0) {
          if(shmctl(shmid, IPC_RMID, 0) >= 0) {
            printf("Se ha borrado correctamente el bloque de memoria compartida.\n");
            return 0;
          } else {
            printf("No se pudo borrar el bloque de memoria compartida.\n");
          } 
        } else {
          printf("No existe un bloque de memoria con el shmid especificado.\n");
        }
      } else {
        printf("No se proporcionó el shmid del bloque de memoria.\n");
      }
      return -1;

    case 4:
      ayudaMemoria();
      return 0;

    default:
      printf("\e[31mERROR:\e[0m El comando \"%s\" no se ha reconocido.\nSólo puede ser uno de: crear, escribir, leer o liberar.\n", comando);
      ayudaMemoria();
      return -1;
    }
  } else {
    printf("\e[31mERROR:\e[0m No se proporcionó una operación para el comando shm.\n");
    ayudaMemoria();
    return -1;
  }
}