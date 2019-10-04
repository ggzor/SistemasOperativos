
#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

int main() {
  do {
    limpiarPantalla();
    printf("=== Operaciones de comunicación entre procesos ===\n");
    printf("1. Semáforos\n");
    printf("2. Memoria compartida\n");
    printf("3. Mensajes\n");
    printf("4. Salir\n");

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
        return 0;

      default:
        printf("\nNo se reconoció la opción.\n");
    }
  } while (1);
}
