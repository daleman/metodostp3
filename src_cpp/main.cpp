#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] );

	Matriz<double> XtX( (rec.imagenes)->cantCol(), (rec.imagenes)->cantCol() );

	XtX.cargarTranspuestaPorMat( *(rec.imagenes) );

	XtX.Householder();

	XtX.contieneNaN();

	vector<double> autoval;
	int maxIter = 50;

	XtX.QR( 0.01f, maxIter , autoval );

	for (uint i=0 ; i<autoval.size() ; ++i )
		printf( "%f\n", autoval[i] );

	return 0;
}
