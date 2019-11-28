#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int listarDirectorio(int id, const char *nombreDirectorio, const int fd) {
  DIR *directorio;
  struct dirent *archivo;
  int tamano;

  directorio = opendir(nombreDirectorio);
  printf("[%d] Listando canciones.\n", id);

  while (archivo = readdir(directorio)) {
    // Ignorar directorio actual y anterior
    if (strncmp(archivo->d_name, ".", 2) != 0 && strncmp(archivo->d_name, "..", 3) != 0) {
      tamano = strlen(archivo->d_name) + 1;
      printf("[%d] Enviando tamaño de cadena (%d)\n", id, tamano);
      if (write(fd, &tamano, sizeof(int)) <= 0) {
        closedir(directorio);
        return -1;
      }

      if (write(fd, archivo->d_name, tamano) <= 0) {
        closedir(directorio);
        return -1;
      }
    }
  }

  tamano = -1;
  if (write(fd, &tamano, sizeof(int)) <= 0) {
    closedir(directorio);
    return -1;
  }

  closedir(directorio);
  printf("[%d] Listado finalizado.\n", id);
  return 0;
}

void enviarCancion(const int id, const char *nombreDirectorio, const int indice, const int fd) {
  int i = 1;
  DIR *directorio;
  struct stat st;
  struct dirent *archivo;
  int tamano, enviado = 0;
  char *buffer;

  char rutaCompleta[1000];
  strcpy(rutaCompleta, nombreDirectorio);

  directorio = opendir(nombreDirectorio);

  while (archivo = readdir(directorio)) {
    if (strncmp(archivo->d_name, ".", 2) != 0 && strncmp(archivo->d_name, "..", 3) != 0) {
      if (i == indice) {
        printf("[%d] Archivo con índice %d encontrado.\n", id, indice);

        strcpy(rutaCompleta + strlen(rutaCompleta), archivo->d_name);
        stat(rutaCompleta, &st);
        tamano = st.st_size;
        buffer = malloc(tamano);

        printf("[%d] Cargando archivo %s (%d bytes).\n", id, archivo->d_name, tamano);
        {
          int archivo = open(rutaCompleta, O_RDONLY);
          read(archivo, buffer, tamano);
          close(archivo);
        }

        printf("[%d] Archivo cargado en buffer. Enviando...\n", id);
        write(fd, &tamano, sizeof(int));

        int progresoAnterior = 0;
      
        while (enviado < tamano) {
          int escritos = write(fd, buffer + enviado, tamano - enviado);
      
          if (escritos <= 0) {
            printf("[%d] No se pudo enviar el archivo completo.\n");
            exit(-1);
          }
      
          enviado += escritos;
      
          int progreso = enviado * 100 / tamano;
          if (progreso % 5 == 0 && progreso > progresoAnterior)
            printf("[%d] Progreso %02d%\n", id, progreso);
          progresoAnterior = progreso;
        }

	printf("[%d] Enviados %d bytes\n", id, enviado);
        free(buffer);
        printf("[%d] Archivo %s enviado.\n", id, archivo->d_name);

        return;
      }

      i++;
    }
  }

  printf("[%d] Archivo con índice %d no encontrado.\n", id, indice);
  tamano = -1;
  write(fd, &tamano, sizeof(int));
  closedir(directorio);
}

int main(int argc, char **argv) {
  int sd;
  int idActual = 0, esPadre = 1;
  struct sockaddr_in servidor, cliente;
  socklen_t longitudCliente = sizeof(cliente);
  char buff[100];

  if (argc != 3) {
    printf("Uso: ./Servidor <carpeta_musica> <puerto>\n");
    exit(-1);
  }

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("No se pudo crear el socket");
    exit(-1);
  }

  memchr(&servidor, 0, sizeof(servidor));
  servidor.sin_family = AF_INET;
  servidor.sin_port = htons(atoi(argv[2]));
  servidor.sin_addr.s_addr = INADDR_ANY;
  
  if (bind(sd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
    perror("No se pudo asignar el puerto");
    exit(-1);
  }

  listen(sd, 3);
  printf("Servidor a la escucha.\n");

  while (esPadre) {
    int cd = accept(sd, (struct sockaddr *)&cliente, &longitudCliente);
    printf("[%d] Conectado con %s:%d\n", ++idActual, inet_ntoa(cliente.sin_addr), htons(cliente.sin_port));

    if (fork() == 0) {
      int indice;
      esPadre = 0;

      if (listarDirectorio(idActual, argv[1], cd) == -1) {
        printf("[%d] No se pudo completar el listado del directorio.\n", idActual);
        printf("[%d] Sesión terminada.\n", idActual);
        exit(1);
      }

      if (read(cd, &indice, sizeof(int)) <= 0) {
        printf("[%d] No se pudo leer el índice.\n", idActual);
        printf("[%d] Sesión terminada.\n", idActual);
        exit(1);
      }

      enviarCancion(idActual, argv[1], indice, cd);
      close(cd);
      printf("[%d] Sesión terminada.\n", idActual);
    }
  }

  close(sd);
}
