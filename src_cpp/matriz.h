#ifndef __MATRIZ_H
#define __MATRIZ_H

#include "master_header.h"

#define COMPARAR_DOUBLE(a, b) (abs( (a) - (b) ) < 0.0000001)

template< typename T >
class Matriz
{
	private:
		T ** matriz;
		uint n,m;
		uint triangulada;

	public:
		Matriz(int _n, int _m) : n(_n),m(_m)
		{
			matriz = new T*[n];
			for( uint i = 0; i < n; ++i)
				matriz[i] = new T[m];
			triangulada = 0;
		}

		Matriz( Matriz<T> &B )
		{
			n = B.n;
			m = B.m;
			triangulada = 0;
			uint i,j;

			matriz = new T*[n];

			for( i=0 ; i<n ; ++i )
				matriz[i] = new T[m];

			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					matriz[i][j] = B[i][j];
				}
			}
		}

		~Matriz()
		{
			for(uint i = 0; i < n; ++i)
				delete [] matriz[i];

			delete [] matriz;
		}

		int cantFil() { return n; }
		int cantCol() { return m; }

		T* operator[](uint i)
		{
			if(i >= n) return NULL;
			return matriz[i];
		}

		void cargarSuma( Matriz<T> &A, Matriz<T> &B)
		{
			assert( n==A.n && m==A.m && A.n==B.n && A.m==B.m );

			uint i,j;
			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					matriz[i][j] = A[i][j] + B[i][j];
				}
			}
		}
		
		void cargarResta( Matriz<T> &A, Matriz<T> &B)
		{
			assert( n==A.n && m==A.m && A.n==B.n && A.m==B.m );

			uint i,j;
			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					matriz[i][j] = A[i][j] - B[i][j];
				}
			}
		}

		void cargarMultiplicacion( Matriz<T> &A, Matriz<T> &B )
		{
			assert( n==A.n && A.m==B.n && B.m==m );

			uint i,j,k;

			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					double sum = 0;
					for ( k=0 ; k<A.m ; ++k ) {
						sum += (double) A[i][k] * (double) B[k][j];
					}
					matriz[i][j] = (T) sum;
				}
			}
		}

		void transponer( Matriz<T> &B )
		{
			assert( n==B.m && m==B.n );
	
			uint i,j;

			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					matriz[i][j] = B[j][i];
				}
			}		
		}

		void imprimirMatriz()
		{
			uint i,j;

			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					printf("%f\t", (double) matriz[i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}

		void guardarArchivo(char * archivo)
		{	//DEBUG - NO ME SAQUES

			FILE *nuevo = fopen( archivo , "w" );
	
			int i;

			fprintf( nuevo, "%d\n", n );

			for ( i=0 ; i<n ; ++i ) {
				fprintf( nuevo, "%f ", matriz[i][0] );
			}

			fprintf( nuevo, "\n" );

			fclose( nuevo );
		}


		void invertir(){

			uint i,j;
			Matriz<double> res(n,1);
			Matriz<double> temp(n,m);
			for(i=0;i<m;i++){
				Matriz<double> canonico(n,1);

				for(j=0;j<n;j++){			
					canonico[j][0] = (j==i)? 1:0;
					res[j][0] = 0;
				}

				printf("\nel canonico es :\n");
				canonico.imprimirMatriz();

				Gauss(canonico,res);

				printf("\nres es :\n");
				res.imprimirMatriz();

				for(j=0;j<n;j++){
					temp[j][i] = res[j][0];
				}
			}
			
			for(i=0;i<n;i++){
				for(j=0;j<m;j++){
					matriz[i][j]= temp[i][j];
				}
			}
			
		}
		/*
		void triangular(){
			uint i,k,j;
			//asi funciona la interfaz de pivoteo
			Matriz<double> TX(n,1);

			for (i=0;i<n;i++){
				//pivoteo agarra el minimo p tal que api!=0
				pivoteoParcial(TX,i);
				
				for(j=i+1;j<n;j++){					
					assert( matriz[i][i] !=0 );
					T mji = matriz[j][i] / matriz[i][i];
					
					for (k=i+1;k<n;k++){
						matriz[j][k] = matriz[j][k] - mji * matriz[i][k];						
					}
					matriz[j][i] = 0;
				}
			}	
			triangulada = 1;
		}

		void resolverTriangulada(Matriz<T> &x ,Matriz<T> &res){
			 Dado el sistema A.res = x, donde A es ESTA MATRIZ TRIANGULADA
			   res son las incognitas, y x es un vector<T>, encuentra
			   las soluciones ( el vector RES solucion ) */
		/*
			assert( triangulada == 1 );

			uint j;

			res[n-1][0]=x[n-1][0]/matriz[n-1][n-1];

			int f = n-2;
			
			for(; f >= 0 ; f-- ){
				res[f][0]= x[f][0];

				for(j=f+1;j<n;j++){
					assert( matriz[f][f] != 0 );
					res[f][0]-=matriz[f][j]*res[j][0];
				}
				res[f][0] /= matriz[f][f];
			}
		}
		*/
		
		void Gauss(  Matriz<T> &x ,Matriz<T> &res )
		{      	/* Dado el sistema A.res = x, donde A es ESTA MATRIZ
			   res son las incognitas, y x es un vector<T>, encuentra
			   las soluciones ( el vector RES solucion ) */
		
			assert( n==(x.n) && res.m==1 && res.m==x.m );
			uint i,k,j;

			Matriz<double> temp(n,m);
			Matriz<double> TX(x.n,x.m);
			Matriz<double> resD(res.n,res.m);
	
			for(i=0;i<x.n;i++){
				for(j=0;j<x.m;j++){
					TX[i][j]= (double) x[i][j];
				}
			}
	
			for(i=0;i<n;i++){
				for(j=0;j<m;j++){
					temp[i][j]= (double)matriz[i][j];
				}
			}

			uint filas = n;
			

			for (i=0;i<n;i++){
				//pivoteo agarra el minimo p tal que api!=0
				temp.pivoteoParcial(TX,i);
				
				for(j=i+1;j<n;j++){					
					assert( temp[i][i] !=0 );
					T mji = temp[j][i] / temp[i][i];
					
					for (k=i+1;k<n;k++){
						temp[j][k] = temp[j][k] - mji * temp[i][k];
						
					}
					TX[j][0] = TX[j][0] - mji*TX[i][0];
					temp[j][i] = 0;
				}
			}	

			//HASTA ACA FUE GAUSS PARA TRIANGULAR
			//AHORA A SUSTITUIR

			resD[n-1][0]=TX[n-1][0]/temp[n-1][n-1];

			int f = filas -2;
			
			for( f=filas-2; f >= 0 ; f-- ){
				resD[f][0]= TX[f][0];
				for(j=f+1;j<n;j++){
					assert( temp[f][f] != 0 );
					resD[f][0]-=temp[f][j]*resD[j][0];
				}
				resD[f][0]/= temp[f][f];
			}
			
			for(i=0;i<res.n;i++){
				for(j=0;j<res.m;j++){
					res[i][j] = (T) resD[i][j];
				}
			}
		}

		

void Householder ( )
{	// la matriz de entrada es la que llama a la funcion, se llama matriz
	int dimension = n; 
	double q;
	double alfa;
	double rsq;
	double suma;
	double prod;
	Matriz<T> v(dimension,1);
	Matriz<T> u(dimension,1);
	Matriz<T> z(dimension,1);
	Matriz<T> y(dimension,1);
	
	for (int k = 0; k < n-2 ; k++)	// reveer los indices hasta donde van
	{
		q = 0;
		alfa = 0;
		rsq = 0;
		for (int j = k+1; j < n ; j++)
		{
			q += ((matriz[j][k])*(matriz[j][k]));
		}
		// ahora q es la sumatoria 
		if ( COMPARAR_DOUBLE( matriz[k+1][k], 0) ){
			alfa = - sqrt(q);
		}else{
			alfa = - sqrt(q) * matriz[k+1][k] / abs(matriz[k+1][k]);
		}
		
		rsq = alfa*alfa - alfa * matriz[k+1][k];


		v[k][0] = 0;
		v[k+1][0] = matriz[k+1][k] - alfa;
		for (int j = k+2; j < n; j++)
		{
			v[j][0] = matriz[j][k];
		}
		
		for (int j = k; j < n ; j++)
		{	
			suma=0;
			for (int i = k+1; i < n ; i++)
			{
				suma += matriz[j][i] * v[i][0];
			}
			u[j][0] = 1/rsq * suma; 
		}
	
		prod = 0;
		for (int i = k+1; i < n ; i++)
		{
			prod += v[i][0] * u[i][0];
		}
		
		for (int j = k; j < n ; j++)
		{
			z[j][0] = u[j][0] - prod / (2*rsq) * v[j][0];
		}
		
		for (int l = k+1; l < n-1 ; l++)
		{
			for (int j = l+1; j <n ; j++)
			{
				matriz[j][l] = matriz[j][l] - v[l][0] * z[j][0] - v[j][0] * z[l][0];
				matriz[l][j] = matriz[j][l];
			}
			
			matriz[l][l] = matriz[l][l] - 2 * v[l][0] * z[l][0];
			
		}
	
		matriz[n-1][n-1] = matriz[n-1][n-1] - 2 * v[n-1][0] * z[n-1][0];
		
		for (int j = k+2; j < n ; j++)
		{
			matriz[k][j] = 0;
			matriz[j][k] = 0;
		}
		
		matriz[k+1][k] = matriz[k+1][k] - v[k+1][0] * z[k][0]; 
		matriz[k][k+1] = matriz[k+1][k];
	}
}

void submatriz(int desde1, int hasta1, int desde2, int hasta2, Matriz<T> &salida)
{
	for (int i = desde1-1; i < hasta1 ; i++)		//hago desde-1 para que se corresponda con los indices de la bibliografia
	{
		for (int j = desde2-1; j < hasta2; j++)
		{
			salida[i-(desde1-1)][j-(desde2-1)] = matriz [i][j];
		} 
	}
	
	
}


void HouseholderVector(Matriz &salida , double &b)
{
	//assert( m==1 && salida.cantCol()==1 && n==salida.cantFil() );
	double beta;
	double mu;
	double norma = this->norma();
	Matriz submatriz1(n-1,1);
	Matriz submatriz2(n-1,1);
	Matriz sigmaM(1,1);
	double sigma = (sigmaM.transponer(cargarMultiplicacion(submatriz(2,n, submatriz1)),submatriz(2,n, submatriz2))[0][0]);	// submatriz tiene los indices como en la bibliografia, de 1 a n
	salida[0][0] = 1;
	for (int i = 1; i < n; i++)
	{
		salida[i][0] = matriz[i][0];
	}
	
	if ( sigma == 0 )
	{
		beta = 0;
	}else{
		mu = sqrt( matriz[0][0]*matriz[0][0] + sigma);
		if ( matriz[0][0] <= 0 )
		{
			salida[0][0] = matriz[0][0] - mu;
		}else{
			salida[0][0] = -sigma / (matriz[0][0] + mu);
		}
		
		beta = 2 * salida[0][0] * salida[0][0] / (sigma + salida[0][0] * salida[0][0]);
		for (int i = 0; i < n; i++)
		{
			salida[i][0] = salida[i][0]/salida[0][0];		//salida [0][0] deberia ser 1 despues de esto
		}		
	}
}


void householderQR ( Matriz &Q, Matriz &R, Matriz &P)
{
	R.copiar(*this);
	Matriz R2(n,m);
	Matriz Q2(n,m);
	Matriz mult(n,m);
	Matriz mult2(n,m);
	Q.identidad();
	double beta;
	Matriz v(n,1);
	int min = (n-1<m) ? (n-1) : m;
	for (int j = 0; j < min ; j++)
	{
		Matriz v(n-j+1,1);
		Matriz submatriz(n-j,1);
		(R.submatriz(j,n,j,j,submatriz)).HouseholderVector(v,beta);
		Matriz v2(n,1);
		for (int i = 0; i < n ; i++)
		{
			v2[i][0] =  (i<j-1) ? 0 : v[i-(j-1)][1];
		}
		Matriz ident(n,n);
		//P.cargarResta( ident.identidad(),mult2.multiplicarEscalar(beta, mult.cargarMultiplicacion(v2,v2.transponer())) );
		Matriz v3(n,m);
		P.cargarResta(ident.identidad(),(mult.cargarMultiplicacion(v2,v3.transponer(v2))).multiplicarEscalar(beta));
		R2.cargarMultiplicacion(P,R);
		R.copiar(R2);
		Q2.cargarMultiplicacion(Q,P);
		Q.copiar(Q2);
		
	}
	
	
}


void multiplicarEscalar ( double &beta)
{
	for (int i = 0; i < n ; i++)
	{
		for (int j = 0; j < m ; j++)
		{
			matriz[i][j] = matriz[i][j] * beta;
		}
		
	}
}

void copiar (Matriz &entrada)
{
	for (int i = 0; i < n ; i++)
	{
		for (int j = 0; j < m ; j++)
		{
			matriz[i][j] = entrada[i][j];
		}
		
	}
}

void identidad()
{
	for (int i = 0; i < n ; i++)
	{
		for (int j = 0; j < m ; j++)
		{
			matriz[i][j] = (i==j) ? 1 : 0;
		}
		
	}
	
}

double norma()
{
	double res;
	for (int i = 0; i < n; i++)
	{
		res = matriz[i][0] * matriz[i][0];
	}
	return sqrt(res);
}

void recorreMatriz( Matriz<T> &salida )
{
	int dimension=n;
	int ultimo = dimension * dimension - 1;
	int numActual = 0;
	int diagActual = 0;
	int desde;
	int hasta;
	int i;
	int row;
	int col;
	int fila= 0;
	int columna= 0;
	
	do
	{
		if ( diagActual < dimension )
		{
			desde = 0;
			hasta = diagActual;
		}
		else 
		{
			desde = diagActual - dimension + 1;
			hasta = dimension - 1;
		}
 
		for ( i = desde; i <= hasta; i++ )
		{
			if ( diagActual % 2 == 0 ) 
			{
				row = hasta - i + desde;
				col = i;
			}
			else // want to fill downwards
			{
				row = i;
				col = hasta - i + desde;
			}
 
			salida[ fila ][ columna ] = matriz[row][col];
			if(columna==n-1){
				columna=0;
				fila++;
			}else{
				columna++;
			}
			numActual++;
		}
 
		diagActual++;
	}
	while ( numActual <= ultimo );
 
}

	void factorizacionLU(  Matriz<T> &P, Matriz<T> &L, Matriz<T> &U )
	{
		uint i,k,j;
				
		for(int i=0;i<n;i++){
			for(k=0;k<n;k++){
				U[i][k] = matriz[i][k];
				P[i][k] = (i==k) ? 1 : 0;
			}
		}		
				
		uint filas = n;
		for (i=0;i<n;i++){
			
			U.pivoteoParcial(i,P,L);
			L[i][i]=1;
			for(j=i+1;j<n;j++){					
				assert( U[i][i] !=0 );
				double mji = U[j][i] / U[i][i];
				for (k=i+1;k<n;k++){
					U[j][k] = (double)U[j][k] - mji * (double)U[i][k];
					
				}					
				U[j][i] = 0;
				L[j][i] = (T) mji;
			}
		}	
	}

	void resolverLU(Matriz<T> &entrada, Matriz<T> &P, Matriz<T> &L, Matriz<T> &U)
	{
		// Tengo A x = b, como  P A = L U , tengo L U x = Pt b
		// sea Y = U x , entonces L Y = Pt b, lo resuelvo con SustitucionAtras
		// depues para obtener x tengo que U x = Y, lo resuelvo con forward substitution
		
		Matriz<double> Y(n,1);
		Matriz<double> res(n,1);
		Matriz<double> b2(entrada.cantFil(),entrada.cantCol());
		/********* Transpongo P y lo multiplico por b *****/

		b2.cargarMultiplicacion(P,entrada);
		
		
		/*********Hago L Y = b2 *******************/
		forwardSubstituion(L,b2,Y);
		
		/*********Hago U x = Y *******************/
		backwardSubstitution(U,Y,res);
		
		for(int i=0;i<n;i++){
			matriz[i][0] = res[i][0];
		}
	}

	void resolverMatrizLU(Matriz<T> &entrada, Matriz<T> &P, Matriz<T> &L, Matriz<T> &U)
	{
		Matriz<T> res(n,1);
		Matriz<T> columna(n,1);
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				columna[j][0] = entrada[j][i];
			}
			res.resolverLU(columna,P,L,U);
			for(int j=0;j<n;j++){
				matriz[j][i]= res[j][0];
				}
		}
	}

	void forwardSubstituion( Matriz<T> &L, Matriz<T> &B ,Matriz<T> &res )
	{		
		Matriz<double> resD(res.n,res.m);
		//resD[0][0]=B[0][0];

		for( int f=0; f < n ; f++ ){
			resD[f][0]= B[f][0];
			for(int j=0;j<f;j++){
				assert( L[f][f] != 0 );
				resD[f][0]-=L[f][j]*resD[j][0];
			}
			resD[f][0]/= L[f][f];
		}

		for(int i=0;i<res.n;i++){
				res[i][0] = (T) resD[i][0];
		}
	}

	void backwardSubstitution( Matriz<T> &U, Matriz<T> &B ,Matriz<T> &res )
	{	
		Matriz<double> resD(res.n,res.m);
		resD[n-1][0]=B[n-1][0]/U[n-1][n-1];

		int f = n -2;

		for( f=n-2; f >= 0 ; f-- ){
			resD[f][0]= B[f][0];
			for(int j=f+1;j<n;j++){
				assert( U[f][f] != 0 );
				resD[f][0]-=U[f][j]*resD[j][0];
			}
			resD[f][0]/= U[f][f];
		}

		for(int i=0;i<res.n;i++){
				res[i][0] = (T) resD[i][0];
		}
	}

	void pivoteoParcial( int i,Matriz<T> &P,Matriz<T> &L)
	{
		uint k;
		int max=i;

		for (k=i;k<n;k++){
			 max = (abs(matriz[k][i]) > abs(matriz[max][i])) ? k :max;
		}

		if( max != i ){

			for (k=0;k<n;k++){
				//cambio de filas
				T temp=matriz[i][k]; 
				matriz[i][k]= matriz[max][k];
				matriz[max][k] = temp;
				
				temp=P[i][k]; 
				P[i][k]= P[max][k];
				P[max][k] = temp;
				
				temp=L[i][k]; 
				L[i][k]= L[max][k];
				L[max][k] = temp;
			}		
		}			
	}

	void pivoteoParcial( Matriz<T> &x,int i)
	{
		uint k;
		int max=i;

		for (k=i;k<n;k++){
			 max = (abs(matriz[k][i]) > abs(matriz[max][i])) ? k :max;
		}

		if( max != i ){

			for (k=0;k<n;k++){
				//cambio de filas
				T temp=matriz[i][k]; 
				matriz[i][k]= matriz[max][k];
				matriz[max][k] = temp;
			}		
			//cambio la fila  de b
			T temp = x[max][0];
			x[max][0]= x[i][0];
			x[i][0] = temp;
		}			
	}
};

#endif
