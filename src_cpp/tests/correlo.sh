#g++ testPotencias.cpp ../matriz.h ../reconocedor.cpp -o testPotencias.exe
#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prim5000test.dat 30 0.001 QR 5000
#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prim5000test.dat 30 0.001 PS 5000
#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prueba20.dat 30 0.000001 PS 20
#--------------------------------------------------------------------------------------------------------------------------------

#g++ testAutovalores.cpp ../matriz.h ../reconocedor.cpp -o testAutovalores.exe

#for metodo in {PS,QR};
#do
#	for tolerancia in {0.1,0.001,0.0001,0.00001};
#	do
#		echo > ../../resultados/distAutoval/$metodo.$tolerancia



#		./testAutovalores.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prueba20.dat 30 $tolerancia $metodo 20 >> ../../resultados/distAutoval/$metodo.$tolerancia
#		echo $metodo.$tolerancia
#	done
#done

#---------------------------------------------------------------------------------------------------------------------------------


g++ testTiempo.cpp ../matriz.h ../reconocedor.cpp -o testTiempo.exe

#for tolerancia in {0.1,0.01,0.001,0.0001,0.00001};
#do
#	echo > ../../resultados/tiempos/$tolerancia

#	./testTiempo.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prueba20.dat 30 $tolerancia 20 >> ../../resultados/tiempos/$tolerancia
#	echo $tolerancia
#done
	tolerancia=0.000001

	echo > ../../resultados/tiempos/$tolerancia

	./testTiempo.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prueba20.dat 30 $tolerancia 20 >> ../../resultados/tiempos/$tolerancia

