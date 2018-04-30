#!/bin/sh

MATLAB=/usr/local/MATLAB/bin/matlab
FILE=src/demo.m

$MATLAB -nodisplay -nosplash -nodesktop -r "run('src/demo')"
