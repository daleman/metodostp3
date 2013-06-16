#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] );

	rec.calcularAutovectores_QR( 1000, 30, 0.001f, 0.1f );

	printf("tengo %d autovectores\n", rec.cuantosAutovectores() );

	rec.abrir_instancia_a_evaluar( argv[2], 1, 2);

	int dig = rec.reconocer_kVecinos(50, 20, 2);

	printf("me dio esto: %d\n", dig);

	return 0;
}
