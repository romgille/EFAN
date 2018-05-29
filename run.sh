#!/bin/sh

# change path to matlab bin
MATLAB=/usr/local/MATLAB/bin/matlab
EFAN=src/efan
FAN=src/fan
TV=src/tv
IMG=../../img
OUT=../../out

if [ $# -eq 0 ]; then
    echo "-----------------------------------------------------------"
    echo "You have to put one of the following arguments: "
    echo "- all : run all the programs"
    echo "- efan : run the EFAN program"
    echo "- fan : run the FAN program"
    echo "- tv : run the TVINPAINT program"
    echo "- clean : clean the project"
    echo "----------------"
    echo "You can also add a second argument as an input of programs"
    echo "Just write the name of the file you have in the img/ folder"
    echo "This option does not work for EFAN nor FAN, you have to change the"
    echo "input file in src/{efan,fan}/demo.m file."
    echo "------------------------------------------------------------"
fi

if [[ $1 == "clean" ]]; then
    echo "---------------------------------------------"
    echo "-----Removing old outputs and make clean-----"
    echo "---------------------------------------------"
    rm -v out/* img/mask*.png img/masked*.png
    cd $TV && make clean && cd ../..
fi

if [[ $1 == "efan" || $1 == "all" ]]; then
    echo "---------------------------------------------"
    echo "----------------Running EFAN-----------------"
    echo "---------------------------------------------"

    # run the efan program with matlab and options specified in src/efan/demo.m
    $MATLAB -nodisplay -nosplash -nodesktop -sd $EFAN -r "run('demo')"
fi

if [[ $1 == "fan" || $1 == "all" ]]; then
    echo "---------------------------------------------"
    echo "----------------Running FAN------------------"
    echo "---------------------------------------------"

    # run the efan program with matlab and options specified in src/efan/demo.m
    $MATLAB -nodisplay -nosplash -nodesktop -sd $FAN -r "run('demo')"
fi

if [[ $1 == "tv" || $1 == "all" ]]; then
    echo "---------------------------------------------"
    echo "----------------Running TV-------------------"
    echo "---------------------------------------------"

    cd $TV && make -j

    if [ $# -eq 2 ]; then
        # create the random mask of pixels with 0.01% of the image
        ./randmask Bernoulli:0.95 $IMG/$2 $IMG/mask.png

        # apply the mask on masked.png
        ./applymask $IMG/$2 $IMG/mask.png $IMG/masked.png
    else
        # create the random mask of pixels with 0.01% of the image
        ./randmask Bernoulli:0.95 $IMG/lena.png $IMG/mask.png

        # apply the mask on masked.png
        ./applymask $IMG/lena.png $IMG/mask.png $IMG/masked.png

    fi
    time ./tvinpaint $IMG/mask.png 1e3 $IMG/masked.png $OUT/lena_tv.bmp

    cd ../..
fi
