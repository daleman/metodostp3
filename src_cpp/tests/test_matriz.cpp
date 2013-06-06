#include <stdlib.h>
#include <stdio.h>

#include "../matriz.h"

using namespace std;

int main()
{
	Matriz<double> hola(4, 4);
	Matriz<double> temp(4, 4);
	Matriz<double> temp2(4, 4);
	Matriz<double> chau(4, 4);
	Matriz<double> P(4, 4);
	Matriz<double> L(4, 4);
	Matriz<double> U(4, 4);
	Matriz<double> mult(4, 4);
	Matriz<double> mult2(4, 4);
	Matriz<double> Pt(4, 4);
	
	int i, j;

	for ( i=0 ; i<4 ; i++ ) {
		for ( j=0 ; j<4 ; j++ ) {
			chau[i][j] = ( i >=j ) ? i+j+1 : 0;
		}
	}
	/*
	hola.imprimirMatriz();
	printf("\n vamos a factorizar \n");
	hola.factorizacionLU(P,L,U);
	printf("\n Ahi va la P: \n");
	P.imprimirMatriz();
	printf("\n Ahi va la L: \n");
	L.imprimirMatriz();
	printf("\n Ahi va la U: \n");
	U.imprimirMatriz();
	/*
	Pt.transponer(P);
	mult.cargarMultiplicacion(L,U);
	printf("\n Ahi va la P x L x U: \n");
	mult2.cargarMultiplicacion(Pt,mult);
	mult2.imprimirMatriz();
	*/
	hola[0][0] = 1;
	hola[0][1] = 2;
	hola[0][2] = 6;
	hola[0][3] = 7;
	
	hola[1][0] = 3;
	hola[1][1] = 5;
	hola[1][2] = 8;
	hola[1][3] = 13;
	
	hola[2][0] = 4;
	hola[2][1] = 9;
	hola[2][2] = 12;
	hola[2][3] = 14;
	
	hola[3][0] = 10;
	hola[3][1] = 11;
	hola[3][2] = 15;
	hola[3][3] = 16;
	
	hola.imprimirMatriz();
	hola.recorreMatriz(chau);
	chau.imprimirMatriz();
	
	/*
	hola.factorizacionLU(P,L,U);
	printf("\n Ahi va la P: \n");
	P.imprimirMatriz();
	printf("\n Ahi va la L: \n");
	L.imprimirMatriz();
	printf("\n Ahi va la U: \n");
	U.imprimirMatriz();
	
	Pt.transponer(P);
	temp.resolverMatrizLU(chau,P,L,U);
	
	temp.imprimirMatriz();
	
	temp2.cargarMultiplicacion(hola,temp);
	temp.cargarMultiplicacion(Pt,temp2);
	temp.imprimirMatriz();
	
	chau.imprimirMatriz();
	return 0;
	*/
}
