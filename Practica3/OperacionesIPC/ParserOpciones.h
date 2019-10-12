#ifndef __PARSER_OPCIONES_H__
#define __PARSER_OPCIONES_H__

typedef struct {
  char **argv;
  int argc;

  int indiceEntero;
  int indiceComando;
} Parser;

Parser inicializarParser(int argc, char **argv);
int siguienteEntero(Parser *parser, int *entero);
int siguienteComando(Parser *parser, char **comando);
int siguienteBandera(Parser *parser, char **bandera);
int siguienteArgumentoEntero(Parser *parser, char **nombre, int *valor);

#endif