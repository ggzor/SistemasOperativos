#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv) {
  int cd;
  struct hostent *servidor;
  struct sockaddr_in cliente;
  char cadena[] = "Hola y adiós";

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

  printf("Conectado. Enviando datos.\n");
  write(cd, cadena, sizeof(cadena));

  close(cd);
}
