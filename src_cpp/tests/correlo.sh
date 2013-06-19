g++ testPotencias.cpp ../matriz.h ../reconocedor.cpp -o testPotencias.exe

#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prim5000test.dat 30 0.001 QR 5000
#./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prim5000test.dat 30 0.001 PS 5000

./testPotencias.exe ../../imagenes/train6000.dat ../../imagenes/train6000.cov ../../imagenes/prueba20.dat 30 0.000001 PS 20
