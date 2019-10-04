#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

void operacionesMensajes() {
  do {
    limpiarPantalla();
    printf("=== Operaciones de mensajes ===\n");
    printf("1. \n");
    printf("2. \n");
    printf("3. \n");
    printf("4. Regresar\n");
    printf("\n\n");

    switch (obtenerOpcion()) {
      case 1:
        operacionesSemaforos();
        break;

      case 2:
        operacionesMemoriaCompartida();
        break;

      case 3:
        operacionesMensajes();
      break;

      case 4:
        return;
      default:
        printf("\nNo se reconoció la opción.\n");
    }

  } while (1);
}