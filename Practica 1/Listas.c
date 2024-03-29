#include "Listas.h"

Nodo *crearNodo() {
  Nodo *aux = malloc(sizeof(struct Nodo));

  aux->marcado = true;
  aux->pid = 0;
  aux->siguiente = NULL;

  return aux;
}

void liberarLista(Nodo *lista) {
  Nodo *aux = lista;

  while (aux != NULL) {
    lista = lista->siguiente;
    free(aux);
    aux = lista;
  }
}

Nodo *buscar(Nodo *lista, int pid) {
  while (lista != NULL) {
    if (lista->pid == pid) {
      return lista;
    }

    lista = lista->siguiente;
  }

  return NULL;
}

Nodo *insertar(Nodo *lista, Nodo *nuevo) {
  Nodo *aux = lista;

  if (aux == NULL) {
    return nuevo;
  } else {
    while (aux->siguiente != NULL) {
      aux = aux->siguiente;
    }
    aux->siguiente = nuevo;

    return lista;
  }
}

Nodo *eliminarNoMarcados(Nodo *lista, Nodo **eliminados) {
  Nodo *anterior, *actual, *siguiente;

  *eliminados = NULL;

  // Eliminar los primeros que no estén marcados, posiblemente todos.
  actual = lista;
  while (actual != NULL && !actual->marcado) {
    siguiente = actual->siguiente;

    // Agregar el actual a los eliminados
    actual->siguiente = *eliminados;
    *eliminados = actual;
    
    actual = siguiente;
  }

  // La lista está vacía o no hay elementos marcados
  if (actual == NULL)
    return NULL;


  // Buscar los demás nodos que no están marcados
  lista = anterior = actual;
  actual = actual->siguiente;

  // Desmarcar anterior
  anterior->marcado = false;

  while (actual != NULL) {
    if (actual->marcado) {
      // Debemos de desmarcarlo, una vez que ya se eliminaron los marcados.
      actual->marcado = false;

      anterior = actual;
      actual = actual->siguiente;
    } else {
      siguiente = actual->siguiente;

      actual->siguiente = *eliminados;
      *eliminados = actual;

      actual = siguiente;
      // Actualizar apuntador del anterior
      anterior->siguiente = actual;
    }
  }

  return lista;
}
