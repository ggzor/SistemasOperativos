#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  DIR *directorio = opendir(argv[1]);
  struct dirent *archivo;
  struct stat st;
  
  printf("%-10s   %6s   %4s\n", "NOMBRE", "TAMAÑO", "TIPO");
  while (archivo = readdir(directorio)) {
    stat(archivo->d_name, &st);
    printf("%-10s   %6d   %4d\n", archivo->d_name, st.st_size, archivo->d_type);
  }

  closedir(directorio);
}