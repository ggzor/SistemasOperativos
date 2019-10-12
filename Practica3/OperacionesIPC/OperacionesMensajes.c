#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

int operacionesMensajes(Parser *parser) {
  char *comando;
  char *comandos[] = { "crear", "enviar", "recibir", "liberar" };

  if (siguienteComando(parser, &comando)) {
    switch (indiceDeCadena(comandos, 4, comando))
    {
    case 0:
      // TODO: Crear
      break;
    
    case 1:
      // TODO: Enviar
      break;
    
    case 2:
      // TODO: Recibir 
      break;

    case 4:
      // TODO: Liberar 
      break;
    
    default:
      printf("El comando \"%s\" no se ha reconocido.\nSólo puede ser uno de: crear, enviar, recibir o liberar.\n", comando);
      return -1;
    }
  } else {
    printf("No se proporcionó una operación para el comando msg.\n");
    return -1;
  }
}