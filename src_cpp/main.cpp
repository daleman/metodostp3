#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

#define COMPONENTES 20

using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] , argv[2] );

	rec.calcularAutovectores_QR(1000, 30, 0.001f, COMPONENTES);

	rec.abrir_instancia_a_evaluar( argv[3], 1, 20);


	printf("Seguidilla de digitos:\n");
	for ( int i=0 ; i<20 ; ++i ) { 
		int dig = rec.reconocer_kVecinos(COMPONENTES, 30, i+1);
		printf("%d ", dig);
	}
	printf("\n");

	return 0;
}
