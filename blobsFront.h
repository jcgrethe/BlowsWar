/*blobsFront.h*/ 
#include "blobsBack.h"
#ifndef _blobsFront_h
#define _blobsFront_h




void pedirDimensiones(tipoJuego *);
void imprimirError(tipoJuego *);
void imprimirTablero(tipoJuego *);
int menuInicial();
void imprimirResultados(tipoJuego *);
int siNo(char*mensaje);
char * pedirComando();
#endif
