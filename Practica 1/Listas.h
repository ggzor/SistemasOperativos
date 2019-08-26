#ifndef _LISTAS_H_
#define _LISTAS_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct Nodo
{
  bool marcado;
  int pid;
  char tty[6];
  char stat[6];
  char time[6];
  char cmd[31];
  struct Nodo *siguiente;
} Nodo;

Nodo *crearNodo();
void liberarLista(Nodo *lista);

Nodo *buscar(Nodo *lista, int pid);
Nodo *insertar(Nodo *lista, Nodo *nuevo);
Nodo *eliminarNoMarcados(Nodo *lista, Nodo **eliminados);

#endif