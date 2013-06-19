#include "../master_header.h"
#include "../matriz.h"
#include "../reconocedor.h"
#include<string.h>

#define TOL 0.0001
#define maxIter 200
#define COMPONENTES 5
using namespace std;

int main(int argc, char** argv)
{
	//%1 = set entrenamiento //%2 = covar. set de entrenamiento
	//%3 = set testeo        //%4 = itQR
	//%5 = tolerancia	 //%6 = metodo
	//%7 = tam muestra

	int hits=0;
	int i = 0;

	Reconocedor rec( argv[1] , argv[2] );

	int    it   = atoi(argv[4]);
	double tol  = atof(argv[5]);
	int    tamM = atoi(argv[7]);

	clock_t end;
	clock_t start = clock();

	if( strcmp(argv[6],"QR") == 0 ){
		rec.calcularAutovectores_QR(1000, it , tol, COMPONENTES);
		printf("QR\n");
	}else{
		rec.calcularAutovectores_potencia( it, tol, COMPONENTES);
		printf("PS\n");
	}

	rec.abrir_instancia_a_evaluar( argv[3], 1, tamM);

	for ( i=0 ; i<tamM ; ++i ) { 
		int dig = rec.reconocer_kVecinos(COMPONENTES, 30, i+1);

		if( rec.labels_aEvaluar[i]==dig){
			hits++;
		}
	}

	end = clock();

	//printf("Tolerancia: %f\n", argv[6]);
	printf("%f\n", (double)(end - start));
	printf("%d/%d\n",hits,tamM);

	return 0;

	
	return 0;
}
