
reset

set term png

set terminal png size 1280, 480
set zeroaxis
set grid

set xlabel "Cantidad de componentes"

set ylabel "Porcentaje Digitos bien reconocidos"

set yrange [:97]

set output "variandoPrec.png"
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1   # --- blue
set style line 2 lc rgb '#dd181f' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 3 lc rgb 'green' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 4 lc rgb 'brown' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 5 lc rgb 'black' lt 1 lw 2 pt 7 ps 1   # --- red
set style line 6 lc rgb 'violet' lt 1 lw 2 pt 7 ps 1   # --- red

set key right bottom


plot "PsVariandoPrec" using 1:6 title "Prec: 10" with linespoints ls 4,\
	"PsVariandoPrec" using 1:5 title "Prec: 1" with linespoints ls 5,\
	"PsVariandoPrec" using 1:2 title "Prec: 0.1" with linespoints ls 1,\
	"PsVariandoPrec" using 1:3 title "Prec: 0.01" with linespoints ls 2,\
	"PsVariandoPrec" using 1:4 title "Prec: 0.001" with linespoints ls 3
