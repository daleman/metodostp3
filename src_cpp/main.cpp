#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] );

	int tam = (rec.imagenes)->cantCol();

	Matriz<double> XtX(tam,tam);

	XtX.cargarTranspuestaPorMat( *(rec.imagenes) );
	Matriz<double> original( XtX );

	XtX.contieneNaN();

	XtX.Householder();

	XtX.contieneNaN();

	vector<double> autoval;
	int maxIter = 500000;

	XtX.QR( 0.00001f, maxIter , autoval );

	sort(autoval.begin(),autoval.end() );

	Matriz<double> x(tam, 1);
	Matriz<double> xt(1,tam);

	for( int i=0 ; i<tam ; ++i ) {
		x[i][0] = 1.f;
	}

	for (uint i=1 ; i<=autoval.size() ; ++i ) {
		double autovalActual = autoval[autoval.size()-i];

		original.potenciaInversa(autovalActual, x ,0.0001f, 200);

		printf( "\n El autovalor es: %f\n", autovalActual );
	}

	return 0;
}
