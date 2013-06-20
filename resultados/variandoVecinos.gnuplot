
reset

set term png

set terminal png size 1280, 480
set zeroaxis
set grid

set xlabel "Cantidad de componentes"

set ylabel "Porcentaje Digitos bien reconocidos"

set yrange [:100]

set output "variandoVecinos.png"
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1   # --- blue
set style line 2 lc rgb '#dd181f' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 3 lc rgb 'green' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 4 lc rgb 'orange' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 5 lc rgb 'black' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 6 lc rgb 'violet' lt 1 lw 2 pt 7 ps 1   # --- red



plot "TestPrec0.001MetPsVariandoVecinos" using 1:2 title "1 Vecino" with linespoints ls 1,\
	"TestPrec0.001MetPsVariandoVecinos" using 1:3 title "5 Vecinos" with linespoints ls 2,\
	"TestPrec0.001MetPsVariandoVecinos" using 1:4 title "10 Vecinos" with linespoints ls 3,\
	"TestPrec0.001MetPsVariandoVecinos" using 1:5 title "30 Vecino" with linespoints ls 4,\
	"TestPrec0.001MetPsVariandoVecinos" using 1:6 title "50 Vecino" with linespoints ls 5,\
	"TestPrec0.001MetPsVariandoVecinos" using 1:7 title "100 Vecinos" with linespoints ls 6
