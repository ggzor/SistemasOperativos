#include "Listas.h"
#include <stdio.h>

Nodo *actualizar(Nodo *lista, int *pids, int lpids) {
  Nodo *eliminados = NULL, *nuevo = NULL;
  int i;

  printf("Nuevos:");
  for (i = 0; i < lpids; i++) {
    if (existe(lista, pids[i])) {
      marcar(lista, pids[i]);
    } else {
      printf(" %d", pids[i]);
      nuevo = crearNodo();
      nuevo->pid = pids[i];
      nuevo->marcado = true;
      lista = insertar(lista, nuevo);
    }
  }
  printf("\n");

  lista = eliminarNoMarcados(lista, &eliminados);

  printf("Removidos:");
  while (eliminados != NULL) {
    printf(" %d", eliminados->pid);
    eliminados = eliminados->siguiente;
  }
  printf("\n");

  return lista;
}

void imprimirLista(Nodo *lista) {
  printf("Valores en la lista:");
  while (lista != NULL) {
    printf(" %d", lista->pid);

    lista = lista->siguiente;
  }
  printf("\n\n");
}

int main() {
  Nodo *lista = NULL;
  int i;
  bool rechazado;

  int pids1[] = {1, 2, 3, 4, 5};
  int lpids1 = 5;

  int pids2[] = {1, 3, 4, 6, 7};
  int lpids2 = 5;

  int pids3[] = {};
  int lpids3 = 0;

  lista = actualizar(lista, pids1, lpids1);
  imprimirLista(lista);

  lista = actualizar(lista, pids2, lpids2);
  imprimirLista(lista);

  lista = actualizar(lista, pids3, lpids3);
  imprimirLista(lista);
}

/*
Nuevos: 1 2 3 4 5
Removidos: 
Valores en la lista: 1 2 3 4 5

Nuevos: 6 7
Removidos: 5 2
Valores en la lista: 1 3 4 6 7

Nuevos: 
Removidos: 7 6 4 3 1
Valores en la lista: 
*/
