#include "../master_header.h"
#include "../matriz.h"


int main()
{
	Matriz<double> hola(5, 5);
	Matriz<double> Q(5, 5);
	Matriz<double> R(5, 5);
	Matriz<double> P(5, 5);
	
	/*
	 * 	1	2	6	7
	 * 	2	5	8	13
	 *	6	8	12	14 
	 	7	13	14	16
	 */
	 
	hola[0][0] = 1;
	hola[0][1] = 2;
	hola[0][2] = 6;
	hola[0][3] = 7;
	
	hola[1][0] = 2;
	hola[1][1] = 5;
	hola[1][2] = 4;
	hola[1][3] = 13;
	
	hola[2][0] = 5;
	hola[2][1] = 4;
	hola[2][2] = 2;
	hola[2][3] = 1;
	
	hola[3][0] = 7;
	hola[3][1] = 8;
	hola[3][2] = 4;
	hola[3][3] = 2;
	
	/*
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
	*/

	hola.imprimirMatriz();
	printf("hago la tridiagonal");
	hola.Householder();
	printf("ya hice la tridiagonal, ahora hago householder");
	hola.householderQR( Q, R, P);
	//hola.imprimirMatriz();
	printf("esto es Q");
	Q.imprimirMatriz();
	printf("esto es R");
	R.imprimirMatriz();
	printf("esto es P");
	P.imprimirMatriz();
	
	
	
	return 0;
	
}
