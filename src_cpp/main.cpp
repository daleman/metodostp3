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

	char archivoEntradaDat[1024];
	strcpy( archivoEntradaDat, argv[1] );

	char archivoEntradaCov[1024];
	strcpy( archivoEntradaCov, argv[2] );

	char archivoEntradaTest[1024];
	strcpy( archivoEntradaTest, argv[3] );

	double tolerancia = atof(argv[4]);
	int cantComponentes = atoi(argv[5]);
	int cantVecinos = atoi(argv[6]);

	Reconocedor rec( archivoEntradaDat , archivoEntradaCov );

	rec.calcularAutovectores_QR(MAXITER_QR, MAXITER_INV_POTENCIA, tolerancia, cantComponentes);

	rec.abrir_instancia_a_evaluar( archivoEntradaTest, 1, DIGITOS );


//	rec.promediarTcs( cantComponentes );

	//printf("Seguidilla de digitos:\n");
	int hits = 0;
	for ( int i=0 ; i<DIGITOS ; ++i ) { 
		int dig = rec.reconocer_kVecinos( cantComponentes, cantVecinos, i+1);
		if( rec.labels_aEvaluar[i]==dig){
			hits++;
		}
		//printf("%d ", dig);
	}
	printf("%d\t%d\n",hits,DIGITOS);

	return 0;
}
