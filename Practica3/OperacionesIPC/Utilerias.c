#include "Utilerias.h"

int indiceDeCadena(char **a, int n, const char *valor) {
  int i;
  int lvalor = strlen(valor);

  for (i = 0; i < n; i++)
    if (strncmp(a[i], valor, lvalor) == 0)
      return i;

  return -1;
}