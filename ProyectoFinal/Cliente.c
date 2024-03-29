#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/ioctl.h>
#include <sys/soundcard.h>

int abrirSocket(const char *host, const char *puerto) {
  int cd;
  struct hostent *servidor;
  struct sockaddr_in cliente;

  if ((cd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("No se pudo crear el socket");
    exit(-1);
  }

  servidor = gethostbyname(host);
  if (servidor == NULL) {
    perror("Servidor inválido");
    exit(-1);
  }

  memchr(&cliente, 0, sizeof(cliente));
  cliente.sin_family = AF_INET;
  cliente.sin_port = htons(atoi(puerto));
  memcpy(&cliente.sin_addr.s_addr, servidor->h_addr, servidor->h_length);

  if (connect(cd, (struct sockaddr *)&cliente, sizeof(cliente)) < 0) {
    perror("No se pudo conectar con el servidor");
    exit(-1);
  }

  return cd;
}

int main(int argc, char **argv) {
  int cd, i = 1;
  int tamano, indice;
  char *buffer = NULL;

  if (argc != 3) {
    printf("Uso: ./Cliente <host> <puerto>\n");
    exit(-1);
  }

  while (1) {
    printf("Conectando con %s:%s\n", argv[1], argv[2]);
    cd = abrirSocket(argv[1], argv[2]);
    printf("Conectado.\n\n");

    int accion = 0;
    printf("Solicitando canciones...\n");
    if (write(cd, &accion, sizeof(int)) <= 0) {
      perror("No se pudieron listar las canciones.");
      exit(-1);
    }
   
    i = 1;
    printf("La lista de canciones es: \n");

    do {
      read(cd, &tamano, sizeof(int));

      if (tamano != -1) {
        if (buffer != NULL)
          free(buffer);

        buffer = malloc(tamano);
        read(cd, buffer, tamano);

        printf("%d .- %s\n", i++, buffer);
      }
    } while(tamano != -1);

    printf("¿Qué canción quieres escuchar? ");
    scanf("%d%*c", &indice);

   
    if (fork() == 0) {
      cd = abrirSocket(argv[1], argv[2]);

      accion = 1;
      if (write(cd, &accion, sizeof(int)) <= 0 || write(cd, &indice, sizeof(int)) <= 0) {
        perror("No se descargar la canción.");
        exit(-1);
      }

      if (read(cd, &tamano, sizeof(int)) <= 0) {
        printf("No se pudo leer el tamaño del archivo.");
        exit(-1);
      }

      if (tamano == -1) {
        printf("No se encontró el archivo con índice %d.\n", indice);
        exit(-1);
      }
     
      if (buffer != NULL)
        free(buffer);
      
      buffer = malloc(tamano);
      int recibido = 0;

      printf("Descargando canción.\n");
      while (recibido < tamano) {
        int leidos = read(cd, buffer + recibido, tamano - recibido);

        if (leidos <= 0) {
          printf("No se pudo recibir el archivo completo.\n");
          exit(-1);
        }

        recibido += leidos;
      }
      printf("Descarga finalizada.\n");
      close(cd);

      int tarjetaAudio;
      int taza = 48000;
      int canales = 2;
      int formato = AFMT_S16_LE;

      if ((tarjetaAudio = open("/dev/dsp1", O_WRONLY)) == -1) {
        perror("\nError al abrir tarjeta de Audio\n");
        exit(-1);
      }

      if (ioctl(tarjetaAudio, SNDCTL_DSP_STEREO, &canales) == -1) {
        perror("Error al configurar canales de audio\n");
        exit(-1);
      }
      if (ioctl(tarjetaAudio, SNDCTL_DSP_SETFMT, &formato) == -1) {
        perror("Error al leer el formato de pista\n");
        exit(-1);
      }
      if (ioctl(tarjetaAudio, SNDCTL_DSP_SPEED, &taza) == -1) {
        perror("Error al configurar la taza de muestreo\n");
        exit(-1);
      }

      write(tarjetaAudio, buffer, tamano);

      free(buffer);
      close(tarjetaAudio);
      exit(-1);
    }

    close(cd);
  }
}
