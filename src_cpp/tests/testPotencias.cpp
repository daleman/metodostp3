#include "../master_header.h"
#include "../matriz.h"
#include "../reconocedor.h"

#define TOL 0.0001
#define maxIter 200
#define COMPONENTES 5
using namespace std;

int main(int argc, char** argv)
{

	Reconocedor rec( argv[1] , argv[2] );

	rec.calcularAutovectores_QR(1000, 30, 0.001f, COMPONENTES);


	printf("here comes de QR,and its all right\n");
	rec.autovectores->imprimirMatriz();
	printf("\n");



	printf("here comes de QR,and its all right\n");
	rec.calcularAutovectores_potencia( 1000 , 0.001f, COMPONENTES);
	printf("\n");

	rec.autovectores->imprimirMatriz();

	rec.abrir_instancia_a_evaluar( argv[3], 1, 20);


	printf("Seguidilla de digitos:\n");
	for ( int i=0 ; i<20 ; ++i ) { 
		int dig = rec.reconocer_kVecinos(COMPONENTES, 30, i+1);
		printf("%d ", dig);
	}
	printf("\n");

	return 0;

/*	Matriz<double> autovector(4, 1);
	double autoval = 0;	
	
	double arreglo[4][4] = {{1,2,3,4},{2,3,4,5},{3,4,5,6},{4,5,6,7}};
	Matriz<double> mat(4,4);

	for ( int i=0 ; i<4 ; ++i ) {
		for ( int j=0 ; j<4 ; ++j ) {
			mat[i][j] = arreglo[i][j];
		}
	}
	
	autovector[0][0] = 1.;
	autovector[1][0] = 2.;
	autovector[2][0] = 3.;
	autovector[3][0] = 4.;
	
	mat.potenciaSimple(autoval,autovector,0.0001, 200);

	printf( "\n El autovalor es: %f\n", autoval );
	printf( "\n Su autovector asocioado es:\n");

	autovector.imprimirMatriz();

	mat.deflacion(autoval, autovector);
	mat.potenciaSimple(autoval,autovector,0.0001, 200);

	printf( "\n El autovalor es: %f\n", autoval );
	printf( "\n Su autovector asocioado es:\n");

	autovector.imprimirMatriz();

	mat.imprimirMatriz();
*/

	
	return 0;
}
