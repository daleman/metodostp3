#ifndef __RECONOCEDOR_H
#define __RECONOCEDOR_H

#include "master_header.h"
#include "matriz.h"

class Reconocedor
{
	public :
		
		Reconocedor( char *puntoDat );
		Reconocedor( char *puntoDat, char *matCovarianza );
		~Reconocedor();

		void guardarCovarianza( char *nombre_archivo );
		void calcularAutovectores_QR( int maxIterQR, int maxIterInvPot, double tol);
		void calcularAutovectores_potencia();

//		reconocer_kVecinos( char *archivo );
//		reconocer_kPromedio( char *archivo );

		int cuantosAutovectores() { return cantAutovectores; };

		Matriz<double> *imagenes;
		Matriz<double> *covarianza;
		Matriz<double> *autovectores;


	private :
		
		int cantidad;
		int *labels;
		int cantAutovectores;
		// supongo que mas cosas
};

#endif
