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
  int sd;
  struct sockaddr_in servidor, cliente;
  socklen_t longitudCliente = sizeof(cliente);
  char buff[100];

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("No se pudo crear el socket");
    exit(-1);
  }

  memchr(&servidor, 0, sizeof(servidor));
  servidor.sin_family = AF_INET;
  servidor.sin_port = htons(atoi(argv[1]));
  servidor.sin_addr.s_addr = INADDR_ANY;
  
  if (bind(sd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
    perror("No se pudo asignar el puerto");
    exit(-1);
  }

  listen(sd, 3);
  printf("Servidor a la escucha\n");

  while (1) {
    int cd = accept(sd, (struct sockaddr *)&cliente, &longitudCliente);
    printf("Conectado con %s:%d\n", inet_ntoa(cliente.sin_addr), htons(cliente.sin_port));

    if (read(cd, buff, 100) < 0) {
      perror("No se pudieron recibir datos del cliente\n");
      close(cd);
    }

    printf("Se recibió: %s\n", buff);
    close(cd);
  }

  close(sd);
}
