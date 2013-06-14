#ifndef __RECONOCEDOR_H
#define __RECONOCEDOR_H

#include "master_header.h"
#include "matriz.h"

class Reconocedor
{
	public :
		
		Reconocedor( char *puntoDat );
		~Reconocedor();
		Matriz<double> *imagenes;

		//reconocer( char *archivo );

	private :
		
		int cantidad;
		int *labels;
		// supongo que mas cosas
};

#endif
