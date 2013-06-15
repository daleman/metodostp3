#include "../master_header.h"
#include "../matriz.h"
#define TOL 0.1
#define maxIter 2000000

using namespace std;

int main()
{
	Matriz<double> hola(4, 4);
	Matriz<double> chau(4, 4);
	Matriz<double> x(4, 1);
	Matriz<double> autovector(4, 1);
	
//	Matriz<double> mult(4, 4);
//	Matriz<double> sub(4,1);
//	Matriz<double> salida(4,1);
//	Matriz<double> salidat(1,4);
//	
//	double beta;
//	Matriz<double> Q(4, 4);
//	Matriz<double> R(4, 4);
//	Matriz<double> P(4, 4);

	double mat[4][4] = {{1,2,3,4},{2,3,4,5},{3,4,5,6},{4,5,6,7}};

	for ( int i=0 ; i<4 ; ++i ) {
		for ( int j=0 ; j<4 ; ++j ) {
			hola[i][j] = mat[i][j];
		}
	}
	for ( int i=0 ; i<4 ; ++i ) {
		x[i][0] = 0;
	}

	chau.cargarTranspuestaPorMat( hola );

	chau.imprimirMatriz();
	printf("hago la tridiagonal\n");
	chau.Householder();


	chau.imprimirMatriz();



	printf("ya hice la tridiagonal, ahora hago householder\n");

	vector<double> autoval;
	//int maxIter = 100;

	chau.QR( 0.0001f, maxIter , autoval );

	for (int i=0 ; i<autoval.size() ; ++i ) {
		printf( "\n El autovalor es: %f\n", autoval[i] );
		double autov = autoval[i];
		chau.potenciaInversa(x,TOL, maxIter,autov,autovector);
		printf( "\n Su autovector asocioado es:");
		autovector.imprimirMatriz();
	}

	
//
//	//hola.householderQR( Q, R, P);
//	hola.submatriz(1,4,1,1,sub);
//	sub.imprimirMatriz();
//	sub.HouseholderVector(salida,beta);
//	salida.imprimirMatriz();
//	printf("beta es %f\n",beta);
//	salidat.transponer(salida);
//	mult.cargarMultiplicacion(salida,salidat);
//	mult.imprimirMatriz();
//	mult.multiplicarEscalar(beta);
//	mult.imprimirMatriz();
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
