int kvecinos(int cantComponentes, Matriz<T> imagen, Matriz<T> V,Matriz<T> X, Vector<int> labels,k)
{
	std::vector<double> digitos(10,0.f);
	std::vector<double> labels;
	std::vector<double> normas(cantComponentes,0.f);
	Vt.transponer(V);
	Matriz<double> autodigito_orig = cargarMultiplicacion(Vt,imagen);
	Matriz<T> x_i(cantComponentes,1);
	Matriz<T> temp(cantComponentes,1);
	
	for (i = 0; i < cantComponentes; i++)
	{
		x_i.submatriz(i,i,1,n,X);		//agarro la fila i, es decir una imagen
		
		temp.cargarMultiplicacion(Vt,x_i);
		resta.cargarResta(autodigito_original,temp);
		normas[i] = resta.norma();
	}
	
	//hacemos un k-selection sort con las normas y el vector de labels al mismo tiempo
	// asi quedan ordenados por distancia
	// luego en el vector digitos ( de diez elementos, uno para cada digito), guardamos las frecuencias de los
	// primeros elementos de normas, sumando 1 cuando vemos que es uno de los primeros k de menor distancia.
	
	double min = 0;
	for (i = 0; i < k; i++)
	{
		min = i;
		for (j = i; j < n-1 ; j++)
		{
			min = ( normas [min] < normas[j+1] ) ?  min : (j+1) ;
		}
		double temp2 = normas [i]
		double templ = labels[i];
		labels[i] = labels[min];
		normas[i] = normas[min];
		normas[min] = temp2;
		labels[min] = templ;
		digitos[(labels[i])]++;
	}
	
	int max = 0;
	for (i = 0; i < 9; i++)
	{
		max = ( digitos[max] > digitos[i+1] ) ?  max : (i+1) ;
	}
}
