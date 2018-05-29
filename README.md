# EFAN
Introduction à la recherche sur le thème de la complétion d'image

## Structure du dépot

Le dépot contient deux dossiers de sources (`src` et `rapport`), un dossier
`img` pour les entrées et deux dossiers de sorties (`out` et `pdf`).

* `src` contient les codes sources des différents programmes testés :
    * `efan`
    * `fan`
    * `tv`
* `rapport` contient les sources du rapport
* `img` contient les images d'entrées pour l'exécution des programmes
* `out` contient les images de sorties des différents programmes sous la forme
    `out_`$programme$`.png`
* `pdf` contient le rapport au format pdf


## Utilisation des programmes

Il est possible de lancer les différents programmes depuis le fichier
`run.sh` à la racine du dépot. Les arguments à lui passer sont décrits en
exécutant ce fichier depuis un terminal shell.

Il est également possible de lancer les programmes directement depuis leurs
dossiers respectifs. (ex: `src/efan`)
