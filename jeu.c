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
    int partie_en_cours = 1;

    while (partie_en_cours!=0)
    {
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
        char* nom_fichier = "english_full.txt";//"liste_complete_triee.txt";
        int taille_test = 15919;
        char* mots[taille_test];
        int nb_mots = extraction_mots(mots, nom_fichier, nb_lettres);
        printf("nb_mots : %d\n", nb_mots);

        //CHOIX ALEATOIRE DU MOT A DEVINER
        char* mot_cible = choix_mot(mots,nb_mots);

        switch (mode)
        {
            case 0:
                partie_joueur(max_essais, nb_lettres, mot_cible, mots, nb_mots);
                break;
            case 1: //Bot entropique Otanpi qui peut tenter des mots non compatibles
                printf("mot_cible : %s\n", mot_cible);
                partie_bot1(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier); 
                break;
            case 2: //Bot entropique Otanpi qui ne peut tenter que des mots compatibles
                printf("mot_cible : %s\n", mot_cible);
                partie_bot2(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier); 
                break;
            case 3: //Bot naïf à mémoire
                printf("mot_cible : %s\n", mot_cible);
                partie_bot3(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier);
                break;
            case 4: //Bot Train-Coule x Otanpi mots compatibles
                printf("mot_cible : %s\n", mot_cible);
                partie_bot4(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier);
                break;
            case 5: //Bot Train-Coule x naïf à mémoire
                printf("mot_cible : %s\n", mot_cible);
                partie_bot4(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier);
                break;
        }
    
    printf_blanc();
    printf("\nVoulez-vous rejouer ? (Yes : 1, No : !=1)\n\n-> ");
    scanf("%d", &partie_en_cours);
    printf_standard();
    }

    return 0;
}