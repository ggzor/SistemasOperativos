#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>

void crearMemoriaCompartida();
void escribirMemoriaCompartida();
void leerMemoriaCompartida();

void operacionesMemoriaCompartida() {
  int opcion;

  do {
    limpiarPantalla();
    printf("=== Operaciones de memoria compartida ===\n");
    printf("1. Crear bloque de memoria compartida\n");
    printf("2. Escribir en memoria compartida\n");
    printf("3. Leer memoria compartida\n");
    printf("4. Regresar\n");

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

      case 4:
        return;
      default:
        printf("\nNo se reconoció la opción.\n");
    }
  } while (1);
}

void crearMemoriaCompartida() {

}

void escribirMemoriaCompartida() {

}

void leerMemoriaCompartida() {

}
