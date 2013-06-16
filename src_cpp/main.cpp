#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] );

	char *nombre = "gol.cov";

	rec.guardarCovarianza( nombre );

	Reconocedor rec2( argv[1], nombre );

	rec.calcularAutovectores_QR( 100, 30, 0.001f );

	printf("tengo %d autovectores\n", rec.cuantosAutovectores() );

	return 0;
}
