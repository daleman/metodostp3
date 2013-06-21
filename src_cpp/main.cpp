#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

#define MAXITER_QR 1000
#define MAXITER_INV_POTENCIA 1000


using namespace std;


void modoUso()
{
	printf("\nModo de uso:\n\n"
			"\t./tp3.exe 1DIGITOS_TRAINING 2DIGITOS_TEST 3MATRIZ_COVARIANZA "		//1 2 3
				"4PRIMER_DIGITO 5ULTIMO_DIGITO 6TOLERANCIA 7CANTIDAD_COMPONENTES "	//4 5 6 7
				"8METODO_AUTOVECTORES 9METODO_RECONOCIMIENTO "					//8 9
				"10CANTIDAD_VECINOS\n\n"											//10
			"\tDonde:\n\n"
				"\t\tMATRIZ_COVARIANZA es una matriz de covarianza precalculada,"
					"o NONE en caso de querer calcular la matriz en el momento\n\n"
				"\t\tPRIMER_DIGITO y ULTIMO_DIGITO determinan el intervalo"
					"de digitos que desean analizarse de la entrada DIGITOS_TEST\n\n"
				"\t\tMETODO_AUTOVECTORES puede ser QR o POTENCIA\n\n"
				"\t\tMETODO_RECONOCIMIENTO puede ser KVECINOS, KPONDERADOS o PROMEDIO\n\n"
				"\t\tCANTIDAD_VECINOS es la cantidad de vecinos o NONE si no se usa un metodo "
				"que use este parametro\n\n"
				"\t ES IMPORTANTE notar el hecho de que este programa DEBE usarse siempre "
				"con 10 parametros\n\n"
			);
}


int main( int argc, char** argv )
{
	if (argc != 11) {
		modoUso();
		return 0;
	}

	char *archivoEntradaDat		= argv[1];
	char *archivoEntradaTest	= argv[2];
	char *archivoEntradaCov		= argv[3];
	int imagenIn				= atoi(argv[4]); 
	int imagenFin				= atoi(argv[5]); 
	double tolerancia			= atof(argv[6]);
	int cantComponentes			= atoi(argv[7]);
	char *metodoAutove			= argv[8];
	char *metodoRecon			= argv[9];
	int cantVecinos				= atoi(argv[10]);


	Reconocedor* rec;

	if ( strcmp(archivoEntradaCov, "NONE" ) == 0 )
		rec = new Reconocedor(archivoEntradaDat);
	else
		rec = new Reconocedor( archivoEntradaDat , archivoEntradaCov );


	if ( strcmp(metodoAutove,"QR") == 0 ) {
		rec->calcularAutovectores_QR(MAXITER_QR, MAXITER_INV_POTENCIA, tolerancia, cantComponentes);
	} else {
		rec->calcularAutovectores_potencia( MAXITER_INV_POTENCIA, tolerancia, cantComponentes);
	}


	rec->abrir_instancia_a_evaluar( archivoEntradaTest, imagenIn, imagenFin);


	if ( strcmp(metodoRecon, "PROMEDIO") == 0 )
		rec->promediarTcs( cantComponentes );


	printf("\nDigitos reconocidos:\n");

	int hits = 0;
	int tamMuestra = imagenFin - imagenIn +1;

	if ( strcmp(metodoRecon, "PROMEDIO") == 0 ) {
		for ( int i=0 ; i<tamMuestra ; ++i ) { 
			int dig = rec->reconocer_distanciaMedia( cantComponentes, i+1);

			if( rec->labels_aEvaluar[i] == dig) hits++;

			printf("%d ", dig);
		}
	} else if ( strcmp(metodoRecon, "KVECINOS") == 0 ) {
		for ( int i=0 ; i<tamMuestra ; ++i ) { 
			int dig = rec->reconocer_kVecinos( cantComponentes,cantVecinos, i+1);
			if( rec->labels_aEvaluar[i] == dig ) hits++;

			printf("%d ", dig);
		}
	} else {
		for ( int i=0 ; i<tamMuestra ; ++i ) { 
			int dig = rec->reconocer_kVecinosPonderados( cantComponentes,cantVecinos, i+1);
			if( rec->labels_aEvaluar[i] == dig ) hits++;

			printf("%d ", dig);
		}
	}

	printf("\n\nSe reconocieron %d digitos correctamente\n\nHit rate: %.3f\n\n",hits, (double)hits/(double)tamMuestra);

	return 0;
}
