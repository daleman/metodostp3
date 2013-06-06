#include <stdlib.h>
#include <stdio.h>

#include <stdlib.h>
#include <stdio.h>

#include "../matriz.h"


int main()
{
	Matriz<double> hola(8, 8);
	
	/*
	 * 	1	2	6	7
	 * 	2	5	8	13
	 *	6	8	12	14 
	 	7	13	14	16
	 */
	 /*
	hola[0][0] = 1;
	hola[0][1] = 2;
	hola[0][2] = 6;
	hola[0][3] = 7;
	
	hola[1][0] = 2;
	hola[1][1] = 5;
	hola[1][2] = 8;
	hola[1][3] = 13;
	
	hola[2][0] = 6;
	hola[2][1] = 8;
	hola[2][2] = 12;
	hola[2][3] = 14;
	
	hola[3][0] = 7;
	hola[3][1] = 13;
	hola[3][2] = 14;
	hola[3][3] = 16;
	*/
	
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j <=i ; j++)
		{
			hola[i][j] = rand()%10;
		}
	}
	
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j< 8 ; j++)
		{
			hola[i][j] = hola[j][i];
		}
	}
	
	hola.imprimirMatriz();
	
	hola.Householder();
	
	//hola.imprimirMatriz();
	
	
	return 0;
	
}
