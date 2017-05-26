/*blobsBack.h*/
#ifndef _blobsBack_h
#define _blobsBack_h

typedef struct
{
	int **matriz;
	int ancho;
	int alto;
}tablero;
typedef struct
{
	int flagMaquina;
	int cantidadFichas;
}jugador;
typedef struct
{
	char * nombreDeArchivo;
}archivo;
typedef struct 
{
	int codigoError;
}error;
typedef struct
{
	int turno;
	int modo;
	jugador jugadorUno;
	jugador jugadorDos;
	tablero tablero;
	archivo archivo;
	error error;
}tipoJuego;

int buscarAtaque(tipoJuego *, int *);
int fichasAfectadas(tipoJuego *, int, int);
int azar();
int analizarComando(tipoJuego *, int *,char *);
int crearTablero(tipoJuego *);
int buscarMovimiento(tipoJuego *, int *);
int hallarCero(tipoJuego *, int, int, int *);
int ganador(tipoJuego *);
void atacar(tipoJuego *, int *);
int buscarMovimientoSimple(tipoJuego *, int *);
int decidirMenu(tipoJuego *);
int abrirPartida(tipoJuego *, char *);
int guardarPartida(tipoJuego *);
void rellenar(tipoJuego *);
void movimientoDeMaquina(tipoJuego *, int *);
int validarMovimiento(tipoJuego * , int * );
void efectuarMovimiento(tipoJuego *, int * );
void resetear(tipoJuego * );
int ataqueOptimo(tipoJuego *, int , int, int * , int);
#endif
