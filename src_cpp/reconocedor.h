#ifndef __RECONOCEDOR_H
#define __RECONOCEDOR_H

#include "master_header.h"
#include "matriz.h"

class Reconocedor {
	public :
		
		Reconocedor( char *puntoDat );
		Reconocedor( char *puntoDat, char *matCovarianza );
		~Reconocedor();

		void guardarCovarianza( char *nombre_archivo );
		void calcularAutovectores_QR( int maxIterQR, int maxIterInvPot, double tol, int cuantosAutovec);
		void calcularAutovectores_potencia();

		void abrir_instancia_a_evaluar( char *archivo, int primero, int ultimo );

		int reconocer_kVecinos( int cantComponentes, int k, int indice_imagen );
		int reconocer_distanciaMedia( int cantComponentes, int indice_imagen );
		int reconocer_kVecinosPonderados( int cantComponentes, int k, int indice_imagen );
//		reconocer_kPromedio( char *archivo );

		int cuantosAutovectores() { return cantAutovectores; };

	private :
	
		void calcular_tcs();

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
