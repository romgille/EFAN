---
title: Introduction à la recherche 
subtitle: Extreme image completion
author:
- Huy-Hai Vincent NGUYEN
- Romain GILLE
date: \today
geometry: margin=1in
...

\newpage

# Synthèse de l'article
## Application de l'algorithme Extreme image completion
L'Extreme image completion développé par l'École Polytechnique Fédérale de Lausanne (EPFL) en Suisse est un algorithme d'inpainting. L'inpainting est une technique qui permet de restaurer des parties perdues ou détériorée d'une image ou d'une vidéo. La technique d'image complétion ne se contente pas que de restaurer une image ou vidéo mais permet également de procéder à d'autres traitements d'image comme l'**agrandissement** ou la **réduction** d'une image en conservant les échelles, la **super-résolution**, la **réduction de bruit**, le **dématriçage**, l'**élimination des artefacts**, et l'**édition d'image**.
Plusieurs algorithmes d'inpainting existent déjà comme le **Texture synthesis based image inpainting**, le **Exemplar and search based image inpainting**, le **PDE based inpainting**, le **Fast semi-automatic inpainting**, ou le **Hybrid Inpainting**.

* **Texture synthesis based image inpainting**

* **Exemplar and search based image inpainting**

* **PDE based inpainting**

* **Fast semi-automatic inpainting**

* **Hybrid Inpainting**

# Étude de l'implémentation

* compléter une image avec 1% des pixels présents
* FAN puis EFAN
* FAN : low-pass filtering approach using Gaussian filter
    * FIR (Finite Impulse Response) est possible quand une image a tous ses pixels
    * pas possible quand il manque de pixels car la 'sum of the filter coefficients' inclue les
        coefficients des pixels manquants ce qui résulte en valeurs incorrectes pour l'output.

We present a computationally efficient approach to complete
a 1% pixel image, and then we present an improvement that is
even faster. For the first method we take a low-pass filtering
approach using a Gaussian filter.
In an image containing all its pixels, the use of a separable
Finite Impulse Response (FIR) kernel in the time domain is
easy. In such a case, the result of the convolution of an image
row (or column) f at each position n by a Gaussian filter h
of finite support in the set {−M, −M + 1, ..., M − 1, M } is
given by:
(f ∗ h)[n] =
1
W
M
X
f [n − m]h[m]
(1)
adds up coefficients of missing pixels as well, resulting in in-
correct values for the filtered output. To use an FIR filter as
above, only the available pixels should be used, and W should
be accumulated for only those pixels, requiring a search oper-
ation at each pixel position. The need to search prevents the
use of computationally efficient ways of performing convolu-
tion in the frequency domain or by the use of Infinite Impulse
Response (IIR) filters.
To overcome this problem we create another signal g con-
taining ones and zeros of similar length as f such that:
(
1, if ∃f [n]
g[n] =
0, otherwise
Now, instead of searching for available values in f , we
assume zeros for the missing values, we convolve both f and
the derived signal g with the same kernel h using any tech-
nique, and obtain the desired filtered output F by performing
an element-wise division of the two convolved signals as:
F [n] =
P K
J[i] =
where
W =
k=1 w(i, k)I[k]
P K w(i, k)
k=1
h[m]
(2)
m=−M
is the sum of the filter coefficients, which is unity when the
filter coefficients are normalized.
However, in an image where most pixels are randomly
missing, when we perform such a convolution (assuming, say,
zeros for the missing pixel values), the value of W in Eq. 1
(3)
This works because the denominator signal is affected in
the same way as the numerator by zero values and therefore
adaptively compensates for it. This scheme is easily extended
to two or higher dimensions. Such an approach has been used
in the past for linearizing the bilateral filter using the bilateral
grid [17].
Another issue is to choose the standard deviation σ of the
Gaussian filter. If K pixels are uniformly randomly retained,
the average number of unknown pixels closest to each known
pixel is N/K. The value of σ is therefore p
chosen to be the
radius of a circle covering this area, i.e σ = N/πK, which
proves to be a robust heuristic in practice.
The result of filtering the entire image using the approach
of Eq. 3 can be seen in Fig. 2e. We refer to this method as the
Filtering by Adaptive Normalization (FAN) method since the
weight normalization is adapted to the presence or absence of
a value without the need for a search.
We now revisit the FAN approach and notice that we can
obtain the same result if at each unknown pixel position i, we
sum up the Gaussian-weighted contribution of each known
pixel value at position k. That is,
m=−M
M
X
(f ∗ h)[n]
(g ∗ h)[n]
2
2
(4)
where w(i, k) = e −0.5(i−k) /σ is the Gaussian weight and
J is the completed output. With this observation we can sim-
plify the image completion process of FAN.
Firstly, we only compute the contributions known pixels
can make at the unknown pixel locations. We rely on two
maps G and D (initialized to zero) of the same dimensions
as the input image. G and D accumulate, respectively, theGaussian weighted contribution and the Gaussian weight of
each known pixel value at each unknown pixel location in the
image plane. In the same way as FAN, we compute the com-
pleted image by performing an element-wise division of G
and D. Notice that we are not doing a convolution operation
any more on the entire image. Also notice that there is no sig-
nal of ones and zeros that needs to be filtered for the sake of
normalization.
Secondly, we note that beyond the 3σ distance, Gaussian
weights are negligible. We take advantage of it to restrict
the computation within a 6σ × 6σ window S centered at the
known pixels. Without this restriction, for K known pixels
the complexity is O(N K) (as in Eq. 4). By limiting the com-
putation to window S, whose size is proportional to σ 2 , the
complexity becomes O(σ 2 K) i.e., O(N ). In other words, the
computation is linear in N and independent of K. We call this
method Efficient Filtering by Adaptive Normalization (EFAN)
since it is more than twice as fast as FAN (Fig. 2).
Algorithm 1 The EFAN algorithm.
Require: Single channel input image I containing K values,
list L of indices of the K values, weighted pixel value ac-
cumulation map G, weight accumulation map D, output
image
p J to contain the completed image.
1: σ =
N/πK
2: Initialize G and D to zero.
3: for k ∈ L do
4:
Define window S centered at k
5:
for i ∈ S do
2
2
6:
w(i, k) = e −0.5(i−k) /σ
7:
D[i] = D[i] + w(i, k)
8:
G[i] = G[i] + w(i, k)I[k]
9:
end for
10: end for
11: for i ∈ I do
G[i]
12:
J[i] = D[i]
13: end for
These two steps of using only the known pixel values, and
restricting the computation to a small window, drastically re-
duce the computation as well as provide O(N ) complexity
for any number of missing pixels. Effectively, we turn the
FAN solution around from performing a convolution opera-
tion at each unknown pixel location to only performing the
computation in restricted square window regions around the
known pixels. Algorithm 1 lays out the steps for a practi-
cal implementation 1 of EFAN. Color images are processed
by simply applying Algorithm 1 to each color channel sepa-
rately. It is worth noting that our algorithm easily extends to
higher-dimensional data like videos or image stacks.
Even though the two solutions FAN and EFAN are equiv-
alent, in practice EFAN outputs exhibit lower errors (see
Fig. 2). This is because efficient convolution performed using
IIR or frequency-domain filtering may lead to minor artifacts
at the image borders.

# Expérimentations
# Sources 
* [Review of Different Inpainting Algorithms - International Journal of Computer Applications, Décembre 2012](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.303.5459&rep=rep1&type=pdf)

* [What is the difference between image super resolution and image scaling - Quora, 18 Décembre 2014](https://www.quora.com/What-is-the-difference-between-image-superresolution-and-image-scaling)
