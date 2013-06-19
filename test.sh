make clean
make

TOLERANCIAS=(0.1 0.01 0.001 0.0001 0.00001)

for tol in ${TOLERANCIAS[@]}; do

	echo > ./resultados/test_${tol}

	for cantComp in {1..30..2}; do
		echo -e -n $"\n${tol}\t${cantComp}\t" >> ./resultados/test_${tol} #_${cantComp}

		./ejecutables/tp3.exe imagenes/train6000.dat imagenes/train6000.cov imagenes/prim5000test.dat $tol $cantComp 5 5000 PS >> ./resultados/test_${tol}

		echo $tol.$cantComp
	done
done
