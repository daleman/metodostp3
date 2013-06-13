#ifndef __MATRIZ_H
#define __MATRIZ_H

#include "master_header.h"

#define COMPARAR_DOUBLE(a, b) (abs( (a) - (b) ) < 0.0000001)
#define SIGNO_DOUBLE( a ) ((abs( (a) ) > 0.0000001) ? (((a) > 0) ? 1.f : -1.f) : 1.f)

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

			for ( i=0 ; i<n ; ++i )
				for ( j=0 ; j<m ; ++j )
					matriz[i][j] = B[i][j];
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
			for ( i=0 ; i<n ; ++i )
				for ( j=0 ; j<m ; ++j )
					matriz[i][j] = A[i][j] + B[i][j];
		}
		
		void cargarResta( Matriz<T> &A, Matriz<T> &B)
		{
			assert( n==A.n && m==A.m && A.n==B.n && A.m==B.m );

			uint i,j;
			for ( i=0 ; i<n ; ++i )
				for ( j=0 ; j<m ; ++j )
					matriz[i][j] = A[i][j] - B[i][j];
		}

		void cargarMultiplicacion( Matriz<T> &A, Matriz<T> &B )
		{
			assert( n==A.n && A.m==B.n && B.m==m );

			uint i,j,k;

			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					double sum = 0;
					for ( k=0 ; k<A.m ; ++k )
						sum += (double) A[i][k] * (double) B[k][j];

					matriz[i][j] = (T) sum;
				}
			}
		}

		void transponer( Matriz<T> &B )
		{
			assert( n==B.m && m==B.n );
	
			uint i,j;

			for ( i=0 ; i<n ; ++i )
				for ( j=0 ; j<m ; ++j )
					matriz[i][j] = B[j][i];
		}

		void imprimirMatriz()
		{
			uint i,j;

			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j )
					printf("%f\t", (double) matriz[i][j]);

				printf("\n");
			}

			printf("\n");
		}

		void guardarArchivo(char * archivo)
		{	//DEBUG - NO ME SAQUES

			FILE *nuevo = fopen( archivo , "w" );
	
			int i;

			fprintf( nuevo, "%d\n", n );

			for ( i=0 ; i<n ; ++i )
				fprintf( nuevo, "%f ", matriz[i][0] );

			fprintf( nuevo, "\n" );

			fclose( nuevo );
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
				alfa = - sqrt(q) * SIGNO_DOUBLE(matriz[k+1][k]);
				
				rsq = alfa*alfa - alfa * matriz[k+1][k];


				v[k][0] = 0;
				v[k+1][0] = matriz[k+1][k] - alfa;

				for (int j = k+2; j < n; j++)
					v[j][0] = matriz[j][k];
				
				for (int j = k; j < n ; j++)
				{	
					suma=0;
					for (int i = k+1; i < n ; i++)
						suma += matriz[j][i] * v[i][0];

					u[j][0] = 1/rsq * suma; 
				}
			
				prod = 0;
				for (int i = k+1; i < n ; i++)
					prod += v[i][0] * u[i][0];
				
				for (int j = k; j < n ; j++)
					z[j][0] = u[j][0] - prod / (2*rsq) * v[j][0];
				
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
				for (int j = desde2-1; j < hasta2; j++)
					salida[i-(desde1-1)][j-(desde2-1)] = matriz [i][j];
			
			
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
			// submatriz tiene los indices como en la bibliografia, de 1 a n
			double sigma = (sigmaM.transponer(cargarMultiplicacion(submatriz(2,n, submatriz1)),submatriz(2,n, submatriz2))[0][0]);
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
				for (int j = 0; j < m ; j++)
					matriz[i][j] = matriz[i][j] * beta;
		}

		void copiar (Matriz &entrada)
		{
			for (int i = 0; i < n ; i++)
				for (int j = 0; j < m ; j++)
					matriz[i][j] = entrada[i][j];
		}

		void identidad()
		{
			for (int i = 0; i < n ; i++)
				for (int j = 0; j < m ; j++)
					matriz[i][j] = (i==j) ? 1.f : 0.f;
		}

		double norma()
		{
			double res;
			for (int i = 0; i < n; i++)
				res = matriz[i][0] * matriz[i][0];

			return sqrt(res);
		}

};

#endif
