#include "reconocedor.h"

#define TAMANO_IMAGEN 784


using namespace std;

Reconocedor::Reconocedor( char *puntoDat )
{
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

	cantAutovectores = 0;
}

Reconocedor::Reconocedor( char *puntoDat, char *matCovarianza )
{
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
	fscanf( dataCov, "%d\t%d\n", &dim1, &dim2 );

	assert( dim1==TAMANO_IMAGEN && dim2==TAMANO_IMAGEN );

	covarianza = new Matriz<double> (dim1, dim2);

	double numeritoDoubel;
	for ( int i=0 ; i<cantidad ; ++i ) {
		for ( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			fscanf( dataCov, "%lf ", &numeritoDoubel );
			(*covarianza)[i][j] = numeritoDoubel;
		}
	}

	fclose( dataCov );

	cantAutovectores = 0;
}

Reconocedor::~Reconocedor()
{
	delete imagenes;
	delete covarianza;
	delete[] labels;
}

void Reconocedor::guardarCovarianza( char *nombre )
{
	FILE *guardar = fopen(nombre, "w");

	//tamano de la matriz
	fprintf(guardar, "%d\t%d\n", covarianza->cantFil(), covarianza->cantCol());

	//coeficientes de la matriz
	for ( int i=0 ; i<covarianza->cantFil() ; ++i ) {
		for ( int j=0 ; j<covarianza->cantCol() ; ++j )
		 	fprintf(guardar, "%f\t", (*covarianza)[i][j]);

		fprintf(guardar, "\n");
	}

	fclose(guardar);
}

void Reconocedor::calcularAutovectores_QR( int maxIterQR, int maxIterInvPotencia, double tolerancia )
{
	autovectores = new Matriz<double> (TAMANO_IMAGEN, TAMANO_IMAGEN);

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
		if ( autovalActual == 0 ) break;
		else cantAutovectores++;	//calculo un autovector mas

		// le paso un autovector que este lejos de 0
		for( int j=0 ; j<TAMANO_IMAGEN; ++j ) {
			x[j][0] = 1.f;
		}

		covarianza->potenciaInversa(autovalActual, x, tolerancia, maxIterInvPotencia);

		// lo guardo en mi matriz de autovectores
		for( int j=0 ; j<TAMANO_IMAGEN ; ++j ) {
			(*autovectores)[j][i] = x[j][0];
		}
	}
}

void Reconocedor::calcularAutovectores_potencia()
{
}

/*
int kvecinos(int cantComponentes, Matriz<T> imagen, Matriz<T> V,Matriz<T> X, Vector<int> labels,k)
{
	std::vector<double> digitos(10,0.f);
	std::vector<double> labels;
	std::vector<double> normas(cantComponentes,0.f);
	Vt.transponer(V);
	Matriz<double> autodigito_orig = cargarMultiplicacion(Vt,imagen);
	Matriz<T> x_i(cantComponentes,1);
	Matriz<T> temp(cantComponentes,1);
	
	for (i = 0; i < cantComponentes; i++)
	{
		x_i.submatriz(i,i,1,n,X);		//agarro la fila i, es decir una imagen
		
		temp.cargarMultiplicacion(Vt,x_i);
		resta.cargarResta(autodigito_original,temp);
		normas[i] = resta.norma();
	}
	
	//hacemos un k-selection sort con las normas y el vector de labels al mismo tiempo
	// asi quedan ordenados por distancia
	// luego en el vector digitos ( de diez elementos, uno para cada digito), guardamos las frecuencias de los
	// primeros elementos de normas, sumando 1 cuando vemos que es uno de los primeros k de menor distancia.
	
	double min = 0;
	for (i = 0; i < k; i++)
	{
		min = i;
		for (j = i; j < n-1 ; j++)
		{
			min = ( normas [min] < normas[j+1] ) ?  min : (j+1) ;
		}
		double temp2 = normas [i]
		double templ = labels[i];
		labels[i] = labels[min];
		normas[i] = normas[min];
		normas[min] = temp2;
		labels[min] = templ;
		digitos[(labels[i])]++;
	}
	
	int max = 0;
	for (i = 0; i < 9; i++)
	{
		max = ( digitos[max] > digitos[i+1] ) ?  max : (i+1) ;
	}
}
*/
