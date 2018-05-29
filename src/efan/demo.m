%======================================================================
% Extreme image completion demo
% Copyright (C) 2016 Ecole Polytechnique Federale de Lausanne
% File created by Radhakrishna Achanta
% Please also read the copyright notice in the file efan_mex.cpp
%======================================================================
%Input parameters for efan_mex function are:
%
%[1] 8 bit images (color or grayscale)
%[2] Mask of ones and zeros
%
%Ouputs are:
%[1] The completed image
%
%NOTES:
%[2] Before using this demo code you must compile the C++ file usingthe command:
% mex efan_mex.cpp
% ==========================================
close all;

img = imread('../../img/lena.png');

percentage = 0.05;%choose percentage of regained pixels

s = size(img);
sz = s(1)*s(2);
rng('default');%choose seed for random number generator
%row vector containing sz unique integers selected randomly from 1 to sz inclusive.
randvec = randperm(sz,sz);
%round to nearest int
numpixels = round(sz*percentage);
randind = randvec(1:numpixels);
M = zeros([s(1) s(2)]);%mask
M(randind) = 1;
imwrite(M(img), '../../img/mask.png');
%--------------------------------------------------------------------------
tic
outimg = efan_mex(img,M);
toc
%--------------------------------------------------------------------------
imwrite(outimg, '../../out/lena_efan.png');
imshow(outimg);

exit;
