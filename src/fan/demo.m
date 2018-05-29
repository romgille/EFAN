%======================================================================
% Extreme image completion demo
% Copyright (C) 2016 Ecole Polytechnique Federale de Lausanne
% File created by Radhakrishna Achanta
% This file is meant to demonstrate the extreme imgage completion
% algorithm called Filtering by Adaptive Normalization (FAN) presented
% the paper:
%
% "Extreme Image Completion", Radhakrishna Achanta, Nikolaos
% Arvanitopoulos, Sabine Susstrunk. ICASSP 2017, New Orleans, USA.
% 
%======================================================================
%Inputs for fan_func function are:
%
%[1] 8 bit images (color or grayscale)
%[2] Mask of ones and zeros
%
%Ouput:
%[1] The completed image
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
imwrite(M(img), '../../img/mask_fan.png');
%--------------------------------------------------------------------------
tic
outimg = fan_func(img,uint8(M));% FAN (EFAN is about twice as fast)
toc
%--------------------------------------------------------------------------
imwrite(outimg, '../../out/lena_fan.png');
imshow(outimg);

exit;
