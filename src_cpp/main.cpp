#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] );

	char *nombre = "matCov.cov";

	rec.guardarCovarianza( nombre );

	Reconocedor rec2( argv[1], nombre );

	rec2.calcularAutovectores_QR( 100, 30, 0.001f );

	printf("tengo %d autovectores\n", rec2.cuantosAutovectores() );

	return 0;
}
