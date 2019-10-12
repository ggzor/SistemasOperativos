#include "ParserOpciones.h"
#include <string.h>

int esBandera(char *cadena) {
  return strlen(cadena) == 2 && cadena[0] == '-';
}

int esNumero(char *cadena) {
  int i;
  int len = strlen(cadena);

  for (i = 0; i < len; i++)
    if (!('0' <= cadena[i] && cadena[i] <= '9'))
      return 0;

  return 1;
} 

int esComando(char *cadena) {
  return !esBandera(cadena) && !esNumero(cadena);
}

Parser inicializarParser(int argc, char **argv) {
  Parser parser;

  parser.argc = argc;
  parser.argv = argv;
  parser.indiceEntero = 1;
  parser.indiceComando = 0;

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
