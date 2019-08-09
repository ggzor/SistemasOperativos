#include <stdio.h>
#include <unistd.h>

int main() {
  int pid;

  if ((pid = fork()) == 0) {
    write(1, "Proceso Hijo\n", sizeof("Proceso Hijo\n"));
  } else {
    write(1, "Proceso Padre\n", sizeof("Proceso Padre\n"));
  }
}