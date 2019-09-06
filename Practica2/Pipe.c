#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  DIR *directorio = opendir(argv[1]);
  struct dirent *archivo;
  struct stat st;

  int c;

  int fd[2];
  pipe(fd);

  if (fork() == 0) {
    while (read(fd[0], &c, 1)) {
      write(1, &c, 1);
    }
  } else {
    while (archivo = readdir(directorio)) {
      stat(archivo->d_name, &st);

      write(fd[1], archivo->d_name, strlen(archivo->d_name));
      c = '\n';
      write(fd[1], &c, 1);
    }

    closedir(directorio);
  }
}