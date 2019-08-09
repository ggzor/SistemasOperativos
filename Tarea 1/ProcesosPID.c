#include <stdio.h>
#include <unistd.h>

int main() {
  int pid = fork();
  int ppid = getppid();

  while (1) {
    if (pid == 0) {
      printf("Proceso Hijo  - pid = %6d, ppid = %6d\n", getpid(), ppid);
    } else {
      printf("Proceso Padre - pid = %6d, ppid = %6d\n", getpid(), ppid);
    }
  }
}