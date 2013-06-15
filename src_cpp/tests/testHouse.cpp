#include "../master_header.h"
#include "../matriz.h"
#define TOL 20.f
#define maxIter 80

using namespace std;

int main()
{
	Matriz<double> hola(4, 4);
	Matriz<double> chau(4, 4);

	double mat[4][4] = {{1,2,3,4},{2,3,4,5},{3,4,5,6},{4,5,6,7}};
for ( int i=0 ; i<4 ; ++i ) { for ( int j=0 ; j<4 ; ++j ) {
			hola[i][j] = mat[i][j];
		}
	}


	chau.cargarTranspuestaPorMat( hola );
	Matriz<double> original(chau);

	chau.imprimirMatriz();
	printf("hago la tridiagonal\n");
	chau.Householder();


	chau.imprimirMatriz();


	printf("ya hice la tridiagonal, ahora hago householder\n");

	vector<double> autoval;

	chau.QR( 0.0001f, maxIter , autoval );

	Matriz<double> x(4, 1);
	x[0][0] = 2.35;
	x[1][0] = 4;
	x[2][0] = 2.35;
	x[3][0] = 1;

	Matriz<double> autovector(4, 1);

	for (int i=0 ; i<autoval.size() ; ++i ) {
		double autov = autoval[i];
		original.potenciaInversa(x,TOL, maxIter,autov);
		printf( "\n El autovalor es: %f\n", autoval[i] );
		
		printf( "\n Su autovector asocioado es:\n");
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
