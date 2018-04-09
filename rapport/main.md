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
## Application de l'algorithme Efficient Filtering by Adaptive Normalization
L'Efficient Filtering by Adaptive Normalization (EFAN) développé par l'École Polytechnique Fédérale
de Lausanne (EPFL) en Suisse est un algorithme d'inpainting. L'inpainting est une technique qui 
permet de restaurer des parties perdues ou détériorée d'une image ou d'une vidéo. La technique 
d'image complétion ne se contente pas que de restaurer une image ou vidéo mais permet également de 
procéder à d'autres traitements d'image comme l'**agrandissement** ou la **réduction** d'une image 
en conservant les échelles, la **super-résolution**, la **réduction de bruit**, le **dématriçage**,
l'**élimination des artefacts**, et l'**édition d'image**.
Plusieurs algorithmes d'inpainting existent déjà comme le **Texture synthesis based image 
inpainting**, le **Exemplar and search based image inpainting**, le **PDE based inpainting**, le 
**Fast semi-automatic inpainting**, ou le **Hybrid Inpainting**.

* **Texture synthesis based image inpainting**

* **Exemplar and search based image inpainting**

* **PDE based inpainting**

* **Fast semi-automatic inpainting**

* **Hybrid Inpainting**

Le principal avantage de l'EFAN contrairement aux autres algorithmes qui sont très gourmand en
ressources, est sa complexité en  $O(n) $ avec $ n $, le nombre de pixels contenus dans l'image 
finale, et la complexité de l'algorithme est indépendante de l'état de détérioration de l'image.

## Applications
* **Super-résolution**
* **Dématriçage**

## Mesures de la qualité d'une image
* **Mean-Squared Error - MSE**
	$$ e_{MSE} = \frac{1}{M N} \sum_{n=1}^{M} \sum_{m=1}^{N}[\hat{g}(n, m) - g(n, m)]^2 $$
* **Peak Signal-to-Noise Ratio - PSNR**
	$$ PSNR = -10\log_{10}\frac{e_{MSE}}{S^2} $$
* **Structural SIMilarity - SSIM**


# Étude de l'implémentation
# Expérimentations
# Sources 
* [Review of Different Inpainting Algorithms - International Journal of Computer Applications, Décembre 2012](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.303.5459&rep=rep1&type=pdf)
* [What is the difference between image super resolution and image scaling - Quora, 18 Décembre 2014](https://www.quora.com/What-is-the-difference-between-image-superresolution-and-image-scaling)
* [Measures of image quality -  Todd Veldhuizen, 16 Janvier 1998](http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/VELDHUIZEN/node18.html)
* [Mean squared error - Wikipedia](https://en.wikipedia.org/wiki/Mean_squared_error)
* [Peak signal-to-noise ratio - Wikipedia](https://en.wikipedia.org/wiki/Peak_signal-to-noise_ratio)
* [Structural Similarity - Wikipedia](https://en.wikipedia.org/wiki/Structural_similarity)
