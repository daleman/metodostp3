#ifndef __MATRIZ_H
#define __MATRIZ_H 
#include "master_header.h"

#define COMPARAR_DOUBLE(a, b) (abs( (a) - (b) ) < 0.000000001)
#define SIGNO_DOUBLE( a ) ((abs( (a) ) > 0.000000001) ? (((a) > 0) ? 1.f : -1.f) : 1.f)

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

		void cargarMatPorTranspuesta( Matriz<T> &A )
		{
			uint i,j,k;
			for ( i=0 ; i<n ; ++i ) {
				for ( j=0 ; j<m ; ++j ) {
					double sum = 0;
					for ( k=0 ; k<A.m ; ++k )
						sum += (double) A[i][k] * (double) A[j][k];

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

		void dameDiagonales( std::vector<double> &As, std::vector<double> &Bs )
		{
			As.push_back( matriz[0][0] );
			Bs.push_back( 0.f );

			for ( int i=1 ; i<n ; ++i ) {
				As.push_back(matriz[i][i]);
				Bs.push_back(matriz[i][i-1]);
			}
		}


		void QR( std::vector<double> &As, std::vector<double> &Bs, double tol, int maxIter, std::vector<double> &autoval )
		{
			double shift = 0.f;

			QR_rec( As, Bs, tol, maxIter, autoval, shift );
		}

		void QR_rec( std::vector<double> &As, std::vector<double> &Bs, double tol, int &maxIter, std::vector<double> &autoval, double shift )
		{
			int n = As.size();

			std::vector<double> Cs(n);	// cosenos
			std::vector<double> Ds(n);
			std::vector<double> Qs(n);
			std::vector<double> Rs(n);
			std::vector<double> Ss(n);	// senos
			std::vector<double> Xs(n);
			std::vector<double> Ys(n);
			std::vector<double> Zs(n);

			while ( maxIter > 0 ) {

				if ( abs(Bs[n-1]) <= tol ) {
					// el ultimo b es suficientemente chico, tengo un autoval
					autoval.push_back( As[n-1]+shift );
					n = n-1;
				}
				
				if ( abs( Bs[1] ) <= tol ) {
					// el primer b es suficientemetne chico, tengo un autoval
					autoval.push_back( As[0]+shift );
					n = n-1;
					As[0] = As[1];
					for ( int j=1 ; j<n ; ++j ) {
						As[j] = As[j+1];
						Bs[j] = Bs[j+1];
					}
				}
				
				// casos base
				if ( n==0 ) {
					return;
				}

				if ( n==1 ) {
					autoval.push_back( As[0] + shift );
					return;
				}
	

				for ( int j=2 ; j < n-1 ; ++j ) {
					if ( abs(As[j]) <= tol ) {
						//si tengo un numero pequeno parto la matrz
						//para seguir teniendo una convergencia rapida

						//construimos los As y Bs para el llamado recursivo
						std::vector<double> As1;
						std::vector<double> Bs1;
						std::vector<double> As2;
						std::vector<double> Bs2;

						for ( int k=0 ; k<n ; ++k ) {
							
							if ( k<j-1 ) {
								As1[k] = As[k];
								Bs1[k] = Bs[k];
							} else {
								As2[k] = As[k];
								Bs2[k] = Bs[k];
							}
						}
			
						QR_rec( As1, Bs1, tol, maxIter, autoval, shift );
						QR_rec( As2, Bs2, tol, maxIter, autoval, shift );

						return;
					}
				}

				double b = -(As[n-2] + As[n-1]);
				double c = As[n-1]*As[n-2] - Bs[n-1]*Bs[n-1];
				double d = sqrt(b*b - 4*c);

				double mu1, mu2;
				if ( !COMPARAR_DOUBLE(b,0) && b>0 ) {
					mu1 = -2*c / (b+d);
					mu2 = -(b+d)/2;
				} else {
					mu1 = (d-b)/2;
					mu2 = 2*c / (d-b);
				}

				if ( n==2 ) {
					autoval.push_back( mu1 + shift );
					autoval.push_back( mu2 + shift );
					return;
				}

				double sigma;
				sigma = (abs(mu1-As[n-1]) < abs(mu2-As[n-1])) ? mu1 : mu2;

				shift += sigma;

				for ( int j=0 ; j<n ; ++j )
					Ds[j] = As[j] - sigma;

				Xs[0] = Ds[0];
				Ys[0] = Bs[1];
	
				for ( int j=1 ; j<n ; ++j ) {
					Zs[j-1] = sqrt( Xs[j-1]*Xs[j-1] + Bs[j]*Bs[j]);
					
					Cs[j] = Xs[j-1] / Zs[j-1];

					Ss[j] = Bs[j] / Zs[j-1];

					Qs[j-1] = Cs[j] * Ys[j-1] + Ss[j] * Ds[j];
					Xs[j] = -Ss[j] * Ys[j-1] + Cs[j] * Ds[j];

					if ( j != n ) {
						Rs[j-1] = Ss[j] * Bs[j+1];
						Ys[j] = Cs[j] * Bs[j+1];
					}
				}

				Zs[n-1] = Xs[n-1];

				As[0] = Ss[1] * Qs[0] + Cs[1] * Zs[0];

				Bs[1] = Ss[1] * Zs[1];
	
				for ( int j=1 ; j<n-1 ; ++j ) {
					As[j] = Ss[j+1] * Qs[j] + Cs[j] * Cs[j+1] * Zs[j];
					Bs[j+1] = Ss[j+1] * Zs[j+1];
				}

				As[n-1] = Cs[n-1] * Zs[n-1];

				maxIter--;
			}
		}


		void HouseholderVector(Matriz <T> &salida , double &b)
		{
			//assert( m==1 && salida.cantCol()==1 && n==salida.cantFil() );
			double beta;
			double mu;
			double norma = this->norma();
			Matriz <T> submatriz1(n-1,1);
			Matriz <T> submatriz2(n-1,1);
			Matriz <T> tsubmatriz1(1,n-1);
			
			Matriz <T> sigmaM(1,1);
			submatriz(2,n,1,1,submatriz1);
			submatriz(2,n,1,1,submatriz2);
			tsubmatriz1.transponer(submatriz1);
			sigmaM.cargarMultiplicacion(tsubmatriz1,submatriz2);
			printf("aca esta sigma m      ");
			sigmaM.imprimirMatriz();
			// TRANSPONER SIGMA M?
			double sigma = (sigmaM[0][0]);	// submatriz tiene los indices como en la bibliografia, de 1 a n
			printf("sigma es %f\n",sigma);
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
				printf("mu es %f\n",mu);
				if ( matriz[0][0] <= 0 )
				{
					salida[0][0] = matriz[0][0] - mu;
				}else{
					printf("obviamente paso por aca");
					salida[0][0] = (-1*sigma) / (matriz[0][0] + mu);
					printf ("-%f / (%f + %f) = %f \n",sigma,matriz[0][0],mu,salida[0][0]);
				}
				
				beta = 2.0 * salida[0][0] * salida[0][0] / (sigma + salida[0][0] * salida[0][0]);
				for (int i = 1; i < n; i++)
				{
					salida[i][0] = salida[i][0]/salida[0][0];		//salida [0][0] deberia ser 1 despues de esto
				}
				salida[0][0]=1;		
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
				R.submatriz(j,n,j,j,submatriz);
				R.HouseholderVector(v,beta);
				
				Matriz v2(n,1);
				for (int i = 0; i < n ; i++)
				{
					v2[i][0] =  (i<j-1) ? 0 : v[i-(j-1)][1];
				}
				Matriz ident(n,n);
				//P.cargarResta( ident.identidad(),mult2.multiplicarEscalar(beta, mult.cargarMultiplicacion(v2,v2.transponer())) );
				Matriz v3(n,m);
				v3.transponer(v2);
				mult.cargarMultiplicacion(v2,v3);
				mult.multiplicarEscalar(beta);
				ident.identidad();
				P.cargarResta(ident,mult);
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

		double normaInf()
		{
			double res = matriz[0][0];
			for (int i = 0; i < n; i++){
				res = (abs(res) > abs(matriz[i][0]) ) ? res : matriz[i][0];
			}
			return res;
		}

		
		int menorP()
		{
			int p = 0;
			double res = matriz[0][0];
			for (int i = 0; i < n; i++){
				if ( abs(res) < abs(matriz[i][0]) )
				{
						res = matriz[i][0];
						p = i;
				}
				
				
			}
			return p;
		}

				
		void potenciaInversa( Matriz <T> &x, double tol, int maxIter, double &autovalor, Matriz<T> &autovector)
		{

		assert( x.cantFil() == n && x.cantCol()==1 );
		Matriz<T> xt(1,n);
		Matriz<T> resta(n,1);
		Matriz<T> mult(1,1);
		Matriz<T> xtA(1,n);
		Matriz<T> xtAx(1,1);
		Matriz<T> y(n,1);
		Matriz<T> P(n,n);	// asumo que P, L y U son cuadradas
		Matriz<T> L(n,n);
		Matriz<T> U(n,n);
		Matriz<T> B(n,n);
		Matriz<T> qident(n,n);
		mult.cargarMultiplicacion(xt,x);
		double m = mult[0][0];
		int k = 0;
		this->factorizacionLU(P,L,U);

		xtA.cargarMultiplicacion(xt,this);
		xtAx.cargarMultiplicacion(xtA,x);
		double q = xtAx[0][0] / m;
		qident.identidad();
		qident.multiplicarEscalar(q);
		

		int p = x.menorP();
		x.MultiplicarEscalar(1/p);
			while ( k< n){
				B.cargarResta(this,qident);
				y.resolverLU(B,P,L,U);		// resuelvo el sistema de ecuaciones
				double mu =  y[p][0];
				p = y.menorP();
				y.multiplicarEscalar(1/mu);
				resta.cargarResta(x,y);
				for (int i = 0; i < n; i++)
				{
					x[i][0] = y[i][0];
				}
				double err = normaInf(resta);
				if (err < tol)
				{
					mu = 1/ mu + q;
					autovalor = mu;
					autovector.copiar(x);
					return;
				}
				k++;
			}
			printf("se llego a la maxima cant de iteraciones");
			return;
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
};

#endif
