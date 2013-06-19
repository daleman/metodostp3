#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"
#include "stdlib.h"


#define COMPONENTES 5 
#define DIGITOS 5000
#define VECINOS 10
#define TOLERANCIA 0.1
#define MAXITER_QR 10000
#define MAXITER_INV_POTENCIA 10


using namespace std;

int main( int argc, char** argv )
{
	Reconocedor rec( argv[1] , argv[2] );

	rec.calcularAutovectores_QR(MAXITER_QR, MAXITER_INV_POTENCIA, atof(argv[4]), atoi(argv[5]));

	rec.abrir_instancia_a_evaluar( argv[3], 1, DIGITOS );


	//printf("Seguidilla de digitos:\n");
	int hits = 0;
	for ( int i=0 ; i<DIGITOS ; ++i ) { 
		int dig = rec.reconocer_kVecinos(atoi(argv[5]), atoi(argv[6]), i+1);
		if( rec.labels_aEvaluar[i]==dig){
			hits++;
		}
		//printf("%d ", dig);
	}
	printf("%d\t%d\n",hits,DIGITOS);

	return 0;
}
