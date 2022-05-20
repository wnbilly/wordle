#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parties.h"
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"
#include "entropie.h"

//Main du Wordle
/*Compile avec 
gcc -Wall -Werror -Wfatal-errors -o JEU jeu.c acquisition.c tests_lettres.c calculs.c entropie.c parties.c -lm && ./JEU
*/

int main(int argc, char* argv[])
{
    srand(time(0));

    //AFFICHAGE REGLES ET BIENVENUE ETC

    //PARAMETRES DE LA PARTIE
    int max_essais;
    int nb_lettres;
    int mode;
    
    affichage_regles();

    mode = choix_mode();

/*  Pour pouvoir modifier le nombre de lettres et d'essais
    printf("Nombre de lettres dans [4;12] :");
    
    scanf("%d", &nb_lettres);

    printf("Nombre d'essais : ");
    
    scanf("%d", &max_essais);
*/

    nb_lettres = 5;
    max_essais = 6;

    //ACQUISITION MOTS
    char* nom_fichier = "liste_complete_triee.txt";
    int taille_test = 5070;
    char* mots[taille_test];
    int nb_mots = extraction_mots(mots, nom_fichier, nb_lettres);

    //CHOIX ALEATOIRE DU MOT A DEVINER
    char* mot_cible = choix_mot(mots,nb_mots);

    switch (mode)
    {
        case 0:
            partie_joueur(max_essais, nb_lettres, mot_cible, mots, nb_mots);
            break;
        case 1: //Bot entropique qui peut tenter des mots non compatibles
            printf("mot_cible : %s\n", mot_cible);
            partie_bot1(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier); 
            break;
        case 2: //Bot entropique qui ne peut tenter que des mots compatibles
            printf("mot_cible : %s\n", mot_cible);
            partie_bot2(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier); 
            break;
        case 3: //Bot aléatoire à mémoire
            printf("mot_cible : %s\n", mot_cible);
            partie_bot3(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier);
            break;
        case 4: //Bot mixte
            printf("mot_cible : %s\n", mot_cible);
            partie_bot4(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier);
            break;
    }

    return 0;
}