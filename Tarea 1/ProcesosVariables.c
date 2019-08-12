#include <stdio.h>
#include <unistd.h>

int main() {
  int pid, a = 10;
  pid = fork();

  if (pid == 0)
    while (1)
      printf("P.H. %d\n", a++);
  else
    while (1)
      printf("P.P. %d\n", --a);
}