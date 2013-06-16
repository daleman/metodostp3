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
		void calcularAutovectores_QR( int maxIterQR, int maxIterInvPot, double tol, double minimo_autovalor_significativo );
		void calcularAutovectores_potencia();

		void abrir_intancia_a_evaluar( char *archivo, int primero, int ultimo );

		void reconocer_kVecinos( char *archivo );
//		reconocer_kPromedio( char *archivo );

		int cuantosAutovectores() { return cantAutovectores; };

	private :
	
		void tcs();

		Matriz<double> *imagenes;
		int *labels;

		Matriz<double> *covarianza;
		Matriz<double> *autovectores;
		Matriz<double> *tcs;

		Matriz<double> *aEvaluar;
		int *labels_aEvaluar;
		Matriz<double> *tcs_aEvaluar;

		int cantidad;
		int cantAutovectores;
		bool instanciaAbierta;
		bool tcsCalculados;
};

#endif
