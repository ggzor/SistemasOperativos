#include "Operaciones.h"

#include "ParserOpciones.h"
#include "Utilerias.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  Parser opciones = inicializarParser(argc, argv);
  char *comando;
  char *comandos[] = { "sem", "shm", "msg" };

  if (siguienteComando(&opciones, &comando)) {
    switch (indiceDeCadena(comandos, 3, comando)) {
      case 0:
        return operacionesSemaforos(&opciones);
        break;
      case 1:
        return operacionesMemoriaCompartida(&opciones);
        break;
      case 2:
        return operacionesMensajes(&opciones);
        break;
      default:
        printf("El comando \"%s\" no se ha reconocido.\nSólo puede ser uno de: sem, shm o msg.\n", comando);
        return -1;
    }
  } else {
    printf("No se proporcionó un comando para el programa.\n");
    return -1;
  }

  return 0;
}