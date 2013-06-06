#include "master_header.h"
#include "matriz.h"
#include "filtroSonidos.h"
#include "filtroImagenes.h"
#include "sonidos.h"
#include "imagenes.h"

using namespace std;

int main( int argc, char** argv )
{
	if ( argc!=10 ) {
		printf("Modo de uso:\n\t\t./tp2.exe TIPO_ARCHIVO ARCHIVO_ENTRADA ARCHIVO_SALIDA_CON_RUIDO ARCHIVO_SALIDA_FILTRADO FILTRO PAR1 ATENUACION RUIDO PIXELES_BLOQUE\n\nDonde:\n\n\tTIPO_ARCHIVO puede ser AUDIO o IMAGEN\n\n\tFILTRO puede ser GATE, LPF y GATE_BLOQ y LPF_BLOQ en el caso de las imagenes\n\n\tPAR1 es el umbral en dB en el caso de GATE o el porcentaje entre 0 y 100 de frecuencias que pasan del LPF\n\n\tATENUACION es el coeficiente de atenuacion entre 0 y 1\n\n\tRUIDO es el porcentaje de ruido en funcion del rango dinamico\n\n\tPIXELES_BLOQUE representa la cantidad de pixeles que tiene el lado del bloque\n\n");
		return 0;
	}

	char tipo_archivo[32];
	strcpy( tipo_archivo, argv[1] );

	char archivo_entrada[1024];
	strcpy( archivo_entrada, argv[2] );

	char archivo_salida_ruido[1024];
	strcpy( archivo_salida_ruido, argv[3] );

	char archivo_salida_filtrado[1024];
	strcpy( archivo_salida_filtrado, argv[4] );

	char filtro[32];
	strcpy( filtro, argv[5] );

	double par1 = atof(argv[6]);

	double atenuacion = atof(argv[7]);

	double porcentaje_ruido = atof(argv[8]);
	
	int pixeles_bloque = atoi(argv[9]);
	

	if ( strcmp( tipo_archivo, "AUDIO" ) == 0 ) {	// audio

		Sonidos *sonido = new Sonidos( archivo_entrada );

		double ruido = porcentaje_ruido / 100.f * sonido->maxVal * 2;

		FiltroSonidos filter ( sonido->tamano, sonido->maxVal );

		filter.ruido_normal( *(sonido->samples), ruido );
		sonido->componerSonido( archivo_salida_ruido );
	
		double psnrConRuido = sonido->PSNR();	

		if ( strcmp( filtro, "UMBRAL") == 0 ) {
			filter.denoiser_umbral(*(sonido->samples), par1, atenuacion );
		} else {
			filter.denoiser_sacoFrecAltas(*(sonido->samples), par1, atenuacion );
		}

		sonido->componerSonido( archivo_salida_filtrado );

		double psnrSinRuido = sonido->PSNR();	

		printf( "\nPSNR de la senal ruidosa:\t\t\t%f\nPSNR de la senal filtrada:\t\t\t%f\ncantidad de dB de rango dinamico ganados:\t%f\n\n", psnrConRuido, psnrSinRuido, psnrSinRuido-psnrConRuido );

		delete sonido;

	} if ( strcmp( tipo_archivo, "IMAGEN" ) == 0 ) {

		Imagenes *imagenL = new Imagenes( archivo_entrada );

		double ruido = porcentaje_ruido / 100.f * imagenL->maxVal;

		FiltroImagenes filter ( imagenL->ancho,imagenL->alto, imagenL->maxVal, pixeles_bloque);
		filter.ruido_normal( *(imagenL->pixeles), ruido );
		imagenL->componerImagen( archivo_salida_ruido );
		
		double psnrConRuido = imagenL->PSNR();	

		if ( strcmp( filtro, "UMBRAL") == 0 ) {
			filter.denoiser_umbral(*(imagenL->pixeles), par1, atenuacion );
		} else if ( strcmp( filtro, "UMBRAL_BLOQ") == 0 ) { 
			filter.denoiser_umbral_bloques(*(imagenL->pixeles), par1, atenuacion );
		} else if ( strcmp( filtro, "LPF") == 0 ) { 
			filter.denoiser_sacoFrecAltas(*(imagenL->pixeles), par1, atenuacion );
		} else if ( strcmp( filtro, "LPF_BLOQ" ) == 0 ) { 
			filter.denoiser_sacoFrecAltas_bloques(*(imagenL->pixeles), par1, atenuacion );
		}

		imagenL->componerImagen( archivo_salida_filtrado );

		double psnrSinRuido = imagenL->PSNR();	

		printf( "\nPSNR de la senal ruidosa:\t\t\t%f\nPSNR de la senal filtrada:\t\t\t%f\ncantidad de dB de rango dinamico ganados:\t%f\n\n", psnrConRuido, psnrSinRuido, psnrSinRuido-psnrConRuido );

		delete imagenL;
	}
	

	return 0;
}
