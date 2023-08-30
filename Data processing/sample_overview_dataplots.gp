set terminal pngcairo enhanced background rgb 'white' size 1920, 1080 font 'Times-New-Roman, 35'
set border lw 4
set border behind
set xlabel offset 0, 0.5
set ylabel offset 0.25, 0
#set xtics 432000
set mxtics 1
set xlabel 'Date (in day/month/year)'
set ylabel "Average power (W)"
set xdata time
set datafile separator ","
set timefmt "%d/%m/%Y"
set format x "%d/%m/%Y"
set xtics "",86400,""
set xrange ["":""]
set size ratio 0.5
#set key reverse
#set key Left
#set key left top
set key spacing 1
set xtics rotate
set key samplen 1
set key font ',30'

##avg power increase %

set ytics 20
set mytics 2
set ylabel "% increase in {/:Italic P}_{10-min,avg}|_{avg}"
set output "monthavg%.png"
plot [][0:100]".csv" u 1:4 w p notitle ps 6 pt 7 lc rgb 'black' #change x range

##total avg energy

set ytics 300
set mytics 3
set ylabel 'Average Energy (Wh)'
set output "energy_avg10min.png"
plot [][0:1500]".csv" u 1:6 w p t 'WT with FAS-3E' ps 6 pt 5 lc rgb 'blue',\
".csv" u 1:7 w p t 'Bare WT' ps 6 pt 6 lc rgb 'red' lw 4

## %increase total avg energy

set ytics 20
set mytics 2
#unset key
set ylabel '% increase in average energy'
set size ratio 0.5
set xlabel offset 0, 1
set ylabel offset 1, 0

set output 'energy_avg10min%increase.png'
plot [][0:100]".csv" u 1:8 w p notitle ps 6 pt 7 lc rgb 'black'

#avg 10minavg

set ytics nomirror
#set key at "",48
set ytics 10
set mytics 1
set y2tics 2
set my2tics 2
set yrange [0:60]
set y2range [0:10]
set ylabel '{/:Italic P}_{10-min,avg}|_{avg} (W)'
set y2label '{/:Italic v}_{10-min,avg}|_{avg} (m/s)'
set y2label offset -1.5, 0
set output "monthavg.png"
plot ".csv" u 1:2 w p t 'WT with FAS-3E' ps 6 pt 5 lc rgb 'blue',\
".csv" u 1:3 w p t 'Bare WT' ps 6 pt 6 lc rgb 'red' lw 4, \
".csv" u 1:5 w p t 'Wind speed' ps 6 pt 8 lc rgb 'dark-green' lw 4 axes x1y2