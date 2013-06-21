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
	//%5 = tolerancia	 //%6 = tam muestra

	Reconocedor rec( argv[1] , argv[2] );

	int    it   = atoi(argv[4]);
	double tol  = atof(argv[5]);
	int    tamM = atoi(argv[6]);

	clock_t end;
	clock_t start = clock();

	rec.calcularAutovectores_QR(1000, it , tol, tamM);

	end = clock();
	printf("QR %f\n", (double)(end - start));

	start = clock();

	rec.calcularAutovectores_potencia( it, tol, tamM);//		printf("QR\n");

	end = clock();

	printf("PS %f\n", (double)(end - start));

	
	return 0;
}
