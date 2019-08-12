#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int pid, *a;
  a = (int *) malloc(sizeof(int));
  pid = fork();

  if (pid == 0)
    while (1)
      printf("P.H. %d\n", (*a)++);
  else
    while (1)
      printf("P.P. %d\n", --(*a));
}