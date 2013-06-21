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
	int j = 0;


	Reconocedor rec( argv[1] , argv[2] );

	int    it   = atoi(argv[4]);
	double tol  = atof(argv[5]);
	int    tamM = atoi(argv[7]);

	clock_t end;
	clock_t start = clock();

	if( strcmp(argv[6],"QR") == 0 ){
		rec.calcularAutovectores_QR(1000, it , tol, tamM);
	}else{
		rec.calcularAutovectores_potencia( it, tol, tamM);//		printf("QR\n");
	}


	int n = rec.autovectores->cantFil();
	int m = rec.autovectores->cantCol();

	Matriz<double> autovecActual(n,1);

	for(j = 0; j<m; j++){

		for(i=0;i<n;i++){
			autovecActual[i][0] = (*rec.autovectores)[i][j];
		}


		printf("%f\n", rec.covarianza->distanciaAutovector( autovecActual ) );
	}
	
	return 0;
}
