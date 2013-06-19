reset

set term png

set terminal png size 1280, 480
set zeroaxis
set grid

set xlabel "Cantidad de Componentes"

set ylabel "# digitos Reconocidos"


set output "CantidadComponenetesVsDigitos.png"
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1   # --- blue
plot archivo using 2:3 title nombre with linespoints ls 1
