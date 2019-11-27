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

int main(int argc, char **argv) {
  int cd, i = 1;
  struct hostent *servidor;
  struct sockaddr_in cliente;
  int tamano, indice;
  char *buffer = NULL;

  if (argc != 3) {
    printf("Uso: ./Cliente <host> <puerto>\n");
  }

  if ((cd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("No se pudo crear el socket");
    exit(-1);
  }

  servidor = gethostbyname(argv[1]);
  if (servidor == NULL) {
    perror("Servidor inválido");
    exit(-1);
  }

  memchr(&cliente, 0, sizeof(cliente));
  cliente.sin_family = AF_INET;
  cliente.sin_port = htons(atoi(argv[2]));
  memcpy(&cliente.sin_addr.s_addr, servidor->h_addr, servidor->h_length);
  
  printf("Conectando con %s:%s\n", argv[1], argv[2]);
  if (connect(cd, (struct sockaddr *)&cliente, sizeof(cliente)) < 0) {
    perror("No se pudo conectar con el servidor");
    exit(-1);
  }

  printf("Conectado.\n\n");

  printf("La lista de canciones es: \n");

  do {
    read(cd, &tamano, sizeof(int));

    if (tamano != -1) {
      if (buffer != NULL)
        free(buffer);

      buffer = malloc(tamano);
      read(cd, buffer, tamano);

      printf("%d .- %s\n", i, buffer);
    }
  } while(tamano != -1);

  printf("¿Qué canción quieres escuchar? ");
  scanf("%d%*c", &indice);

  if (write(cd, &indice, sizeof(int)) <= 0) {
    printf("No se pudo conectar con el servidor.\n");
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

  printf("\nDescargando (%d bytes)...", tamano);
  
  if (buffer != NULL)
    free(buffer);
  
  buffer = malloc(tamano);
  read(cd, buffer, tamano);
  close(cd);
  printf("Terminado.\nDesconectando del servidor.\nReproduciendo...\n");

  int tarjetaAudio;
  int taza = 48000;
  int canales = 1;
  int formato = AFMT_S16_LE;
  int tiempo = 200;

  if ((tarjetaAudio = open("/dev/dsp1", O_WRONLY)) == -1) {
    perror("Error al abrir tarjeta de Audio");
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

  write(tarjetaAudio, buffer, taza * tiempo * (canales));

  if (ioctl(tarjetaAudio, SNDCTL_DSP_SYNC) == -1) {
    perror("Error al enviar pista");
    exit(-1);
  }

  free(buffer);
  close(tarjetaAudio);
  printf("Saliendo....\n");
}
