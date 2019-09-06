#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>

void mostrarArchivo(struct dirent *archivo);

int main(int argc, char **argv) {
  DIR *directorio;
  struct dirent *archivo;

  if (argc > 1) {
    directorio = opendir(argv[1]);

    if (directorio != NULL) {
      printf("%-12s %8s  %-11s %-10s %7s %-10s %-5s %s\n", "NOMBRE", "INODO", "TIPO", "PROPIETARIO", "TAMAÑO", "FECHA", "HORA", "PERMISOS");

      while (archivo = readdir(directorio))
        mostrarArchivo(archivo);

      closedir(directorio);
    } else {
      printf("Error: No se pudo abrir el directorio.\n");
    }
  } else {
    printf("Error: No se proporcionó un directorio para mostrar.\n");
  }
}

void imprimirPermisos(__mode_t modo) {
  static int banderas[] = { 
    S_IRUSR, S_IWUSR, S_IXUSR,
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH
   };
  int i;

  for (i = 0; i < 9; i++) {
    if ((modo & banderas[i]) != 0) {
      switch (i % 3)
      {
      case 0:
        putchar('r');
        break;
      case 1:
        putchar('w');
        break;
      case 2:
        putchar('x');
        break;
      }
    } else {
      putchar('-');
    }
  }
}

void mostrarArchivo(struct dirent *archivo) {
  struct stat st;
  struct passwd *usuario;
  struct tm *fecha;

  // Recuperar información del archivo
  stat(archivo->d_name, &st);
  // Obtener nombre del propietario
  usuario = getpwuid(st.st_uid);
  // Obtener fecha en distintos campos
  fecha = localtime(&st.st_mtime);

  // Imprimir nombre
  printf("%-12s ", archivo->d_name);

  // Imprimir el inodo
  printf("%8d  ", st.st_ino);

  // Imprimir el tipo
  if (archivo->d_type == 8)
    printf("%-11s ", "Carpeta");
  else if (archivo->d_type == 4)
    printf("%-11s ", "Archivo");
  else
    printf("%-11s ", "Desconocido");
  
  // Imprimir el propietario
  printf("%-10s ", usuario->pw_name);

  //Imprimir el tamaño 
  printf("%7d " , st.st_size);

  // Imprimir la fecha
  printf("%02d/%02d/%4d ", fecha->tm_mday, fecha->tm_mon, fecha->tm_year + 1900);

  // Imprimir la hora
  // color(VERDE, printf("%02d:%02d ", fecha->tm_hour, fecha->tm_min));
  printf("%02d:%02d ", fecha->tm_hour, fecha->tm_min);

  // Imprimir permisos
  imprimirPermisos(st.st_mode);

  printf("\n");  
}
