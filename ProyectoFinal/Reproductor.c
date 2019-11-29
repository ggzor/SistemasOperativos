#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/soundcard.h>
#define tiempo 1000

int main(int argc, char *argv[]) {
  int archivo;
  int tarjetaAudio;

  char *buffer;
  int taza = 48000;
  int canales = 1;
  int formato = AFMT_S16_LE;

  char *nombreArchivo = argv[1];

  printf("\n->Repoduciendo: %s \n", nombreArchivo);

  if ((tarjetaAudio = open("/dev/dsp1", O_WRONLY)) == -1) {
    perror("Error al abrir tarjeta de Audio");
    exit(-1);
  }

  if ((archivo = open(nombreArchivo, O_RDONLY)) == -1) {
    perror("Error al abrir pista de audio\n");
    exit(-1);
  }

  buffer = malloc(taza * tiempo * (canales + 1));
  read(archivo, buffer, taza * tiempo * (canales + 1));

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
  close(archivo);
  close(tarjetaAudio);
  printf("Saliendo....\n");

  return 0;
}
