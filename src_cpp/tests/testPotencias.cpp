#include "../master_header.h"
#include "../matriz.h"

#define TOL 0.0001
#define maxIter 200

using namespace std;

int main()
{

	Matriz<double> autovector(4, 1);
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
	
	return 0;

	
}
