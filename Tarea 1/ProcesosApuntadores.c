#include <stdio.h>
#include <unistd.h>

int main() {
  int pid, b = 10, *a;
  a = &b;
  pid = fork();

  if (pid == 0)
    while (1)
      printf("P.H. %d\n", (*a)++);
  else
    while (1)
      printf("P.P. %d\n", --(*a));
}