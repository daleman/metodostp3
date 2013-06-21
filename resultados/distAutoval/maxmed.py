import sys
import string

datos = sys.argv[1] 

f = open(datos,"r")


serie = []

f.readline()

for line in f:
	serie.append( float( line ) )

suma = 0
j = 0
for i in serie:
	suma += i	
	j += 1


promedio = suma/j
maximo = max(serie)

print str(maximo) + " " + str(promedio)
