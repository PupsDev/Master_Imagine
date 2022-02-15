#!/bin/bash
if [ $1 == "plotconfig" ];
then
    gnuplot -p -e "load 'config/"$1"'";montage -tile 2x -geometry +0+0 histogramme.png densite.png repartition.png bigboy.jpg;display bigboy.jpg;  
fi
if [ $1 == "plotconfigRGB" ];
then
    gnuplot -p -e "load 'config/"$1"'";montage -tile 2x -geometry +0+0 histogrammeRGB.png densiteRGB.png repartitionRGB.png bigboy.jpg;display bigboy.jpg;
fi