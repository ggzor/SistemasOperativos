#include <stdio.h>
#include <unistd.h>

int main() {
  int pid, b = 10, *a;
  a = &b;
  pid = fork();

  (*a)++;

  if (pid == 0) {
    while(1)
      printf("P.H. a = %p, *a = %5d, b = %d, &b = %p\n", a, (*a)++, b, &b);
  }
  else {
    while (1)
      printf("P.P. a = %p, *a = %5d, b = %d, &b = %p\n", a, --(*a), b, &b);
  }
  printf("Valor: %d\n", b);
}