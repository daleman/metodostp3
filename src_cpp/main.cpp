#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

#define COMPONENTES 5 
#define DIGITOS 20
#define VECINOS 10
#define TOLERANCIA 0.001f
#define MAXITER_QR 10000
#define MAXITER_INV_POTENCIA 10


using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] , argv[2] );

	rec.calcularAutovectores_QR(MAXITER_QR, MAXITER_INV_POTENCIA, TOLERANCIA, COMPONENTES);

	rec.abrir_instancia_a_evaluar( argv[3], 1, DIGITOS );


	printf("Seguidilla de digitos:\n");
	for ( int i=0 ; i<DIGITOS ; ++i ) { 
		int dig = rec.reconocer_kVecinos(COMPONENTES, VECINOS, i+1);
		printf("%d ", dig);
	}
	printf("\n");

	return 0;
}
