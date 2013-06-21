#include "master_header.h"
#include "matriz.h"
#include "reconocedor.h"

using namespace std;

int main( int argc, char** argv )
{
	if( argc != 3 ) {
		printf("\nModo de uso:\n\n"
				"\t./crear_covarianza.exe INSTANCIA_ENTRADA ARCHIVO_SALIDA\n\n"
			  );

		return 0;
	}

	char *archivoEntradaDat	= argv[1];
	char *salidaCov			= argv[2];

	Reconocedor rec(archivoEntradaDat);

	rec.guardarCovarianza( salidaCov );

	return 0;
}
