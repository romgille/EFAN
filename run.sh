#!/bin/sh

MATLAB=/usr/local/MATLAB/bin/matlab
EFAN=src/efan
TV=src/tv
IMG=../../img
OUT=../../out

echo "------------------------------"
echo "-----Removing old outputs-----"
echo "------------------------------"
rm -v out/*
cd $TV && make clean && cd ../..

echo "------------------------------"
echo "---------Running EFAN---------"
echo "------------------------------"

$MATLAB -nodisplay -nosplash -nodesktop -sd $EFAN -r "run('demo')"

echo "------------------------------"
echo "---------Running TV-----------"
echo "------------------------------"

cd $TV && make

./randmask Bernoulli:0.01 $IMG/lena.png $IMG/mask.png

./applymask $IMG/lena.png $IMG/mask.png $IMG/masked.png

./tvinpaint $IMG/mask.png 100 $IMG/masked.png $OUT/lena_tv.bmp

cd ../..
