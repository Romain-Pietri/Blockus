# Blockus boudi bouda


## compilation

Le projet utilise Cmake, pour compiler faire `make` dans le fichier src


## Request serveur 

|id | meaning | format du socket : id_joueur + "|
|----------|:-------------:|:------:|
|0 |init|pseudo|
|1 | Wait | $\empty$|
|2| Jouer|nb_piece_jouer, Grille|
|3| Plus jouer| $\empty$|
|4|classment|$\empty$|
|5|quitter|$\empty$|

## Reponse serveur

|id | meaning | format du socket|
|----------|:-------------:|:------:|
|0 |init|id_joueur|
|1 | Wait | jouer ? , Nom_joueurs|
|2| Jouer|nb_piece_jouer, Grille|
|3| Plus jouer| $\empty$|
|4|classment|id_joueur, score|
|5|quitter|$\empty$|


