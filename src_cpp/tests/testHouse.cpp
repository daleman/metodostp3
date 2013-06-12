#include "../master_header.h"
#include "../matriz.h"


int main()
{
	Matriz<double> hola(4, 4);
	Matriz<double> Q(4, 4);
	Matriz<double> R(4, 4);
	Matriz<double> P(4, 4);

	hola[0][0] = 4;
	hola[0][1] = 1;
	hola[0][2] = -2;
	hola[0][3] = 2;
	
	hola[1][0] = 1;
	hola[1][1] = 2;
	hola[1][2] = 0;
	hola[1][3] = 1;
	
	hola[2][0] = -2;
	hola[2][1] = 0;
	hola[2][2] = 3;
	hola[2][3] = -2;
	
	hola[3][0] = 2;
	hola[3][1] = 1;
	hola[3][2] = -2;
	hola[3][3] = -1;

	hola.imprimirMatriz();
	printf("hago la tridiagonal\n");
	hola.Householder();

	__BITACORA
	hola.imprimirMatriz();
//
//	printf("ya hice la tridiagonal, ahora hago householder");
//	hola.householderQR( Q, R, P);
//	//hola.imprimirMatriz();
//	printf("esto es Q");
//	Q.imprimirMatriz();
//	printf("esto es R");
//	R.imprimirMatriz();
//	printf("esto es P");
//	P.imprimirMatriz();
//	
//	
	
	return 0;
	
}
