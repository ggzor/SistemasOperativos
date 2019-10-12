#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

int operacionesSemaforos(Parser *parser) {
  char *comando;
  char *comandos[] = { "crear", "establecer", "leer", "operar", "liberar" };
  int numeroSemaforos, permisos = 0666, semid;

  if (siguienteComando(parser, &comando)) {
    switch (indiceDeCadena(comandos, 5, comando))
    {
    case 0:
      if(siguienteEntero(parser,&numeroSemaforos) ) {
        siguienteEntero(parser, &permisos);

        semid = semget(IPC_PRIVATE, numeroSemaforos, IPC_CREAT | permisos);
        if (semid >= 0) 
          printf("Se creó exitosamente el semáforo con semid %d.\n", semid);
        else 
          printf("Ocurrió un error al crear el semáforo.\n");

      } else {
        printf("No se proporcionó el número de semáforos.\n");
      } 
      break;
    
    case 1:
      // TODO: Establecer 
      break;
    
    case 2:
      // TODO: Leer 
      break;

    case 3:
      // TODO: Operar 
      break;
    
    case 4:
      // TODO: Liberar 
      break;
    
    default:
      printf("El comando \"%s\" no se ha reconocido.\nSólo puede ser uno de: crear, establecer, leer, operar o liberar.\n", comando);
      return -1;
    }
  } else {
    printf("No se proporcionó una operación para el comando sem.\n");
    return -1;
  }
}