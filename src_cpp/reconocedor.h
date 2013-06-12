#ifndef __RECONOCEDOR_H
#define __RECONOCEDOR_H

#include "master_header.h"
#include "matriz.h"

class Reconocedor
{
	public :
		
		Reconocedor( char *puntoDat );
		~Reconocedor();

		//reconocer( char *archivo );

	private :
		
		int cantidad;
		Matriz<double> *imagenes;
		int *labels;
		// supongo que mas cosas
};

#endif
