#ifndef __PARSER_OPCIONES_H__
#define __PARSER_OPCIONES_H__

typedef struct {
  char **argv;
  int argc;

  int indiceBandera;
  int indiceComando;
  int indiceEntero;
} Parser;

Parser inicializarParser(int argc, char **argv);
int siguienteBandera(Parser *parser, char *bandera);
int siguienteComando(Parser *parser, char **comando);
int siguienteEnteroOctal(Parser *parser, int *entero);
int siguienteEntero(Parser *parser, int *entero);

#endif