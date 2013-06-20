#g++ testPotencias.cpp ../matriz.h ../reconocedor.cpp -o testPotencias.exe

#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prim5000test.dat 30 0.001 QR 5000
#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prim5000test.dat 30 0.001 PS 5000

#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prueba20.dat 30 0.000001 PS 20

metodo=QR
#for metodo in {PS,QR};
#do
	for tolerancia in {0.1,0.001,0.0001,0.00001};
	do
		echo > ../../resultados/distAutoval/$metodo.$tolerancia

		g++ testAutovalores.cpp ../matriz.h ../reconocedor.cpp -o testAutovalores.exe

		./testAutovalores.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prueba20.dat 30 $tolerancia $metodo 20 >> ../../resultados/distAutoval/$metodo.$tolerancia

		echo $metodo.$tolerancia
	done
#done
