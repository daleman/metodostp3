reset

set term png

set terminal png size 1280, 480
set zeroaxis
set grid

set xlabel "Precisión"
set ylabel "Ticks Microprocesador Promedio"

set logscale x
set xrange [:] reverse

set output "precVsTick.png"
set style line 1 lt 1 lw 2 pt 7 ps 1   # --- blue

plot	"promedios" using 1:2 title "QR" with linespoints ls 1 lc rgb 'blue', \
		"promedios" using 1:3 title "Potencia" with linespoints ls 1 lc rgb 'red'
