#include "ParserOpciones.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int esNumero(char *cadena) {
  int i;
  int len = strlen(cadena);

  if (len > 1 && (cadena[0] == '+' || cadena[0] == '-'))
    i = 1;
  else
    i = 0;

  for (; i < len; i++)
    if (!('0' <= cadena[i] && cadena[i] <= '9'))
      return 0;

  return 1;
} 

int esBandera(char *cadena) {
  return !esNumero(cadena) && (strlen(cadena) == 2 && cadena[0] == '-');
}

int esComando(char *cadena) {
  return !esBandera(cadena) && !esNumero(cadena);
}

Parser inicializarParser(int argc, char **argv) {
  Parser parser;

  parser.argc = argc;
  parser.argv = argv;
  parser.indiceBandera = 0;
  parser.indiceComando = 0;
  parser.indiceEntero = 0;

  return parser;
}

int siguienteComando(Parser *parser, char **comando) {
  int indice = parser->indiceComando + 1;

  if (indice >= parser->argc) {
    return 0;
  } else {
    for (; indice < parser->argc; indice++) {
      if (esComando(parser->argv[indice])) {
        *comando = parser->argv[indice];
        parser->indiceComando = indice;
        return 1;
      }
    }

    parser->indiceComando = indice;
    return 0;
  }
}

int siguienteEntero(Parser *parser, int *entero) {
  int indice = parser->indiceEntero + 1;

  if (indice >= parser->argc) {
    return 0;
  } else {
    for (; indice < parser->argc; indice++) {
      if (esNumero(parser->argv[indice])) {
        *entero = atoi(parser->argv[indice]);
        parser->indiceEntero = indice;
        return 1;
      }
    }
  }

  return 0;
}

int siguienteEnteroOctal(Parser *parser, int *entero) {
  int indice = parser->indiceEntero + 1;

  if (indice >= parser->argc) {
    return 0;
  } else {
    for (; indice < parser->argc; indice++) {
      if (esNumero(parser->argv[indice])) {
        sscanf(parser->argv[indice], "%o", entero);
        parser->indiceEntero = indice;
        return 1;
      }
    }
  }

  return 0;
}

int siguienteBandera(Parser *parser, char *bandera) {
  int indice = parser->indiceBandera + 1;

  if (indice >= parser->argc) {
    return 0;
  } else {
    for (; indice < parser->argc; indice++) {
      if (esBandera(parser->argv[indice])) {
        *bandera = parser->argv[indice][1];
        parser->indiceBandera = indice;
        return 1;
      }
    }
  }
}