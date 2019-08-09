#include <stdio.h>
#include <unistd.h>

int main() {
  int pid = fork();

  if (pid == 0)
    while (1)
      write(1, "Proceso Hijo\n", sizeof("Proceso Hijo\n"));
  else
    while (1)
      write(1, "Proceso Padre\n", sizeof("Proceso Padre\n"));
}