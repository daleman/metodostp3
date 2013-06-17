#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] , argv[2] );

	rec.calcularAutovectores_QR(1000, 30, 0.001f, 20);

	printf("tengo %d autovectores\n", rec.cuantosAutovectores() );

	rec.abrir_instancia_a_evaluar( argv[3], 1, 20);

	for ( int i=0 ; i<20 ; ++i ) { 
		int dig = rec.reconocer_kVecinos(20, 30, i+1);
		printf("%d\n", dig);
	}

	return 0;
}
