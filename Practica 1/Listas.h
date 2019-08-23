#ifndef _LISTAS_H_
#define _LISTAS_H_

typedef struct Nodo
{
  bool marcado;
  int pid;
  struct Nodo *siguiente;
} Nodo;




#endif