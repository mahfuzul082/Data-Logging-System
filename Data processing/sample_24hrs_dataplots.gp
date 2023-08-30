## 24hrs turbulence-wind speed

set terminal pngcairo enhanced background rgb 'white' size 3840, 2160 font 'Times-New-Roman, 70'
set border lw 6
set border behind
set xlabel offset 0, 0.5
set ylabel offset 1, 0
set xtics offset 0, 0.25
set ytics offset 0.25, 0

set tics scale 1
set xtics 1
set mxtics 1
set ytics 20
set mytics 2
set datafile separator ","
set xlabel '{/:Italic v}_{10-min,avg} (m/s)'
set ylabel 'Turbulence intensity (%)'
set size ratio 0.5
set xrange [0:7]
set yrange [0:100]

set output "Time-series speed-turbulence plot 23-6.png"
plot ".csv" u 2:4 every ::1::144 w p notitle ps 8 pt 6 lc rgb 'blue' lw 6

## raw power all and skipped

set terminal pngcairo enhanced background rgb 'white' size 3840, 2160 font 'Times-New-Roman, 45'
set border lw 6
set border behind
set xlabel offset 0, 0.5
set ylabel offset 1, 0
set xtics offset 0, 0.25
set ytics offset 0.25, 0
set tics scale 0.75
set xlabel 'Time (in hours)'
set xdata time
set timefmt '%H:%M:%S'
set size ratio 0.2
set key reverse
set key Left
set key top left
#set key font ',30'
#unset key
set key samplen -0.1
set datafile separator ","
set format x '%02H%02M' 
set xrange ["0:00:00":"24:00:00"]

set ylabel 'Power (W)'
set xtics 10800 #change if necessary (1800 for 2hrs window, 3600 for 3hrs and beyond)
set mxtics 3 #change if necessary (2 for 3 hrs+ window, 1 for 2hrs window)
set ytics 50
set mytics 2

set output "Time-series power plot 23-6 fas.png" #change date
plot [][0:300]".csv" u 2:5 w l t "LE-600 WT with FAS-3E" lw 4 lc rgb 'blue'
set output "Time-series power plot 23-6 bare.png" #change date
plot [][0:300]".csv" u 2:5 w l t 'Bare LE-600 WT' lw 4 lc rgb 'red' #change range, filenames
set output "Time-series power plot 23-6 fas skipped.png" #change date
plot [][0:300]".csv" u 2:5 every 6 w l t "LE-600 WT with FAS-3E" lw 4 lc rgb 'blue'
set output "Time-series power plot 23-6 bare skipped.png" #change date
plot [][0:300]".csv" u 2:5 every 6 w l t 'Bare LE-600 WT' lw 4 lc rgb 'red' #change range, filenames

## 10min bin avg merged

set timefmt "%Y-%M-%d %H:%M:%S"
set ylabel 'Average Power (W)'
set xtics 10800 #change if necessary (1800 for 2hrs window, 3600 for 3hrs and beyond)
set mxtics 18 #change if necessary (2 for 3 hrs+ window, 1 for 2hrs window)
set ytics 20
set mytics 1
set key samplen -0.1
set xrange [" ":" "]
set label "Daily average power for WT with FAS-3E () : 20.76 W\n Daily average power for Bare WT () : 18.93 W" at "", -38 right

set output "Time-series power plot 23-6 10minavg.png" #change date
plot [][0:120]".csv" u 6:7 w p t 'LE-600 WT with FAS-3E ' ps 8 pt 5 lc rgb 'blue',\
".csv" u 6:7 w p t 'Bare LE-600 WT' ps 8 pt 6 lc rgb 'red' lw 6 #change range, filenames

unset label

## 10min bin avg separate

set xtics 10800 #change if necessary (1800 for 2hrs window, 3600 for 3hrs and beyond)
set mxtics 18 #change if necessary (2 for 3 hrs+ window, 1 for 2hrs window)
#set ytics 20
#set mytics 1

set label "Daily average power for WT with FAS-3E () : 20.76 W" at "", -38 right
set output "Time-series power plot 23-6 10min fas.png" #change date
plot [][0:120] NaN t "LE-600 WT with FAS-3E",".csv" u 6:7 w p notitle ps 8 pt 7 lc rgb 'black'

unset label

set label "Daily average power for Bare WT () : 18.93 W" at "", -38 right
set output "Time-series power plot 23-6 10min bare.png" #change date
plot [][0:120] NaN t 'Bare LE-600 WT',".csv" u 6:7 w p notitle ps 8 pt 7 lc rgb 'black' #change range, filenames

unset label

## %increase in 10min bin avg power

set ylabel '% increase in {/:Italic P}_{10-min,avg} (W)'
set label "Daily average power increase () : 27.89%" at "", -32 right

set output "Time-series power % increase plot 23-6 10minavg.png" #change date
plot [][0:100]"_%10min_increase.csv" u 1:2 w p notitle ps 8 pt 7 lc rgb 'black'

unset label

## 10min bin avg energy

set xtics 10800 #change if necessary (1800 for 2hrs window, 3600 for 3hrs and beyond)
set mxtics 18 #change if necessary (2 for 3 hrs+ window, 1 for 2hrs window)
set ytics 5
set mytics 1
set ylabel 'Average Energy (Wh)'
set label "Daily total energy generation for WT with FAS-3E () : 498.37 Wh\n Daily total energy generation for Bare WT () : 454.28 Wh" at "", -8 right

set output "Time-series energy plot 23-6.png" #change date
plot [][0:25]".csv" u 6:9 w p t 'WT with FAS-3E' ps 8 pt 5 lc rgb 'blue',\
".csv" u 6:9 w p t 'Bare LE-600 WT' ps 8 pt 6 lc rgb 'red' lw 6 #change range, filenames

unset label

## 24h wind speed

set terminal pngcairo enhanced background rgb 'white' size 1920, 1080 font 'Times-New-Roman, 35'
set border lw 4
set border behind
set xlabel offset 0, 0.5
set ylabel offset 0.5, 0
set tics scale 1
set size ratio 0.45
#set key reverse
#set key Left
#set key top left
set key font ',30'
#unset format x
set datafile separator ","
set xlabel '{/:Italic v}_{10-min,avg} (m/s)'
set ylabel '{/:Italic P}_{10-min,avg} (W)'
set key samplen 2
set xtics 10800
set mxtics 9
set xlabel 'Time (in hours)'
set ylabel 'Average wind speed (m/s)'
set xdata time
set timefmt "%Y-%M-%d %H:%M:%S"
set format x '%02H%02M'
unset key 
set ytics 1
set mytics 1
set xrange [" ":" "]
set label "Daily average wind speed () : 5.13 m/s" at "", -1.75 right

set output "Time-series wind speed plot 23-6 avg 10min.png" #change date
plot [][0:8]".csv" u 1:2 w p t 'Wind speed' ps 6 pt 7 lc rgb 'black' lw 4 #change range (x range must)

unset label

## 24hrs power and wind speed

set size ratio 0.5
set ylabel 'Average power (W)'
set y2label 'Average wind speed (m/s)'
set ytics nomirror
set yrange [0:150]
set ytics 30
set mytics 2
set y2range [0:10]
set y2tics 2
set my2tics 2
set y2label offset -1.5, 0
set key
#set key reverse
#set key Left
#set key top left

set output "Time-series power and wind speed plot 23-6 avg 10min.png" #change date
plot ".csv" u 6:7 w p t 'WT with FAS-3E' ps 6 pt 5 lc rgb 'blue' lw 4,\
".csv" u 6:7 w p t 'Bare WT' ps 6 pt 6 lc rgb 'red' lw 4,\
".csv" u 1:2 w p t 'Wind speed' ps 6 pt 8 lc rgb 'dark-green' lw 4 axes x1y2 #change range (x range must)

## 24hrs turbulence and wind speed

set terminal pngcairo enhanced background rgb 'white' size 3840, 2160 font 'Times-New-Roman, 45'
set border lw 6
set border behind
set xlabel offset 0, 0.5
set ylabel offset 1, 0
set xtics offset 0, 0.25
set ytics offset 0.25, 0
set xlabel 'Time (in hours)'
set ylabel '{/:Italic v}_{10-min,avg} (m/s)'
set y2label 'Turbulence intensity (%)'
set xdata time
set timefmt '%Y-%M-%d %H:%M:%S'
set size ratio 0.2
set tics scale 0.75
#set key reverse
#set key Left
#set key top left
set key font ',45'
#unset key
#set key at "2023-03-14 23:30", 9.9
set key samplen 0
set datafile separator ","
set format x '%02H%02M' 

set xtics 10800 #change if necessary (1800 for 2hrs window, 3600 for 3hrs and beyond)
set mxtics 18 #change if necessary (2 for 3 hrs+ window, 1 for 2hrs window)
set ytics 2
set mytics 2
set y2tics 20
set my2tics 2
set y2label offset -1.5, 0
set xrange ["":""]
set yrange [0:10]
set y2range [0:100]
set ytics nomirror
set label "Daily average wind speed () : 5.13 m/s\n Daily average turbulence intensity () : 29.37%" at "", -3 right

set output "Time-series speed & turbulence plot 23-6.png" #change date
plot ".csv" u 1:2 w p t '{/:Italic v}_{10-min,avg}' ps 8 pt 8 lc rgb 'forest-green' lw 6,\
".csv" u 1:4 w p t 'Turbulence intensity' ps 8 pt 6 lc rgb 'blue' lw 6 axes x1y2