#include <stdio.h>
#include <unistd.h>

int main() {
  int pid = fork();

  while (1)
    if (pid == 0) {
      write(1, "Proceso Hijo\n", sizeof("Proceso Hijo\n"));
      exit(0);
    } else {
      write(1, "Proceso Padre\n", sizeof("Proceso Padre\n"));
    }
}