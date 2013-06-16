#include "reconocedor.h"

#define TAMANO_IMAGEN 784

using namespace std;



// PARA COMPARAR TUPLAS POR LA SEGUNDA COMPONENTE
//template< typename T >
//bool compararTupla ( T *i,T *j ) { return ( i[1]<j[1] ); }

bool compararTupla ( double *i, double *j ) { return ( i[1]<j[1] ); }



Reconocedor::Reconocedor( char *puntoDat )
{
	instanciaAbierta = false;
	tcsCalculados = false;

	FILE *data = fopen( puntoDat, "r" );

	fscanf( data, "%d\n", &cantidad );

	imagenes = new Matriz<double>( cantidad, TAMANO_IMAGEN );
	labels = new int[cantidad]; 

	int numerito;

	for ( int i=0 ; i<cantidad ; ++i ) {
		fscanf( data, "%d ", &numerito );
		labels[i] = numerito;
	}

	for ( int i=0 ; i<cantidad ; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			fscanf( data, "%d ", &numerito );
			(*imagenes)[i][j] = (double) numerito;
		}
	}

	fclose( data );

	Matriz<double> X (*imagenes); 


	// tomamos promedio
	double promedio[TAMANO_IMAGEN];
	// arreglo limpio
	memset( promedio, 0, sizeof(double)*TAMANO_IMAGEN );

	for ( int i=0 ; i<cantidad ; ++i )
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j )
			promedio[j] += (*imagenes)[i][j];

	double cant = (double) TAMANO_IMAGEN;

	for ( int i=0 ; i<TAMANO_IMAGEN ; ++i )
		promedio[i] /= cant;

	double denominador = sqrt( cant - 1.f );

	// calculo matriz X

	for ( int i=0 ; i<cantidad ; ++i )
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j )
			(*imagenes)[i][j] = ((*imagenes)[i][j] - promedio[j]) / denominador;

	covarianza = new Matriz<double> (TAMANO_IMAGEN,TAMANO_IMAGEN);
	covarianza->cargarTranspuestaPorMat(X);

	autovectores = new Matriz<double> (TAMANO_IMAGEN, TAMANO_IMAGEN);
	cantAutovectores = 0;
}

Reconocedor::Reconocedor( char *puntoDat, char *matCovarianza )
{
	instanciaAbierta = false;
	tcsCalculados = false;

	FILE *data = fopen( puntoDat, "r" );

	fscanf( data, "%d\n", &cantidad );

	imagenes = new Matriz<double>( cantidad, TAMANO_IMAGEN );
	labels = new int[cantidad]; 

	int numerito;

	for ( int i=0 ; i<cantidad ; ++i ) {
		fscanf( data, "%d ", &numerito );
		labels[i] = numerito;
	}

	for ( int i=0 ; i<cantidad ; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			fscanf( data, "%d ", &numerito );
			(*imagenes)[i][j] = (double) numerito;
		}
	}

	fclose( data );


	//matriz de covarianza

	FILE *dataCov = fopen( matCovarianza, "r" );

	int dim1, dim2;
	fscanf( dataCov, "%d %d", &dim1, &dim2 );

	assert( dim1==TAMANO_IMAGEN && dim2==TAMANO_IMAGEN );

	covarianza = new Matriz<double> (dim1, dim2);

	double numeritoDoubel;
	for ( int i=0 ; i<TAMANO_IMAGEN; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			fscanf( dataCov, "%le ", &numeritoDoubel );
			(*covarianza)[i][j] = numeritoDoubel;
		}
	}

	fclose( dataCov );

	autovectores = new Matriz<double> (TAMANO_IMAGEN, TAMANO_IMAGEN);
	cantAutovectores = 0;
}

Reconocedor::~Reconocedor()
{
	delete imagenes;
	delete[] labels;

	delete covarianza;
	delete autovectores;

	if (tcsCalculados) delete tcs;

	if (instanciaAbierta) {
		delete aEvaluar;
		delete[] labels_aEvaluar;
	}
}

void Reconocedor::guardarCovarianza( char *nombre )
{
	FILE *guardar = fopen(nombre, "w");

	//tamano de la matriz
	fprintf(guardar, "%d %d\n", covarianza->cantFil(), covarianza->cantCol());

	//coeficientes de la matriz
	for ( int i=0 ; i<covarianza->cantFil() ; ++i ) {
		for ( int j=0 ; j<covarianza->cantCol() ; ++j )
		 	fprintf(guardar, "%e ", (*covarianza)[i][j]);

		fprintf(guardar, "\n");
	}

	fclose(guardar);
}

void Reconocedor::abrir_instancia_a_evaluar( char *archivo, int primero, int ultimo )
{
	FILE *data = fopen( archivo, "r" );

	fscanf( data, "%d\n", &cantidad );

	if ( ultimo > cantidad ) {
		printf("pediste mas indices de los que hay, esto va a explotar\n");
		return;
	}

	int cuantosPidio = ultimo - primero + 1;

	aEvaluar = new Matriz<double>( cuantosPidio, TAMANO_IMAGEN );
	labels_aEvaluar = new int[cuantosPidio]; 

	int numerito;

	int indice = 0;
	for ( int i=0 ; i<cantidad ; ++i ) {
		fscanf( data, "%d ", &numerito );

		if ( i+1 >= primero && i+1 <= ultimo ) {
			labels_aEvaluar[indice] = numerito;
			indice++;
		}
	}
	indice = 0;
	for ( int i=0 ; i<cantidad ; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			fscanf( data, "%d ", &numerito );

			if ( i+1 >= primero && i+1 <= ultimo ) {
				(*aEvaluar)[indice][j] = (double) numerito;
			}
		}
		if ( i+1 >= primero && i+1 <= ultimo )
			indice++;
	}

	fclose( data );

	instanciaAbierta = true;
}

void Reconocedor::calcularAutovectores_QR( int maxIterQR, int maxIterInvPotencia, double tolerancia, double minSignificativo )
{
	autovectores->copiar(*covarianza);
	autovectores->contieneNaN();

	autovectores->Householder();
	autovectores->contieneNaN();

	vector<double> autoval;
	int iter = maxIterQR;
	autovectores->QR( tolerancia, iter, autoval );

	//ya tengo los autovalores, puedo usar autovectores
	//para empezar a guardar los autovectores
	sort(autoval.begin(),autoval.end() );

	Matriz<double> x(TAMANO_IMAGEN, 1);

	for (uint i=0 ; i<autoval.size() ; ++i ) {
		// agarro los autovalores de menor a mayor!
		double autovalActual = autoval[autoval.size()-i-1];
		if ( fabs(autovalActual) < minSignificativo )
			break;
		else
			cantAutovectores++;	//calculo un autovector

//		printf("%f\n", autovalActual);

		// le paso un autovector que este lejos de 0
		for( int j=0 ; j<TAMANO_IMAGEN; ++j ) {
			x[j][0] = 1.f;
		}

		covarianza->potenciaInversa(autovalActual, x, tolerancia, maxIterInvPotencia);

		x.normalizar();

		// lo guardo en mi matriz de autovectores
		for( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			(*autovectores)[j][i] = x[j][0];
		}
	}
}

void Reconocedor::calcularAutovectores_potencia()
{
}


int Reconocedor::reconocer_kVecinos( int cantComponentes, int k, int indice_imagen )
{
	if (!instanciaAbierta) {
		printf("No se elegio instancia a evaluar\n");
		return -1;
	}

	calcular_tcs();

	if ( indice_imagen > aEvaluar->cantFil() ) {
		printf("pediste una imagen fuera de rango\n");
		return -1;
	}

	//me lo dan partiendo de 1
	//pero yo lo uso desde 0
	indice_imagen--;

	int cantIm = imagenes->cantFil();

	vector< double* > distancias;

	Matriz<double> resta(TAMANO_IMAGEN,1);

	for ( int i=0 ; i<cantIm ; ++i ) {

		for( int j=0 ; j<TAMANO_IMAGEN ; ++j )
			resta[j][0] = (*aEvaluar)[indice_imagen][j] - (*imagenes)[i][j];

		double distancia = resta.norma();
		
		double labelDistancia[2];
		labelDistancia[0] = labels[i];
		labelDistancia[1] = distancia;
		distancias.push_back(labelDistancia);
	}


	sort( distancias.begin(), distancias.end(), compararTupla );

	vector<int> frecuencias(10, 0);

	for (int i=0 ; i<k ; ++i )
		frecuencias[ (int) distancias[i][0] ]++;	// jaja

	int mejor = k;		// k es una cota superior para las frecuencias
	int digito = -1;

	for (int i=0 ; i<10 ; ++i ) {
		if ( frecuencias[i] < mejor ) {
			mejor = frecuencias[i];
			digito = i;
		}
	}

	return digito;
}

void Reconocedor::calcular_tcs()
{
	int cantIm = imagenes->cantFil();
	int tamIm = imagenes->cantCol();

	tcs = new Matriz<double>( cantIm, tamIm );

	int cantImEval = aEvaluar->cantFil();

	tcs_aEvaluar = new Matriz<double>( cantImEval, tamIm );

	tcs->cargarMultiplicacion( *imagenes, *autovectores );
	tcs_aEvaluar->cargarMultiplicacion( *aEvaluar, *autovectores );

	tcsCalculados = true;
}
