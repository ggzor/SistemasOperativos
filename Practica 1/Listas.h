#ifndef _LISTAS_H_
#define _LISTAS_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct Nodo
{
  bool marcado;
  int pid;
  struct Nodo *siguiente;
} Nodo;

Nodo *crearNodo();

/**
 * Función que verifica si existe un nodo con el PID especificado,
 * la función es necesaria para evitar la creación de un nodo al insertar.
 */
bool existe(Nodo *lista, int pid);
void marcar(Nodo *lista, int pid);

Nodo *insertar(Nodo *lista, Nodo *nuevo);
Nodo *eliminarNoMarcados(Nodo *lista, Nodo **eliminados);

#endif