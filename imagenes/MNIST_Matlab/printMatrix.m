function printMatrix(fname,A,cantNum)
	archivo=fopen(fname,'w');
		for i=1:cantNum
			for j=1:28
				for k=1:28
					fprintf(archivo, '%d ', A(j,k,i));
				end
			end
			fprintf(archivo,"\n");
		end
	fclose(archivo);
