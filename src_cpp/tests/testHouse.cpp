#include "../master_header.h"
#include "../matriz.h"

using namespace std;

int main()
{
	Matriz<double> hola(3, 3);


//	Matriz<double> mult(4, 4);
//	Matriz<double> sub(4,1);
//	Matriz<double> salida(4,1);
//	Matriz<double> salidat(1,4);
//	
//	double beta;
//	Matriz<double> Q(4, 4);
//	Matriz<double> R(4, 4);
//	Matriz<double> P(4, 4);

	hola[0][0] = 3.f;
	hola[0][1] = 1.f;
	hola[0][2] = 0.f;
	
	hola[1][0] = 1.f;
	hola[1][1] = 3.f;
	hola[1][2] = 1.f;
	
	hola[2][0] = 0.f;
	hola[2][1] = 1.f;
	hola[2][2] = 3.f;

	hola.imprimirMatriz();
	printf("hago la tridiagonal\n");
//	hola.Householder();


	hola.imprimirMatriz();



	printf("ya hice la tridiagonal, ahora hago householder\n");

	vector<double> As;
	vector<double> Bs;

	As.push_back( hola[0][0] );
	Bs.push_back( 0.f );

	for ( int i=1 ; i<3 ; ++i ) {
		As.push_back(hola[i][i]);
		Bs.push_back(hola[i][i-1]);
	}

	vector<double> autoval;

	int maxIter = 1000000000;


	hola.QR( As, Bs, 0.01f, maxIter , autoval );

	for (int i=0 ; i<3 ; ++i ) {
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
