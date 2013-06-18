

TOLERANCIAS=(0.1 0.01 0.001 0.0001 0.00001)

for cantComp in {1..30}; do
for tol in ${TOLERANCIAS[@]}; do
	echo "tol $tol cantComp $cantComp \n" >> ./resultados/test_${tol}_${cantComp}
	./ejecutables/tp3.exe imagenes/train6000.dat imagenes/train6000.cov imagenes/prim5000test.dat $tol $cantComp >> ./resultados/test_${tol}_${cantComp} 
	done
done
