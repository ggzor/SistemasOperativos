#include "Operaciones.h"

#include "ParserOpciones.h"
#include "Utilerias.h"

#include <stdio.h>
#include <string.h>

void ayuda() {
  printf(
    "OperacionesIPC - Utilerías para probar operaciones de comunicación entre procesos.\n\n"
    "Uso: ./OperacionesIPC COMANDO [OPCIONES...]\n\n"
    "Comandos:\n"
    "  sem    Operaciones sobre semáforos\n"
    "  shm    Operaciones sobre memoria compartida\n"
    "  msg    Operaciones sobre cola de mensajes\n"
    "  help   Muestra este mensaje de ayuda\n"
  );
}

int main(int argc, char **argv) {
  Parser opciones = inicializarParser(argc, argv);
  char *comando;
  char *comandos[] = { "sem", "shm", "msg", "help" };

  if (siguienteComando(&opciones, &comando)) {
    switch (indiceDeCadena(comandos, 4, comando)) {
      case 0:
        return operacionesSemaforos(&opciones);
      case 1:
        return operacionesMemoriaCompartida(&opciones);
      case 2:
        return operacionesMensajes(&opciones);
      case 3:
        ayuda();
        return 0;
      default:
        printf("\e[31mERROR:\e[0m El comando \"%s\" no se ha reconocido.\n", comando);
        ayuda();
        return -1;
    }
  } else {
    printf("\e[31mERROR:\e[0m No se proporcionó un comando para el programa.\n");
    ayuda();
    return -1;
  }

  return 0;
}