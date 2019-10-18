#include "Operaciones.h"
#include "ParserOpciones.h"
#include "Utilerias.h"

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define TAMANO_MENSAJE 100

struct mensaje {
  long mtype;
  char mtext[TAMANO_MENSAJE];
};

void ayudaMensajes() {
  printf(
    "Uso: ./OperacionesIPC msg COMANDO [OPCIONES...]\n\n"
    "Comandos:\n"
    "  crear [PERMS=0666]\n"
    "    Crear una nueva cola de mensajes\n"
    "  \n"
    "  enviar MSGID TYPE\n"
    "    Enviar un mensaje desde con contenidos desde la entrada estándar\n"
    "  \n"
    "  recibir MSGID TYPE\n"
    "    Recibir un mensaje con el tipo especificado \n"
    "  \n"
    "  liberar MSGID\n"
    "    Liberar una cola de mensajes\n"
    "  \n"
    "  help\n"
    "    Muestra este mensaje de ayuda\n"
  );  
}

int operacionesMensajes(Parser *parser) {
  char *comando;
  char *comandos[] = { "crear", "enviar", "recibir", "liberar", "help"};
  
  int permisos = 0666, msqid, tipo;
  struct mensaje msj;
  struct msqid_ds info;

  if (siguienteComando(parser, &comando)) {
    switch (indiceDeCadena(comandos, 5, comando))
    {
    case 0:
      siguienteEntero(parser, &permisos);
      
      msqid = msgget(IPC_PRIVATE, IPC_CREAT | permisos);
      if (msqid >= 0) {
        printf("Se creó exitosamente la cola de mensajes con msqid %d.\n", msqid);
        return 0;
      }
      else {
        printf("Ocurrió un error al crear la cola de mensajes.\n");
      }
      return -1;
    
    case 1:
      if (siguienteEntero(parser, &msqid)) {
        if (msgctl(msqid, IPC_STAT, &info) >= 0) {
          if (siguienteEntero(parser, &tipo)) {
            msj.mtype = tipo;
            fgets(msj.mtext, TAMANO_MENSAJE, stdin);
                    
            if (msgsnd(msqid, &msj, sizeof(struct mensaje), 0) >= 0) {
              printf("El mensaje se envió correctamente.\n");
              return 0;
            } else {
              printf("No se pudo enviar el mensaje.\n");
            }          
          } else {
            printf("No se proporcionó el tipo de mensaje a enviar.\n");
          }
        } else {
          printf("No existe una cola de mensajes con el msqid especificado.\n");
        }
      } else {
        printf("No se proporcionó el msqid la cola de mensajes.\n");  
      }
      return -1;
    
    case 2:
      if (siguienteEntero(parser, &msqid)) {
        if (msgctl(msqid, IPC_STAT, &info) >= 0) {
          if (siguienteEntero(parser, &tipo)) {                   
            if (msgrcv(msqid, &msj, sizeof(struct mensaje), tipo, 0) >= 0) {
              printf("El mensaje recibido es:\n%s", msj.mtext);
              return 0;
            } else {
              printf("No se pudo recibir un mensaje con el tipo especificado.\n");
            }
          } else {
            printf("No se proporcionó el tipo de mensaje a enviar.\n");
          }
        } else {
          printf("No existe una cola de mensajes con el msqid especificado.\n");
        }
      } else {
        printf("No se proporcionó el msqid la cola de mensajes.\n");  
      }
      return -1;

    case 3:
      if (siguienteEntero(parser, &msqid)) {
        if (msgctl(msqid, IPC_STAT, &info) >= 0) {
          if (msgctl(msqid, IPC_RMID, 0) >= 0) {
            printf("La cola de mensajes se borró correctamente.\n");
            return 0;
          } else {
            printf("No se pudo borrar la cola de mensajes.\n");
          }
        } else {
          printf("No existe una cola de mensajes con el msqid especificado.\n");
        }
      } else {
        printf("No se proporcionó el msqid la cola de mensajes.\n");  
      }
      return -1;
    
    case 4:
      ayudaMensajes();
      return 0;
    default:
      printf("\e[31mERROR:\e[0m El comando \"%s\" no se ha reconocido.\n", comando);
      ayudaMensajes();
      return -1;
    }
  } else {
    printf("\e[31mERROR:\e[0m No se proporcionó una operación para el comando msg.\n");
    ayudaMensajes();
    return -1;
  }
}