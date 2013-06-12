#! /bin/octave -qf


function generarPuntoDat(entradaImg,entradaLbl,salida,inicio,fin)
	img = leerMNISTimage( entradaImg );
	lbl = leerMNISTlabel( entradaLbl );

	salida=fopen(salida,'w');

	#cuantas imagenes
	fprintf(salida, '%d\n', fin-inicio+1);

	#vector de labels
	for i=inicio:fin
		fprintf(salida, '%d ', lbl(i));
	end
	fprintf(salida,'\n');

	#matrices de imagenes
	for i=inicio:fin
		for j=1:28
			for k=1:28
				fprintf(salida, '%d ', img(j,k,i));
			end
		end
		fprintf(salida,'\n');
	end

	fclose(salida);

endfunction

########################################

arg_list = argv ();

if nargin != 5
	printf ("Modo de Uso:\n\t\tgenerarPuntoDat ENTRADA_IMAGENES ENTRADA_LABELS SALIDA_NOMBRE PRIMERA_IMAGEN ULTIMA_IMAGEN\n");
	printf ("\n");
else
	entradaImg = arg_list{1};
	entradaLbl = arg_list{2};
	salida = arg_list{3};
	inicio = str2num( arg_list{4} );
	fin = str2num( arg_list{5} );
		
	generarPuntoDat(entradaImg,entradaLbl,salida,inicio,fin);
endif

