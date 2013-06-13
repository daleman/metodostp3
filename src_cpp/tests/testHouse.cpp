#include "../master_header.h"
#include "../matriz.h"


int main()
{
	Matriz<double> hola(4, 4);
	Matriz<double> mult(4, 4);
	Matriz<double> sub(4,1);
	Matriz<double> salida(4,1);
	Matriz<double> salidat(1,4);
	
	double beta;
	Matriz<double> Q(4, 4);
	Matriz<double> R(4, 4);
	Matriz<double> P(4, 4);

	hola[0][0] = 1;
	hola[0][1] = 2;
	hola[0][2] = 3;
	hola[0][3] = 4;
	
	hola[1][0] = 2;
	hola[1][1] = 3;
	hola[1][2] = 4;
	hola[1][3] = 5;
	
	hola[2][0] = 3;
	hola[2][1] = 4;
	hola[2][2] = 5;
	hola[2][3] = 6;
	
	hola[3][0] = 4;
	hola[3][1] = 5;
	hola[3][2] = 6;
	hola[3][3] = 7;

	hola.imprimirMatriz();
	printf("hago la tridiagonal\n");
	//hola.Householder();

	__BITACORA
	hola.imprimirMatriz();
//
//	printf("ya hice la tridiagonal, ahora hago householder");
	//hola.householderQR( Q, R, P);
	hola.submatriz(1,4,1,1,sub);
	sub.imprimirMatriz();
	sub.HouseholderVector(salida,beta);
	salida.imprimirMatriz();
	printf("beta es %f\n",beta);
	salidat.transponer(salida);
	mult.cargarMultiplicacion(salida,salidat);
	mult.imprimirMatriz();
	mult.multiplicarEscalar(beta);
	mult.imprimirMatriz();
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
