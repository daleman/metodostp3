#include "reconocedor.h"

#define TAMANO_IMAGEN 784
#define MIN_SIGNIFICATIVO 0.01f 

using namespace std;

// PARA COMPARAR TUPLAS POR LA SEGUNDA COMPONENTE
bool compararTupla ( pair<int,double> i, pair<int,double> j ) { return ( i.second < j.second ); }


Reconocedor::Reconocedor( char *puntoDat )
{
	instanciaAbierta = false;
	tcsCalculados = false;

	int scan;
	FILE *data = fopen( puntoDat, "r" );

	scan = fscanf( data, "%d\n", &cantidad );

	imagenes = new Matriz<double>( cantidad, TAMANO_IMAGEN );
	labels = new int[cantidad]; 
	int numerito;

	for ( int i=0 ; i<cantidad ; ++i ) {
		scan = fscanf( data, "%d ", &numerito );
		labels[i] = numerito;
	}

	for ( int i=0 ; i<cantidad ; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			scan = fscanf( data, "%d ", &numerito );
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

	
	promediosCalculados = false;

	cantAutovectores = 0;
}

Reconocedor::Reconocedor( char *puntoDat, char *matCovarianza )
{
	instanciaAbierta = false;
	tcsCalculados = false;

	int scan;
	FILE *data = fopen( puntoDat, "r" );

	scan = fscanf( data, "%d\n", &cantidad );

	imagenes = new Matriz<double>( cantidad, TAMANO_IMAGEN );
	labels = new int[cantidad]; 

	int numerito;

	for ( int i=0 ; i<cantidad ; ++i ) {
		scan = fscanf( data, "%d ", &numerito );
		labels[i] = numerito;
	}

	for ( int i=0 ; i<cantidad ; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			scan = fscanf( data, "%d ", &numerito );
			(*imagenes)[i][j] = (double) numerito;
		}
	}

	fclose( data );


	//matriz de covarianza

	FILE *dataCov = fopen( matCovarianza, "r" );

	int dim1, dim2;
	scan = fscanf( dataCov, "%d %d", &dim1, &dim2 );

	assert( dim1==TAMANO_IMAGEN && dim2==TAMANO_IMAGEN );

	covarianza = new Matriz<double> (dim1, dim2);

	double numeritoDoubel;
	for ( int i=0 ; i<TAMANO_IMAGEN; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			scan = fscanf( dataCov, "%lf ", &numeritoDoubel );
			(*covarianza)[i][j] = numeritoDoubel;
		}
	}

	fclose( dataCov );

	promediosCalculados = false;

	cantAutovectores = 0;
}

Reconocedor::~Reconocedor()
{
	delete imagenes;
	delete[] labels;

	delete covarianza;

	if (cantAutovectores>0) delete autovectores;

	if (tcsCalculados) delete tcs;

	if (instanciaAbierta) {
		delete aEvaluar;
		delete[] labels_aEvaluar;
	}

	if (promediosCalculados)
		delete promediosTcs;
}

void Reconocedor::guardarCovarianza( char *nombre )
{
	FILE *guardar = fopen(nombre, "w");

	//tamano de la matriz
	fprintf(guardar, "%d %d\n", covarianza->cantFil(), covarianza->cantCol());

	//coeficientes de la matriz
	for ( uint i=0 ; i<covarianza->cantFil() ; ++i ) {
		for ( uint j=0 ; j<covarianza->cantCol() ; ++j )
		 	fprintf(guardar, "%d ", (int) (*covarianza)[i][j]);

		fprintf(guardar, "\n");
	}

	fclose(guardar);
}

void Reconocedor::abrir_instancia_a_evaluar( char *archivo, int primero, int ultimo )
{
	FILE *data = fopen( archivo, "r" );
	int scan;
	scan = fscanf( data, "%d\n", &cantidad );

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
		scan = fscanf( data, "%d ", &numerito );

		if ( i+1 >= primero && i+1 <= ultimo ) {
			labels_aEvaluar[indice] = numerito;
			indice++;
		}
	}
	indice = 0;
	for ( int i=0 ; i<cantidad ; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			scan = fscanf( data, "%d ", &numerito );

			if ( i+1 >= primero && i+1 <= ultimo ) {
				(*aEvaluar)[indice][j] = (double) numerito;
			}
		}
		if ( i+1 >= primero && i+1 <= ultimo )
			indice++;
	}

	fclose( data );

	instanciaAbierta = true;

	calcular_tcs();
}

void Reconocedor::calcularAutovectores_QR( int maxIterQR, int maxIterInvPotencia, double tolerancia, int cuantosAutovec )
{

//	printf("Me pidieron esta tolerancia: %f\n", tolerancia);

	if ( (uint) cuantosAutovec > covarianza->cantFil() ) {
		printf("pediste mas autovalores que la cantidad de dimensiones de la matriz, esto va a explotar\n");
		return;
	}

	cantAutovectores = cuantosAutovec;

	autovectores = new Matriz<double> (TAMANO_IMAGEN, cantAutovectores);

	Matriz<double> temp (*covarianza);
	//temp.contieneNaN();
	temp.Householder( tolerancia );
	//temp.contieneNaN();

	vector<double> autoval;
	int iter = maxIterQR;
	temp.QR( tolerancia, iter, autoval );

	//ya tengo los autovalores
	sort(autoval.begin(),autoval.end() );

//	for ( int i=0 ; i<autoval.size() ; ++i ) {
//		printf("%f\n", autoval[i]);
//	}

	Matriz<double> x(TAMANO_IMAGEN, 1);

	for (int i=0 ; i<cuantosAutovec ; ++i ) {
		// agarro los autovalores de menor a mayor!
		double autovalActual = autoval[autoval.size()-i-1];

		if ( fabs(autovalActual) < MIN_SIGNIFICATIVO ) break;

//		printf("%f %f\n", autovalActual, autoval[autoval.size()-i-2]);
//		__BITACORA


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

void Reconocedor::calcularAutovectores_potencia( int maxIterPotSim, double tolerancia, int cuantosAutovec )
{
	if ( (uint) cuantosAutovec > covarianza->cantFil() ) {
		printf("pediste mas autovalores que la cantidad de dimensiones de la matriz, esto va a explotar\n");
		return;
	}

	double autovalActual;

	autovectores = new Matriz<double> (TAMANO_IMAGEN, cuantosAutovec);

	Matriz<double> autovectorActual(TAMANO_IMAGEN, 1);

	for( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
		autovectorActual[j][0] = 1.f;
	}


	for (int i=0 ; i<cuantosAutovec ; ++i ) {

		covarianza->potenciaSimple(autovalActual,autovectorActual,tolerancia, maxIterPotSim);

		if ( fabs(autovalActual) < MIN_SIGNIFICATIVO )
			break;
		else
			cantAutovectores++;	//calculo un autovector


		// lo guardo en mi matriz de autovectores
		for( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			(*autovectores)[j][i] = autovectorActual[j][0];
		}

		covarianza->deflacion(autovalActual,autovectorActual);
	}
}

int Reconocedor::reconocer_kVecinos( int cantComponentes, int k, int indice_imagen )
{
	if (!instanciaAbierta) {
		printf("No se elegio instancia a evaluar\n");
		return -1;
	}

	if ( (uint) indice_imagen > aEvaluar->cantFil() ) {
		printf("pediste una imagen fuera de rango\n");
		return -1;
	}

	if ( cantComponentes > cantAutovectores ) {
		printf("pediste mas componenes principales que la cantidad de autovectores que calculaste\n");
		return -1;
	}

	//me lo dan partiendo de 1
	//pero yo lo uso desde 0
	indice_imagen--;

	int cantIm = imagenes->cantFil();

	vector< pair<int,double> > distancias;

	Matriz<double> resta(cantComponentes,1);

	for ( int i=0 ; i<cantIm ; ++i ) {

		for( int j=0 ; j<cantComponentes ; ++j )
			resta[j][0] = (*tcs_aEvaluar)[indice_imagen][j] - (*tcs)[i][j];

		double distancia = resta.norma();
		
		pair<int,double> labelDistancia;
		labelDistancia.first = labels[i];
		labelDistancia.second = distancia;

//		printf("label %d, a distancia %f\n", labelDistancia.first, labelDistancia.second);

		distancias.push_back(labelDistancia);
	}


	sort( distancias.begin(), distancias.end(), compararTupla );

	vector<int> frecuencias(10, 0);

	for (int i=0 ; i<k ; ++i ) {
		frecuencias[ distancias[i].first ]++;
	}

	int mejor = 0;
	int digito = -1;

	for (int i=0 ; i<10 ; ++i ) {
//		printf("%d ", frecuencias[i]);
		if ( frecuencias[i] > mejor ) {
			mejor = frecuencias[i];
			digito = i;
		}
	}

	return digito;
}

int Reconocedor::reconocer_distanciaMedia( int cantComponentes, int indice_imagen )
{
	if (!instanciaAbierta) {
		printf("No se elegio instancia a evaluar\n");
		return -1;
	}

	if ( (uint) indice_imagen > aEvaluar->cantFil() ) {
		printf("pediste una imagen fuera de rango\n");
		return -1;
	}

	if ( cantComponentes > cantAutovectores ) {
		printf("pediste mas componenes principales que la cantidad de autovectores que calculaste\n");
		return -1;
	}

	indice_imagen--;

	int cantIm = imagenes->cantFil();

	vector<double> distanciasMedias(10, 0.f);
	vector<int> cantApariciones(10, 0);

	Matriz<double> resta(cantComponentes,1);

	for ( int i=0 ; i<cantIm ; ++i ) {

		for( int j=0 ; j<cantComponentes ; ++j )
			resta[j][0] = (*tcs_aEvaluar)[indice_imagen][j] - (*tcs)[i][j];

		double distancia = resta.norma();
		
		//aparecion un a vez mas
		cantApariciones[ labels[i] ]++;
		//con esta distancIA
		distanciasMedias[ labels[i] ] += distancia;
	}


	double mejor = INFINITY;
	int digito = -1;

	for (int i=0 ; i<10 ; ++i ) {
//		printf("%d ", frecuencias[i]);

		double mediaActual = distanciasMedias[i]/(double)cantApariciones[i];
		if ( mediaActual < mejor ) {
			mejor = mediaActual;
			digito = i;
		}
	}

	return digito;
}

int Reconocedor::reconocer_kVecinosPonderados( int cantComponentes, int k, int indice_imagen )
{
	if (!instanciaAbierta) {
		printf("No se elegio instancia a evaluar\n");
		return -1;
	}

	if ( (uint) indice_imagen > aEvaluar->cantFil() ) {
		printf("pediste una imagen fuera de rango\n");
		return -1;
	}

	if ( cantComponentes > cantAutovectores ) {
		printf("pediste mas componenes principales que la cantidad de autovectores que calculaste\n");
		return -1;
	}

	//me lo dan partiendo de 1
	//pero yo lo uso desde 0
	indice_imagen--;

	int cantIm = imagenes->cantFil();

	//first LABEL
	//second DISTANCIA
	vector< pair<int,double> > distancias;

	Matriz<double> resta(cantComponentes,1);
	for ( int i=0 ; i<cantIm ; ++i ) {

		for( int j=0 ; j<cantComponentes ; ++j )
			resta[j][0] = (*tcs_aEvaluar)[indice_imagen][j] - (*tcs)[i][j];

		double distancia = resta.norma();
		
		pair<int,double> labelDistancia;
		labelDistancia.first = labels[i];
		labelDistancia.second = distancia;

		distancias.push_back(labelDistancia);
	}


	sort( distancias.begin(), distancias.end(), compararTupla );

	vector<int> frecuencias(10, 0);
	vector<double> distanciasMedias(10, 0.f);

	for (int i=0 ; i<k ; ++i ) {
		int labelActual = distancias[i].first;

		frecuencias[ labelActual ]++;
		distanciasMedias[ labelActual ] += distancias[i].second;
	}

	double mejor = INFINITY;
	int digito = -1;

	for (int i=0 ; i<10 ; ++i ) {

//		printf("%d ", frecuencias[i]);
		double mediaActual = (frecuencias[i]==0)? INFINITY : distanciasMedias[i]/(double)frecuencias[i];

		if ( mediaActual < mejor ) {
			mejor = mediaActual;
			digito = i;
		}
	}

	return digito;
}

int Reconocedor::reconocer_digitoMedio( int cantComponentes, int indice_imagen )
{
	if (!instanciaAbierta) {
		printf("No se elegio instancia a evaluar\n");
		return -1;
	}

	if (!promediosCalculados) {
		printf("No promediaste antes de llamar a la funcion, esto va a explotar\n");
		return -1;
	}

	if ( (uint) indice_imagen > aEvaluar->cantFil() ) {
		printf("pediste una imagen fuera de rango\n");
		return -1;
	}

	if ( cantComponentes > cantAutovectores ) {
		printf("pediste mas componenes principales que la cantidad de autovectores que calculaste\n");
		return -1;
	}

	indice_imagen--; 

	vector<double> distanciasMedias(10, 0.f);

	Matriz<double> resta(cantComponentes,1);

	for ( int i=0 ; i<10 ; ++i ) {
		for( int j=0 ; j<cantComponentes ; ++j ) {
			resta[j][0] = (*tcs_aEvaluar)[indice_imagen][j] - (*promediosTcs)[i][j];
		}

		double distancia = resta.norma();
		
		//con esta distancIA
		distanciasMedias[i] = distancia;
	}


	double mejor = INFINITY;
	int digito = -1;
	for (int i=0 ; i<10 ; ++i ) {

		double mediaActual = distanciasMedias[i];

		//printf("%f ", distanciasMedias[i]);
		if ( mediaActual < mejor ) {
			mejor = mediaActual;
			digito = i;
		}
	}
	//printf("\n");

	return digito;
}

void Reconocedor::promediarTcs( int cantComponentes )
{
	promediosTcs = new Matriz<double> (10, cantComponentes);

	//inicializo
	for ( uint i=0 ; i<10 ; ++i )
		for ( int j=0 ; j<cantComponentes ; ++j )
			(*promediosTcs)[i][j] = 0.f;


	vector<int> apariciones(10, 0);

	int cuantasImagenes = tcs->cantFil();
	
	for ( int i=0 ; i<cuantasImagenes ; ++i ) {
		for ( int j=0 ; j<cantComponentes ; ++j ) {
			(*promediosTcs)[ labels[i] ][j] += (*tcs)[i][j];
			apariciones[ labels[i] ]++;
		}
	}
	
	
	for ( int i=0 ; i<10 ; ++i ) {
		for ( int j=0 ; j<cantComponentes ; ++j ) {
			if (apariciones[i] == 0) {
				(*promediosTcs)[i][j] = INFINITY;
			} else {
				(*promediosTcs)[i][j] /= apariciones[i];
			}
		}
	}

	promediosCalculados = true;
}

void Reconocedor::calcular_tcs()
{
	int cantIm = imagenes->cantFil();
	int componentes = cantAutovectores;

	tcs = new Matriz<double>( cantIm, componentes );

	int cantImEval = aEvaluar->cantFil();

	tcs_aEvaluar = new Matriz<double>( cantImEval, componentes );

	tcs->cargarMultiplicacion( *imagenes, *autovectores );
	tcs_aEvaluar->cargarMultiplicacion( *aEvaluar, *autovectores );

	tcsCalculados = true;
}
