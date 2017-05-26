/*backEnd.c*/

#include <stdlib.h>
#include "getnum.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define ON 1
#define OFF 0
#include "blobsBack.h"





int buscarMovimientoSimple(tipoJuego * m, int * movimiento)
{
	int i, j, k, l, arr, abj, izq, der, flag = 0;
	for(i = 0; i < (*m).tablero.alto && flag == 0; i++)
	{
		for(j = 0; j < (*m).tablero.ancho && flag == 0; j++)
		{
			if((*m).turno == (*m).tablero.matriz[i][j])
			{
				izq = (j == 0)? 0 : 1;
				der = (((*m).tablero.ancho - 1) == j)? 0 : 1;
				arr = (i == 0)? 0 : 1;
				abj = (((*m).tablero.alto - 1) == i)? 0 : 1;
				for(k = i - arr; flag == 0 && k < i + abj; k++)
				{
					for(l = j - izq; flag == 0 && l < j + der; l++)
					{
						if((*m).tablero.matriz[k][l] == 0)
						{
							flag = 1;
							movimiento[0] = i;
							movimiento[1] = j;
							movimiento[2] = k;
							movimiento[3] = l;
						}
					}
				}
			}		
		}
	}
	return flag;	
}
void resetear(tipoJuego * Partida)
	/*hace free de todas las estructuras una vez terminada la partida*/
{	int x;
	for (x=0;x<Partida->tablero.alto;x++)
		free(Partida->tablero.matriz[x]);
	free(Partida->tablero.matriz);
	free(Partida->archivo.nombreDeArchivo);
	Partida->modo = 0;
	Partida->jugadorUno.cantidadFichas = 2;
	Partida->jugadorDos.cantidadFichas = 2;
}
void movimientoDeMaquina(tipoJuego * Partida, int * movimiento)
{
	if(!buscarAtaque(Partida, movimiento))
	{		
		buscarMovimientoSimple(Partida, movimiento);
	}
}

/*buscamos el mejor ataque y si hay varios uno rand entre todos esos*/
int buscarAtaque(tipoJuego * Partida, int * movimiento)
{
	int i,j,flag = 0;
	for(i=0;i<Partida->tablero.alto;i++)	
		for(j=0;j<Partida->tablero.ancho;j++)
			if(Partida->turno == Partida->tablero.matriz[i][j])
				flag = ataqueOptimo(Partida, i, j, movimiento, flag);
	return flag;
}
int ataqueOptimo(tipoJuego * Partida, int fil, int col, int * movimiento, int max)
{
	int izq, der, arr, abj, k, l, cant;
	izq = (col <= 2)? col : 2;
	der = ((*Partida).tablero.ancho - col - 1 <= 2)? (*Partida).tablero.ancho - col - 1 : 2;
	arr = (fil <= 2)? fil : 2;
	abj = ((*Partida).tablero.alto - fil - 1 <= 2)? (*Partida).tablero.alto - fil - 1 : 2;
	for(k = fil - arr; k <= fil + abj;k++)
		for(l=col - izq;l <= col + der;l++)
			if(Partida->tablero.matriz[k][l] == 0)
			{
				cant = fichasAfectadas(Partida, k, l);
				if(cant > max)
				{
					max = cant;
					movimiento[0] = fil;
					movimiento[1] = col;
					movimiento[2] = k;
					movimiento[3] = l;
				}else if(max == cant)
					if(azar() > 0)
					{
						movimiento[0] = fil;
						movimiento[1] = col;
						movimiento[2] = k;
						movimiento[3] = l;
					}
					
			}
	return max;
				
}
int fichasAfectadas(tipoJuego * Partida, int fil, int col)
{
	int izq, der, arr, abj, k, l, cant = 0;
	izq = (col == 0)? 0 : 1;
	der = ((Partida->tablero.ancho - 1) == col)? 0 : 1;
	arr = (fil == 0)? 0 : 1;
	abj = ((Partida->tablero.alto - 1) == fil)? 0 : 1;
	for(k = fil - arr; k <= fil + abj;k++)
		for(l=col - izq;l <= col + der;l++)
			if(Partida->tablero.matriz[k][l] == ((-1)*Partida->turno))
				cant++;
	return cant;		
}
int
guardarPartida(tipoJuego * Partida)
{
    FILE * archivo;
    int i, j, escritos=0, turnoParaArchivo;
    archivo = fopen((Partida->archivo.nombreDeArchivo) ,"w");
	
    if (archivo != NULL)
    {	
        escritos += fwrite( &(Partida->jugadorDos.flagMaquina), sizeof(int), 1, archivo); /*punto 1 - tipo de juego*/
        if (Partida->turno == -1)
            turnoParaArchivo=2;
        else if (Partida->turno == 1)
            turnoParaArchivo=1;
	escritos += fwrite( &(turnoParaArchivo) , sizeof(int), 1, archivo);    
	escritos += fwrite( &(Partida->tablero.alto) , sizeof(int), 1, archivo); /*alto del tablero*/
        escritos += fwrite( &(Partida->tablero.ancho) , sizeof(int), 1, archivo); /*ancho del tablero*/
        escritos += fwrite( &(Partida->jugadorUno.cantidadFichas) , sizeof(int), 1, archivo); /*manchas jugador 1*/
        escritos += fwrite( &(Partida->jugadorDos.cantidadFichas) , sizeof(int), 1, archivo); /*manchas jugador 2*/
        for(i=0 ; i < Partida->tablero.alto ; i++)
        for(j=0 ; j < Partida->tablero.ancho ; j++)
            switch (Partida->tablero.matriz[i][j]){
                case 1: escritos += fwrite( ("A") , 1, 1, archivo);
                        break;
                case -1:escritos += fwrite( ("Z") , 1, 1, archivo);    /*matriz*/
                        break;
                default:escritos += fwrite( ("0") , 1, 1, archivo);
            }
	fclose(archivo);
        if(escritos == 6+(Partida->tablero.ancho * Partida->tablero.alto))
            return 1;
        else
            return 0;
    }else
        return 0;
}

int 
abrirPartida(tipoJuego * Partida, char * nomArch)
{
    FILE * archivo = NULL;
    int i, j, escritos=0, turnoParaArchivo=0;
    char caracter;
	Partida->archivo.nombreDeArchivo = nomArch;
    archivo = fopen(nomArch ,"r");
    if (archivo != NULL)
    {
        escritos += fread( &(Partida->jugadorDos.flagMaquina), sizeof(int), 1, archivo); /*punto 1 - tipo de juego*/
	if(Partida->jugadorDos.flagMaquina)
		Partida->modo = 2;
	else
		Partida->modo = 1;
	escritos += fread( &(turnoParaArchivo), sizeof(int), 1, archivo);
        if (turnoParaArchivo == 1)
            Partida->turno = 1;
        else
            Partida->turno = -1;
        escritos += fread( &(Partida->tablero.alto) , sizeof(int), 1, archivo); /*alto del tablero*/
        escritos += fread( &(Partida->tablero.ancho) , sizeof(int), 1, archivo); /*ancho del tablero*/
	crearTablero(Partida);
        escritos += fread( &(Partida->jugadorUno.cantidadFichas) , sizeof(int), 1, archivo); /*manchas jugador 1*/
        escritos += fread( &(Partida->jugadorDos.cantidadFichas) , sizeof(int), 1, archivo); /*manchas jugador 2*/
        for(i=0 ; i < Partida->tablero.alto ; i++)
            for(j=0 ; j < Partida->tablero.ancho ; j++){
                if (fread( &caracter , 1, 1, archivo)){
                    switch (caracter){
                        case 'A':Partida->tablero.matriz[i][j] = 1; 
                                 escritos++;
                                 break;
                        case 'Z':Partida->tablero.matriz[i][j] = -1; /*se llena la matriz*/
                                 escritos++;
                                 break;
                        default: Partida->tablero.matriz[i][j]=0; 
                                 escritos++;
                                 break;
                    }}
            }
	fclose(archivo);
        if(escritos == 6+(Partida->tablero.ancho * Partida->tablero.alto))
            return 1;
        else
            return 0;
    }else 
        return 0;
}

void efectuarMovimiento(tipoJuego * m, int * movimiento)
{
	if(abs(movimiento[0] - movimiento[2]) > 1 || abs(movimiento[1] - movimiento[3]) > 1)
	{
		(*m).tablero.matriz[movimiento[0]][movimiento[1]] = 0;
		if(m->turno == 1)
		{
			(m->jugadorUno.cantidadFichas)--;
		}
		else
		{
			(m->jugadorDos.cantidadFichas)--;
		}
		
	}
	if(m->turno == 1)
	{
		(m->jugadorUno.cantidadFichas)++;
	}
	else
	{
		(m->jugadorDos.cantidadFichas)++;
	}
	(*m).tablero.matriz[movimiento[2]][movimiento[3]] = (*m).turno;
	atacar(m, movimiento);	
}

int
azar()
{
	int r;
	r = rand() % 2;
	if (r == 0)
		return -1;
	else if (r == 1)
		return 1;
return 0;
}


int
analizarComando(tipoJuego * Partida, int * movimiento, char * ingreso){
	char caracterFinal='\0', corchete, flag = -1;
	char *nombreParaArchivo;
	nombreParaArchivo=malloc(95*sizeof(char));
	if(strlen(ingreso) > 3)
	{	
		if(!strcmp("quit", ingreso)){
			flag=0;
		}else if(sscanf(ingreso, "save %s", nombreParaArchivo)==1 && strlen(nombreParaArchivo)>0){
			flag=2;
			Partida->archivo.nombreDeArchivo = nombreParaArchivo;
		}else if (sscanf(ingreso,"[%d,%d][%d,%d%c%c", &movimiento[0], &movimiento[1], &movimiento[2], &movimiento[3],&corchete, &caracterFinal) == 5 && corchete==']')
			{
			if(validarMovimiento(Partida, movimiento))
				{flag=1;}
			}
	}
	return flag;
}

int decidirMenu(tipoJuego * Partida)
{	Partida->error.codigoError=0;
	switch(Partida->modo)
	{
		case 1: 
			Partida->jugadorDos.flagMaquina=OFF;
			break;
		case 2:
			Partida->jugadorDos.flagMaquina=ON;
			break;
		case 3:
			break;
	}
return 0;
}


int 
crearTablero(tipoJuego * Partida)
{
 /* Se reserva espacio suficiente en memoria para toda la Partida->*/
int **aux = NULL;
int *aux2=NULL;
int k;
 aux = malloc( Partida->tablero.alto * sizeof(int *));
 if(aux != NULL)
 	{Partida->tablero.matriz = aux;
	for (k=0;k< Partida->tablero.alto;k++)
		{aux2=calloc(Partida->tablero.ancho,sizeof(int));
		if (aux2 != NULL)
			Partida->tablero.matriz[k]=aux2;
		}
	}
 if (aux == NULL || aux2==NULL)
 {
 	Partida->error.codigoError = 1;
 	return 0;
 }

 /* Se ponen las primeras dos fichas por jugador en las puntas.*/
 Partida->tablero.matriz[0][0]=1;
 Partida->tablero.matriz[0][(Partida->tablero.ancho - 1)]=-1;
 Partida->tablero.matriz[(Partida->tablero.alto -1)][0]=1;
 Partida->tablero.matriz[(Partida->tablero.alto -1)][(Partida->tablero.ancho -1)]=-1;
 Partida->jugadorUno.cantidadFichas = 2;
 Partida->jugadorDos.cantidadFichas = 2;
return 1;
}



/*buscamos un movimiento cualquiera para comprobar que la matriz no se bloquee*/
int buscarMovimiento(tipoJuego * miPartida, int * movimiento)
{
	int i, j, flag = 0;
	for(i = 0; i < (*miPartida).tablero.alto && flag == 0; i++)
		for(j = 0; j < (*miPartida).tablero.ancho && flag == 0; j++)
			if((*miPartida).turno == (*miPartida).tablero.matriz[i][j])
				flag = hallarCero(miPartida, i, j, movimiento);
	return flag;
}



int hallarCero(tipoJuego * m, int fil, int col, int * movimiento)
{
	int izq = (col <= 2)? col : 2;
	int der = ((*m).tablero.ancho - col - 1 <= 2)? (*m).tablero.ancho - col - 1 : 2;
	int arr = (fil <= 2)? fil : 2;
	int abj = ((*m).tablero.alto - fil - 1 <= 2)? (*m).tablero.alto - fil - 1 : 2;
	int i, j, flag = 0;
	for(i = fil - arr; i <= fil + abj && flag == 0; i++)
	{
		for(j = col - izq; j <= col + der && flag == 0; j++)
		{
			if((*m).tablero.matriz[i][j] == 0)
			{
				flag = 1;
				movimiento[0] = fil;
				movimiento[1] = col;
				movimiento[2] = i;
				movimiento[3] = j;
				
			}
		}
	}
	return flag;
}


void atacar(tipoJuego * m, int * movimiento)
{
	int k,l;
	int izq = (movimiento[3] == 0)? 0 : 1;
	int der = (((*m).tablero.ancho - movimiento[3] - 1) == 0)? 0 : 1;
	int arr = (movimiento[2] == 0)? 0 : 1;
	int abj = (((*m).tablero.alto - movimiento[2] - 1) == 0)? 0 : 1;
	for(k = movimiento[2] - arr; k <= movimiento[2] + abj; k++)
		for(l = movimiento[3] - izq; l <= movimiento[3] + der; l++)
		{
			if(m->tablero.matriz[k][l] != 0 && m->tablero.matriz[k][l] != m->turno)
			{
				if(m->turno == 1)
				{
					(m->jugadorUno.cantidadFichas)++;
					(m->jugadorDos.cantidadFichas)--;
				}
				else
				{
					(m->jugadorDos.cantidadFichas)++;
					(m->jugadorUno.cantidadFichas)--;
				}
			}
			m->tablero.matriz[k][l] = (m->tablero.matriz[k][l]*m->tablero.matriz[k][l]) * m->turno; 
		}
			
}
void 
rellenar(tipoJuego *partida){
		int a,b;
		for (a=0;a<(partida->tablero.alto);a++)
			for(b=0;b<(partida->tablero.ancho);b++)
				if (partida->tablero.matriz[a][b]==0)
				{
					partida->tablero.matriz[a][b]=(partida->turno) *(-1);
					if(partida->turno == 1)
						(partida->jugadorDos.cantidadFichas)++;
					else
						(partida->jugadorUno.cantidadFichas)++;
				}
}

int validarMovimiento(tipoJuego * Partida, int * movimiento)
{				/*los primeros dos if son si hace un movimiento fuera del limite, el 3 no es tu ficha, 4 demasiado largo, */
	int flag = 0;		/*5 si se quiere mover al mismo lugar que esta y 6 si el lugar esta ocupado*/
		if(movimiento[0] >= 0 && movimiento[1] >= 0 && movimiento[2] >= 0 && movimiento[3] >=0){		
				if(movimiento[0] < Partida->tablero.alto && movimiento[1] < Partida->tablero.ancho && movimiento[2] < Partida->tablero.alto && movimiento[3] < Partida->tablero.ancho){
					if(Partida->tablero.matriz[movimiento[0]][movimiento[1]] == Partida->turno){
						if(abs(movimiento[0] - movimiento[2]) <= 2 && abs(movimiento[1] - movimiento[3]) <= 2){
							if(movimiento[0] != movimiento[2] || movimiento[1] != movimiento[3]){
								if(Partida->tablero.matriz[movimiento[2]][movimiento[3]]==0)
									flag = 1;
								else
									Partida->error.codigoError = 6;
							}else{Partida->error.codigoError = 5;}
						}else{Partida->error.codigoError = 4;}
					}else{Partida->error.codigoError = 3;}
				}else{Partida->error.codigoError = 2;}
		}else{Partida->error.codigoError = 2;}
									
					
	return flag;
}
