#include "../master_header.h"
#include "../matriz.h"

using namespace std;

int main()
{
	Matriz<double> hola(4, 4);

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

	hola.imprimirMatriz();
	printf("hago la tridiagonal\n");
	hola.Householder();


	hola.imprimirMatriz();



	printf("ya hice la tridiagonal, ahora hago householder\n");

	vector<double> autoval;
	int maxIter = 100;

	hola.QR( 0.0001f, maxIter , autoval );

	for (int i=0 ; i<autoval.size() ; ++i ) {
		printf( "%f\n", autoval[i] );
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
