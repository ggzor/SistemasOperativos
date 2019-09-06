#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "Listas.h"

Nodo *actualizar(Nodo *lista, char *contenido, int tamano, int *hayCambios);
void imprimirLista(Nodo *lista);

int main() {
  int descriptor, tamano, leidos, hayCambios;
  char *contenido;
  Nodo *lista = NULL;

  struct stat st;

  if (fork() == 0) {
    while (1)
      system("ps a > ps.txt");
  } else {
    while (1) {
      // Esperar hasta que exista el archivo
      while ((descriptor = open("ps.txt", O_RDONLY)) < 0);

      // Obtener el tamaño del archivo
      fstat(descriptor, &st);
      tamano = st.st_size;

      if (tamano > 0) {
        // Crear buffer para el contenido del archivo
        contenido = malloc(tamano + 1);
        contenido[tamano] = '\0';

        // Leer archivo y guardar en el buffer
        leidos = read(descriptor, contenido, tamano);

        if (tamano == leidos) {
          // Actualizar la lista de procesos e imprimirlos
          lista = actualizar(lista, contenido, tamano, &hayCambios);

          if (hayCambios)
            imprimirLista(lista);
        }

        free(contenido);
      }

      close(descriptor);
    }
  }
}

Nodo *actualizar(Nodo *lista, char *contenido, int tamano, int *hayCambios) {
  Nodo *eliminados = NULL, *nuevo = NULL, *nodo;
  char *token, *delimitador = "\n";
  int pid, primero = 1, insertado = 0;

  *hayCambios = 0;

  // Dividir la cadena por saltos de línea
  token = strtok(contenido, delimitador);

  while (token != NULL) {
    // Para saltar la primera línea
    if (primero) {
      primero = 0;
    } else {
      sscanf(token, "%d", &pid);

      nodo = buscar(lista, pid);

      if (nodo != NULL) {
        // Marcar, para que no sea eliminado
        nodo->marcado = true;

        // Actualizar campo TIME
        sscanf(token, "%*d %*5s %*5s %5s", nodo->time);
      } else {
        nuevo = crearNodo();
        
        // Leer los datos del nuevo nodo
        sscanf(token, "%d %5s %5s %5s %30[^\n]", &nuevo->pid, nuevo->tty, nuevo->stat, nuevo->time, nuevo->cmd);
        nuevo->marcado = 1;

        lista = insertar(lista, nuevo);

        if (!insertado)
          printf("Nuevos:");
        printf(" %d", pid);
        insertado = 1;

        *hayCambios = 1;
      }
    }

    token = strtok(NULL, delimitador);
  }

  lista = eliminarNoMarcados(lista, &eliminados);

  if (insertado)
    printf("\n");

  if (eliminados != NULL) {
    printf("Removidos:");
    while (eliminados != NULL) {
      *hayCambios = 1;
      printf(" %d", eliminados->pid);
      eliminados = eliminados->siguiente;
    }
    printf("\n");

    liberarLista(eliminados);
  }

  return lista;
}

void imprimirLista(Nodo *lista) {
  int contador = 1;

  printf("%2s  %6s  %5s  %5s  %5s  %s\n", "N", "PID", "TTY", "STAT", "TIME", "CMD");
  while (lista != NULL)
  {
    printf("%2d. \e[33m%6d\e[0m  \e[36m%5s\e[0m  \e[31m%5s\e[0m  \e[32m%5s\e[0m  %s\n",
            contador, lista->pid, lista->tty, lista->stat, lista->time, lista->cmd);

    contador++;
    lista = lista->siguiente;
  }

  printf("\n");
}