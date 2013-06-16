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

	rec1.calcularAutovectores_QR( 100000, 30, 0.001f );

	printf("tengo %d autovectores\n", rec1.cuantosAutovectores() );

	return 0;
}
