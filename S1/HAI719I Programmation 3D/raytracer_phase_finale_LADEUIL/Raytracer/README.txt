Compiler:

make

Executer:
./bin/main


 On peut naviguer dans la scene
 avec Z;S;Q;D et 8;2;4;6 du pavé numérique. 
On peut faire H Ou L pour haute qualité ou basse qualité.
On peut faire + et - du pavé numérique pour avoir un rendu plus rapide en calculant une résolution plus basse.

On peut charger un maillage 3d en .obj.
Il y a une sphere transparente et 2 spheres réfléchissantes.

Il y a une structure d'accélération boundary box, il manque la structure en hierarchie mais déjà on a un temps de calcul 10 fois plus rapide en moyenne.

Il y a dans le dossier des rendu suivi de leur temps de rendu en seconde comme FinalRender_28 -> 28 secondes.

(J'ai essayé de refaire mon code entre la phase 2 et finale, en essayant de recréer une librairie un peu comme glm pour l'exercice, j'espère que ça marchera bien sur votre ordinateur ! )