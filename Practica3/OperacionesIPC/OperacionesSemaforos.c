#include "Operaciones.h"
#include "Utilerias.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

int operacionesSemaforos(Parser *parser) {
  char *comando, bandera;
  char *comandos[] = { "crear", "establecer", "leer", "operar", "liberar" };
  int numSemCrear, semnum, valor, permisos = 0666, semid, incremento, i;
  union semun operaciones;
  struct semid_ds info;
  struct sembuf op, *ops;
  unsigned short *valores;

  if (siguienteComando(parser, &comando)) {
    switch (indiceDeCadena(comandos, 5, comando))
    {
    case 0:
      if (siguienteEntero(parser, &numSemCrear)) {
        siguienteEnteroOctal(parser, &permisos);

        semid = semget(IPC_PRIVATE, numSemCrear, IPC_CREAT | permisos);
        if (semid >= 0) {
          printf("Se creó exitosamente el semáforo con semid %d.\n", semid);
          return 0;
        } else {
          printf("Ocurrió un error al crear el semáforo.\n");
        }
      } else {
        printf("No se proporcionó el número de semáforos.\n");
      } 
      return -1;
    
    case 1:
      if (siguienteEntero(parser, &semid)) {
        operaciones.buf = &info;

        if (semctl(semid, 0, IPC_STAT, operaciones) >= 0) {
          if (siguienteEntero(parser, &semnum) && siguienteEntero(parser, &valor)) {
            if (semnum < info.sem_nsems) {
              operaciones.val = valor;

              if (semctl(semid, semnum, SETVAL, operaciones) >= 0) {
                printf("Se estableció el valor correctamente.\n");
                return 0;
              } else {
                printf("No se pudo establecer el valor del semáforo.\n");
              }
            } else {
              printf("Error, sólo existen %d semáforos en el conjunto.\n", info.sem_nsems);
            }
          } else {
            printf("No se proporcionaron los suficientes parámetros para establecer el semáforo.\n");
          }
        } else {
          printf("No existe un semáforo con el semid especificado.\n");
        }
      } else {
        printf("No se proporcionó el semid del semáforo.\n");
      }
      return -1;
    
    case 2:
      if (siguienteEntero(parser, &semid)) {
        operaciones.buf = &info;

        if (semctl(semid, 0, IPC_STAT, operaciones) >= 0) {
          if (siguienteEntero(parser, &semnum)) {
            if (semnum < info.sem_nsems) {
              if ((valor = semctl(semid, semnum, GETVAL)) >= 0) {
                printf("%d\n", valor);
                return 0;
              } else {
                printf("No se pudo obtener el valor del semáforo.\n");
              }              
            } else {
              printf("Error, sólo existen %d semáforos en el conjunto.\n", info.sem_nsems);
            }
          } else {
            valores = (unsigned short *) malloc(sizeof(unsigned short) * info.sem_nsems);
            operaciones.array = valores;

            if (semctl(semid, 0, GETALL, operaciones) >= 0) {
              for (i = 0; i < info.sem_nsems; i++)
                printf("%d ", valores[i]);
              printf("\n");

              free(valores);
              return 0;
            } else {
              printf("No se pudieron obtener los valores de los semáforos.\n");
            }

            free(valores);
          }
        } else {
          printf("No existe un semáforo con el semid especificado.\n");
        }
      } else {
        printf("No se proporcionó el semid del semáforo.\n");
      }    

      break;

    case 3:
      if (siguienteEntero(parser, &semid)) {
        operaciones.buf = &info;

        if (semctl(semid, 0, IPC_STAT, operaciones) >= 0) {
          if (siguienteEntero(parser, &incremento)) {
            if (siguienteEntero(parser, &semnum)) {
              if (semnum < info.sem_nsems) {
                op.sem_num = (unsigned short) semnum;
                op.sem_op = (short) incremento;
                op.sem_flg = 0;

                if (semop(semid, &op, 1) >= 0) {
                  printf("Se aplicó la operación correctamente.\n");
                  return 0;
                } else {
                  printf("No se pudo aplicar la operación al semáforo.\n");
                }
              } else {
                printf("Error, sólo existen %d semáforos en el conjunto.\n", info.sem_nsems);
              }
            } else {
              ops = malloc(sizeof(struct sembuf) * info.sem_nsems);

              for (i = 0; i < info.sem_nsems; i++) {
                ops[i].sem_num = (unsigned short) i;
                ops[i].sem_op = (short) incremento;
                ops[i].sem_flg = 0;
              }

              if (semop(semid, ops, info.sem_nsems) >= 0) {
                printf("Se aplicó la operación exitosamente a todos los semáforos.\n");
                free(ops);
                return 0;
              } else {
                printf("No se pudieron decrementar los valores de los semáforos.\n");
              }

              free(ops);
            }
          } else {
            printf("No se proporcionaron los suficientes parámetros para operar el semáforo.\n");
          }
        } else {
          printf("No existe un semáforo con el semid especificado.\n");
        }
      } else {
        printf("No se proporcionó el semid del semáforo.\n");
      }
      return -1;
    
    case 4:
      if (siguienteEntero(parser, &semid)) {
        operaciones.buf = &info;

        if (semctl(semid, 0, IPC_STAT, operaciones) >= 0) {
          if (semctl(semid, 0, IPC_RMID) >= 0) {
            printf("El semáforo se ha liberado exitosamente\n");
            return 0;
           } else {
            printf("Ocurrió un error al borrar el semáforo.\n");
          }
        } else {
          printf("No existe un semáforo con el semid especificado.\n");
        }
      } else {
        printf("No se proporcionó el semid del semáforo.\n");
      }
      return -1;

    default:
      printf("El comando \"%s\" no se ha reconocido.\nSólo puede ser uno de: crear, establecer, leer, operar o liberar.\n", comando);
      return -1;
    }
  } else {
    printf("No se proporcionó una operación para el comando sem.\n");
    return -1;
  }
}