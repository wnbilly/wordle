Wordle

Présentation du projet :
L'essence du projet est d'implémenter une version du jeu en console, puis d'utiliser la théorie de l'information afin de coder un (ou plusieurs) ordinateur qui joue le "meilleur" mot à chaque fois. Si plusieurs ordinateurs ont été codés, une comparaison d’efficacité sera possible.


Ligne de compilation :
gcc -Wall -Werror -Wfatal-errors -o JEU jeu.c acquisition.c tests_lettres.c calculs.c entropie.c parties.c -lm && ./JEU


Règles du jeu :
Le jeu Wordle donne aux joueurs six chances de deviner un mot de cinq lettres, choisi au hasard. Si vous avez la bonne lettre au bon endroit, elle apparaît en vert. Une lettre correcte au mauvais endroit sortira en jaune. Le joueur doit utiliser ces informations à bon escient afin de deviner le bon mot.