function printLabels(fname,A,cantNum)
	archivo=fopen(fname,'w');
		for i=1:cantNum
			fprintf(archivo, '%d ', A(i));
			fprintf(archivo,"\n");
		end
	fclose(archivo);
