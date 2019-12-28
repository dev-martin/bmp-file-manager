#!/bin/usr/gnuplot
# Iria Estevez Ayres
#
# Recibe un argumento, fichero .csv 4 columnas: Intensidad Rojo Verde Azul
# Se debe llamar en la forma
# gnuplot -e "inputfile='nombreficherodatos.csv'" -e "outputfile='fichero.png'" pinto_hist.gp
#
# No realiza control de si el fichero existe o no, sÃ³lo intenta dibujarlo
#
# Guarda el fichero en ficherosalida (que serÃ¡ un eps)
# Ejemplo de invocaciÃ³n:
#
#gnuplot -e "inputfile='histograma.csv'" -e "outputfile='hist.png'" pinto_hist.gp
#


plot for [COL=2:4] inputfile using COL title columnheader with lines
set terminal png
set output outputfile
replot
