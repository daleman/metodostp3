make clean
make

TOLERANCIAS=(0.001)
VECINOS=(1 5 10 30 50 100)
COMPONENTES=(10 20 30 40 50)

for cantVecinos in ${VECINOS[@]};do
	for tol in ${TOLERANCIAS[@]}; do

		echo > ./resultados/test_${tol}_${cantVecinos}

		for cantComp in ${COMPONENTES[@]}; do
		
			echo -e -n $"\n${tol}\t${cantComp}\t" >> ./resultados/test_${tol}_${cantVecinos} #_${cantComp}

			./ejecutables/tp3.exe imagenes/train60000.dat imagenes/train60000.cov imagenes/prim5000test.dat $tol $cantComp $cantVecinos 5000 PS >> ./resultados/test_${tol}_${cantVecinos}

			echo $tol.$cantComp
		done
	done
done
