#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int fd;
  char c;

  if (argc > 1) {
    fd = open(argv[1], O_RDONLY);

    if (fd > 0) {
      while (read(fd, &c, 1)) {
        write(1, &c, 1);
      }

      close(fd);
    } else {
      write(1, "No se pudo abrir el archivo.\n", sizeof("No se pudo abrir el archivo.\n"));
    }
  } else {
    write(1, "No hay argumentos.\n", sizeof("No hay argumentos.\n"));
  }
}