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
		void calcularAutovectores_QR( int maxIterQR, int maxIterInvPot, double tol, int cuantosAutovec);
		void calcularAutovectores_potencia(int maxIterPot, double tol, int cuantosAutovec);

		void abrir_instancia_a_evaluar( char *archivo, int primero, int ultimo );

		int reconocer_kVecinos( int cantComponentes, int k, int indice_imagen );
		int reconocer_distanciaMedia( int cantComponentes, int indice_imagen );
		int reconocer_kVecinosPonderados( int cantComponentes, int k, int indice_imagen );
//		reconocer_kPromedio( char *archivo );

		int cuantosAutovectores() { return cantAutovectores; };

		int reconocer_digitoMedio(int, int);

		Matriz<double> *autovectores;
		int *labels;
		int *labels_aEvaluar;

	private :
	
		void calcular_tcs();

		Matriz<double> *imagenes;
		

		Matriz<double> *covarianza;
		Matriz<double> *tcs;

		Matriz<double> *aEvaluar;
		
		Matriz<double> *tcs_aEvaluar;

		int cantidad;
		int cantAutovectores;
		bool instanciaAbierta;
		bool tcsCalculados;
};

#endif
