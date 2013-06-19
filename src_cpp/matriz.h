#ifndef __MATRIZ_H
#define __MATRIZ_H 

#include "master_header.h"

#define COMPARAR_DOUBLE(a, b) (fabs( (a) - (b) ) < 1e-9)
#define SIGNO_DOUBLE( a ) ((fabs( (a) ) > 1e-9) ? (((a) > 0) ? 1.f : -1.f) : 1.f)

template< typename T >
class Matriz
{
	private:
		uint n,m;
		uint triangulada;

	public:
		T ** matriz;

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

		uint cantFil() { return n; }
		uint cantCol() { return m; }

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
	
		void restarle( Matriz<T> &A )
		{
			assert( n==A.n && m==A.m );

			uint i,j;
			for ( i=0 ; i<n ; ++i )
				for ( j=0 ; j<m ; ++j )
					matriz[i][j] -= A[i][j];
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

		void cargarTranspuestaPorMat(Matriz<T> &A )
		{
			uint i,j,k;
			for ( i=0 ; i<A.m ; ++i ) {
				for ( j=0 ; j<A.m ; ++j ) {
					double sum = 0.f;
					for ( k=0 ; k<A.n ; ++k )
						sum += (double) A[k][i] * (double) A[k][j];

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

		void contieneNaN()
		{
			uint i,j;

			for ( i=0 ; i<n ; ++i )
				for ( j=0 ; j<m ; ++j )
					assert( !(matriz[i][j]!=matriz[i][j]));
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

		void Householder ( double tol )
		{	// la matriz de entrada es la que llama a la funcion, se llama matriz
			std::vector<double> v(n, 0.f);
			std::vector<double> u(n, 0.f);
			std::vector<double> z(n, 0.f);
			std::vector<double> y(n, 0.f);
			
			for (uint k = 0; k < n-2 ; k++)	// reveer los indices hasta donde van
			{
				double q = 0.f;
				for (uint j=k+1; j<n ; j++) {
					q += ( (matriz[j][k])*(matriz[j][k]) );
				}

				// si la columna son todos ceros,
				// no hay nada que hacer
				if( fabs(q) < tol ) continue;

				// ahora q es la sumatoria 

				double alfa = - sqrt(q) * SIGNO_DOUBLE(matriz[k+1][k]);
				
				double rsq = alfa*alfa - alfa * matriz[k+1][k];

				v[k] = 0.f;
				v[k+1] = matriz[k+1][k] - alfa;

				for (uint j = k+2; j<n; j++)
					v[j] = matriz[j][k];
				
				double suma;
				for (uint j=k; j<n ; j++)
				{	
					suma=0.f;
					for (uint i = k+1; i<n ; i++)
						suma += matriz[j][i] * v[i];

					u[j] = (1.f/rsq) * suma; 
				}
			
				double prod = 0.f;
				for (uint i=k+1; i<n ; i++)
					prod += v[i] * u[i];
				
				for (uint j=k; j<n ; j++)
					z[j] = u[j] - (prod/(2.f*rsq)) * v[j];
				
				for (uint l=k+1; l<n-1 ; l++)
				{
					for (uint j=l+1; j<n ; j++)
					{
						matriz[j][l] = matriz[j][l] - v[l] * z[j] - v[j] * z[l];
						matriz[l][j] = matriz[j][l];
					}
					
					matriz[l][l] = matriz[l][l] - 2.f * v[l] * z[l];
				}
			
				matriz[n-1][n-1] = matriz[n-1][n-1] - 2.f * v[n-1] * z[n-1];
				
				for (uint j=k+2; j<n ; j++)
				{
					matriz[k][j] = 0.f;
					matriz[j][k] = 0.f;
				}
				
				matriz[k+1][k] = matriz[k+1][k] - v[k+1] * z[k]; 
				matriz[k][k+1] = matriz[k+1][k];
			}
		}

		void submatriz(int desde1, int hasta1, int desde2, int hasta2, Matriz<T> &salida)
		{
			for (int i = desde1-1; i < hasta1 ; i++)		//hago desde-1 para que se corresponda con los indices de la bibliografia
				for (int j = desde2-1; j < hasta2; j++)
					salida[i-(desde1-1)][j-(desde2-1)] = matriz [i][j];
		}

		void dameDiagonales( std::vector<double> &As, std::vector<double> &Bs )
		{
			As.push_back( matriz[0][0] );
			Bs.push_back( 0.f );

//			printf("%f\n",matriz[0][0] );

			for ( uint i=1 ; i<n ; ++i ) {
				As.push_back(matriz[i][i]);
				Bs.push_back(matriz[i][i-1]);
//				printf("%f\t%f\n",matriz[i][i], matriz[i][i-1]);
			}
		}


		void QR( double tol, int maxIter, std::vector<double> &autoval )
		{
			std::vector<double> As;
			std::vector<double> Bs;

			dameDiagonales(As,Bs);

			double shift = 0.f;

			QR_rec( As, Bs, tol, maxIter, autoval, shift );
		}

		void QR_rec( std::vector<double> &As, std::vector<double> &Bs, double tol, int &maxIter, std::vector<double> &autoval, double shift )
		{
			int tam = As.size();

//#if DEBUG
//			printf("llamado con diag de tamano: %d\n", n);
//#endif

			std::vector<double> Cs(tam, 0.f);	// cosetamos
			std::vector<double> Ds(tam, 0.f);
			std::vector<double> Qs(tam, 0.f);
			std::vector<double> Rs(tam, 0.f);
			std::vector<double> Ss(tam, 0.f);	// senos
			std::vector<double> Xs(tam, 0.f);
			std::vector<double> Ys(tam, 0.f);
			std::vector<double> Zs(tam, 0.f);

			while ( maxIter > 0 ) {
				
				if (tam==0) return;


				// busco la seguidilla mas larga de ceros
				// en la subdiagonal empezando desde el final

				while ( fabs(Bs[tam-1])<tol && tam>0 ) {
					// el ultimo b es suficientemente chico, tengo un autoval
					autoval.push_back( As[tam-1]+shift );
					tam--;
				}

				if (tam==0) return;
				
				// busco la seguidilla mas larga de ceros
				// en la subdiagonal empezando desde el principio 

				int inicio = 1;
				while ( fabs(Bs[inicio]) < tol && inicio<tam) {
					// el primer b es suficientemetne chico, tengo un autoval
					autoval.push_back( As[inicio-1]+shift );
					inicio++;
				}
				if (inicio==tam) return;

				//////// reacomodo
				inicio--;
				if ( inicio>0 ) {
					for ( int j=0; j<tam-inicio ; ++j ) {
						As[j] = As[j+inicio];
						Bs[j] = Bs[j+inicio];
					}
				}
				tam -= inicio;


				////////// casos Base
				if ( tam==0 ) return;

				if ( tam==1 ) {
					autoval.push_back( As[0] + shift );
					return;
				}
	
				//////// posible llamado recursivo
				for ( int j=2 ; j<tam-1 ; ++j ) {
					if ( fabs(Bs[j]) < tol ) {

						//si tengo un numero pequeno parto la matrz
						//para seguir teniendo una convergencia rapida

						//construimos los As y Bs para el llamado recursivo
						std::vector<double> As1;
						std::vector<double> Bs1;
						std::vector<double> As2;
						std::vector<double> Bs2;

						for ( int k=0 ; k<tam ; ++k ) {
							
							if ( k<j-1 ) {
								As1.push_back(As[k]);
								Bs1.push_back(Bs[k]);
							} else {
								As2.push_back(As[k]);
								Bs2.push_back(Bs[k]);
							}
						}
			
						QR_rec( As1, Bs1, tol, maxIter, autoval, shift );
						QR_rec( As2, Bs2, tol, maxIter, autoval, shift );

						return;
					}
				}


				///////// iteracion QR
				double b = -(As[tam-2] + As[tam-1]);
				double c = As[tam-1]*As[tam-2] - Bs[tam-1]*Bs[tam-1];
				double d = sqrt(b*b - 4.f*c);

				double mu1, mu2;
				if ( !COMPARAR_DOUBLE(b,0.f) && b>0.f ) {
					mu1 = -2.f*c / (b+d);
					mu2 = -(b+d)/2.f;
				} else {
					mu1 = (d-b)/2.f;
					mu2 = 2.f*c / (d-b);
				}

				if ( tam==2 ) {
					autoval.push_back( mu1 + shift );
					autoval.push_back( mu2 + shift );
					return;
				}

				double sigma;
				sigma = (fabs(mu1-As[tam-1]) < fabs(mu2-As[tam-1])) ? mu1 : mu2;

				shift += sigma;

				for ( int j=0 ; j<tam ; ++j )
					Ds[j] = As[j] - sigma;

				Xs[0] = Ds[0];
				Ys[0] = Bs[1];
	
				for ( int j=1 ; j<tam ; ++j ) {
					Zs[j-1] = sqrt( Xs[j-1]*Xs[j-1] + Bs[j]*Bs[j]);
					
					Cs[j] = Xs[j-1] / Zs[j-1];

					Ss[j] = Bs[j] / Zs[j-1];

					Qs[j-1] = Cs[j] * Ys[j-1] + Ss[j] * Ds[j];
					Xs[j] = -Ss[j] * Ys[j-1] + Cs[j] * Ds[j];

					if ( j != tam ) {
						Rs[j-1] = Ss[j] * Bs[j+1];
						Ys[j] = Cs[j] * Bs[j+1];
					}
				}

				Zs[tam-1] = Xs[tam-1];

				As[0] = Ss[1] * Qs[0] + Cs[1] * Zs[0];

				Bs[1] = Ss[1] * Zs[1];
	
				for ( int j=1 ; j<tam-1 ; ++j ) {
					As[j] = Ss[j+1] * Qs[j] + Cs[j] * Cs[j+1] * Zs[j];
					Bs[j+1] = Ss[j+1] * Zs[j+1];
				}

				As[tam-1] = Cs[tam-1] * Zs[tam-1];

				maxIter--;
			}

			printf("Paso el max de iteraciones y QR no converge\n");
		}

		void multiplicarEscalar ( double beta)
		{
			for (uint i = 0; i < n ; i++)
				for (uint j = 0; j < m ; j++)
					matriz[i][j] = matriz[i][j] * beta;
		}

		void copiar (Matriz<T> &entrada)
		{
			for (uint i = 0; i < n ; i++)
				for (uint j = 0; j < m ; j++)
					matriz[i][j] = entrada[i][j];
		}

		void identidad( double factor )
		{
			for (uint i = 0; i < n ; i++)
				for (uint j = 0; j < m ; j++)
					matriz[i][j] = (i==j) ? factor : 0.f;
		}

		double norma()
		{
			double res = 0;
			for (uint i = 0; i < n; i++)
				res += matriz[i][0] * matriz[i][0];

			return sqrt(res);
		}

		void normalizar()
		{
			double norm = this->norma();
			this->multiplicarEscalar( 1.f/norm );
		}

		double normaInf()
		{
			double res = fabs(matriz[0][0]);
			for (uint i = 0; i < n; i++){
				res = ( res  > fabs(matriz[i][0]) ) ? res : fabs(matriz[i][0]);
			}
			return res;
		}

		
		int menorP()
		{
			int p = 0;
			double res = matriz[0][0];
			for (uint i = 0; i<n; i++){
				if ( fabs(res) < fabs(matriz[i][0]) )
				{
					res = matriz[i][0];
					p = i;
				}
			}
			return p;
		}

		void deflacion(double autovalor, Matriz<T> &autovector){
			
			int n = autovector.cantFil();
			Matriz<double> temp(n,n);

			Matriz<double> autovectorN( autovector );
			Matriz<double> autovectorNT(1,n);

			autovectorN.normalizar();

			autovectorNT.transponer(autovectorN);
	
			temp.cargarMultiplicacion(autovectorN,autovectorNT);
			temp.multiplicarEscalar(autovalor);

			this->cargarResta(*this, temp);
		}

				
		void potenciaSimple(double &guess, Matriz <T> &x, double tol, int maxIter ){
			assert( x.cantFil() == n && x.cantCol()==1 );

			Matriz<double> resta(n,1);
			Matriz<double> mult(1,1);
			Matriz<double> y(n,1);

			int p = x.menorP();
			double xp = x[p][0];
			x.multiplicarEscalar(1.f/xp);

			double mu_0 = 0;
			double mu_1 = 0;
			double mu_s = 0;

			uint k = 0;
			while (k<n){

				y.cargarMultiplicacion(*this,x);

				p = y.menorP();

				double mu =  y[p][0];
				mu_s = mu_0 - (mu_1 - mu_0)*(mu_1 - mu_0)/( mu - 2*mu_1 + mu_0 );

//				printf("matrix Y");
				y.multiplicarEscalar(1.f/mu);
		
				resta.cargarResta(x,y);

				x.copiar( y );
				double err = resta.normaInf();

				printf("%f\n", err);

				if ( fabs(err)<fabs(tol) && k >= 4 )
				{
					guess = mu_s;
					//printf("Termino Bien en %d iteraciones \n", k);
					return;
				}

				k++;
				mu_0 = mu_1;
				mu_1 = mu;
			}

			printf("se llego a la maxima cant de iteraciones");

			guess = mu_s;
		}

				
		void potenciaInversa(double &guess, Matriz <T> &x, double tol, int maxIter )
		{
			assert( x.cantFil() == n && x.cantCol()==1 );

			//Matriz<double> xt(1,n);
			Matriz<double> resta(n,1);
			Matriz<double> mult(1,1);
			//Matriz<double> xtA(1,n);
			//Matriz<double> xtAx(1,1);
			Matriz<double> y(n,1);
			Matriz<double> P(n,n);	// asumo que P, L y U son cuadradas
			Matriz<double> L(n,n);
			Matriz<double> U(n,n);
			Matriz<double> B(n,n);
			Matriz<double> qident(n,n);


			qident.identidad( guess );

//			x.imprimirMatriz();

			int p = x.menorP();
			double xp = x[p][0];
			x.multiplicarEscalar(1.f/xp);

			B.cargarResta(*this,qident);
			B.factorizacionLU(P,L,U);

			uint k = 0;
			while (k<n) {

				// resuelvo el sistema de ecuaciones
				y.resolverLU(x,P,L,U);

				p = y.menorP();
				double mu =  y[p][0];

				y.multiplicarEscalar(1.f/mu);
				resta.cargarResta(x,y);

				x.copiar( y );
				double err = resta.normaInf();

//				printf("%f\n", err);

				if ( fabs(err)<fabs(tol) )
				{
				//	printf("i: %d - e: %f\n",k,err);
					mu = 1.f / mu + guess;
					guess = mu;
					return;
				}

				k++;
			}
			printf("se llego a la maxima cant de iteraciones\n");
		}
		
		

		void factorizacionLU(  Matriz<T> &P, Matriz<T> &L, Matriz<T> &U )
		{
			uint i,k,j;
					
			for(uint i=0;i<n;i++){
				for(k=0;k<n;k++){
					U[i][k] = matriz[i][k];
					P[i][k] = (i==k) ? 1.f : 0.f;
				}
			}		
					
			for (i=0;i<n;i++){
				
				U.pivoteoParcial(i,P,L);
				L[i][i]=1.f;
				for(j=i+1;j<n;j++){					
					assert( !COMPARAR_DOUBLE( U[i][i], 0.f ) );
					double mji = U[j][i] / U[i][i];
					for (k=i+1;k<n;k++){
						U[j][k] = (double)U[j][k] - mji * (double)U[i][k];
						
					}					
					U[j][i] = 0.f;
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
			
			for(uint i=0;i<n;i++){
				matriz[i][0] = res[i][0];
			}
		}

		void forwardSubstituion( Matriz<T> &L, Matriz<T> &B ,Matriz<T> &res )
		{		
			Matriz<double> resD(res.n,res.m);
			//resD[0][0]=B[0][0];

			for( uint f=0; f < n ; f++ ){
				resD[f][0]= B[f][0];
				for(uint j=0;j<f;j++){
					assert( !COMPARAR_DOUBLE(L[f][f], 0.f) );
					resD[f][0]-=L[f][j]*resD[j][0];
				}
				resD[f][0]/= L[f][f];
			}

			for(uint i=0;i<res.n;i++){
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
				for(uint j=f+1;j<n;j++){
					assert( !COMPARAR_DOUBLE(U[f][f], 0.f) );
					resD[f][0]-=U[f][j]*resD[j][0];
				}
				resD[f][0]/= U[f][f];
			}

			for(uint i=0;i<res.n;i++){
					res[i][0] = (T) resD[i][0];
			}
		}
		
		void pivoteoParcial( int i,Matriz<T> &P,Matriz<T> &L)
        	{
			uint k;
			int max=i;

			for (k=i;k<n;k++){
				 max = (fabs(matriz[k][i]) > fabs(matriz[max][i])) ? k :max;
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
};

#endif
