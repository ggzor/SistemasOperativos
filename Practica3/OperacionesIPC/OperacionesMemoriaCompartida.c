#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

int operacionesMemoriaCompartida(Parser *parser) {
  char *comando;
  char *comandos[] = { "crear", "escribir", "leer", "liberar" };

  if (siguienteComando(parser, &comando)) {
    switch (indiceDeCadena(comandos, 4, comando))
    {
    case 0:
      // TODO: Crear
      break;
    
    case 1:
      // TODO: Escribir 
      break;
    
    case 2:
      // TODO: Leer 
      break;

    case 4:
      // TODO: Liberar 
      break;
    
    default:
      printf("El comando \"%s\" no se ha reconocido.\nSólo puede ser uno de: crear, escribir, leer o liberar.\n", comando);
      return -1;
    }
  } else {
    printf("No se proporcionó una operación para el comando shm.\n");
    return -1;
  }
}