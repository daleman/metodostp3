#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] );

	Matriz<double> XtX( (rec.imagenes)->cantCol, (rec.imagenes)->cantCol );

	XtX.cargarMatPorTranspuesta( *(rec.imagenes) );

	XtX.Householder();

	return 0;
}
