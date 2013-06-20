#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

#define MAXITER_QR 10000
#define MAXITER_INV_POTENCIA 10


using namespace std;

int main( int argc, char** argv )
{
	//%1 = set entrenamiento //%2 = covar. set de entrenamiento
	//%3 = set testeo        //%4 = tolerancia
	//%5 = cant Componentes	 //%6 = cantVecinos
	//%7 = tam muestra	 //%8 = metodo

	char *archivoEntradaDat = argv[1];
	char *archivoEntradaCov = argv[2];
	char *archivoEntradaTest = argv[3];
	double tolerancia 	= atof(argv[4]);
	int cantComponentes 	= atoi(argv[5]);
	int cantVecinos 	= atoi(argv[6]);
	int tamMuestra  	= atoi(argv[7]); 
	char * metodo		= argv[9];

	Reconocedor rec( archivoEntradaDat , archivoEntradaCov );

	if( strcmp(metodo,"QR") == 0 ){
		rec.calcularAutovectores_QR(MAXITER_QR, MAXITER_INV_POTENCIA, tolerancia, cantComponentes);
//		printf("QR\n");
	} else {
		rec.calcularAutovectores_potencia( MAXITER_INV_POTENCIA, tolerancia, cantComponentes);
//		printf("PS\n");
	}

	rec.abrir_instancia_a_evaluar( archivoEntradaTest, 1, tamMuestra );


//	rec.promediarTcs( cantComponentes );

	//printf("Seguidilla de digitos:\n");
	int hits = 0;
	for ( int i=0 ; i<tamMuestra ; ++i ) { 
		int dig = rec.reconocer_kVecinos( cantComponentes,cantVecinos, i+1);
		if( rec.labels_aEvaluar[i]==dig){
			hits++;
		}
		//printf("%d ", dig);
	}

//	printf("Tolerancia: %f\n", argv[6]);
//	printf("%f\n", (double)(end - start));
//	printf("%d/%d\n",hits,tamMuestra);
	printf("%d \t %d",hits,tamMuestra);
	return 0;
}
