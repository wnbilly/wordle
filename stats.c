#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "jeu.h"
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"
#include "entropie.h"


//Programme destiné à la création de statistiques sur les différents bots

/* Compile avec
gcc -Wall -Werror -Wfatal-errors -o STATS stats.c jeu.c entropie.c calculs.c acquisition.c tests_lettres.c -lm && ./STATS
*/

void stats_bot2(int max_essais, int nb_lettres, char* mots[], int nb_mots, char* nom_fichier, int taille_echantillon)
{
    float moyenne_nb_essais = 0;
    int nb_gagne = 0;
    srand(time(0));
    int random;
    clock_t t1 = clock();

    for (int i=0; i<taille_echantillon; i++)
    {
        random = rand()%nb_mots;
        int nb_essais_partie = partie_bot2(max_essais, nb_lettres, mots[random], mots, nb_mots, nom_fichier);

        if (nb_essais_partie > 0)
        {
            nb_gagne++;
            moyenne_nb_essais = moyenne_nb_essais + nb_essais_partie;
        }
    }
    moyenne_nb_essais = moyenne_nb_essais/nb_gagne;

    clock_t t2=clock();
    float temps = (float)(t2-t1);
    printf_rouge();
    printf("Temps d'exécution : %f s\n", temps);
    printf("Sur %d parties, le bot2 en a gagné %d en %.3f essais en moyenne.\n", taille_echantillon, nb_gagne, moyenne_nb_essais);
    printf_standard();
}

int main(int argc, char* argv[])
{
    int nb_lettres = 5;
    int max_essais = 6;

    //ACQUISITION MOTS
    char* nom_fichier = "liste_complete_triee.txt";
    int taille_test = 5070;
    char* mots[taille_test];
    int nb_mots = extraction_mots(mots, nom_fichier, nb_lettres);

    int taille_echantillon = 50;

    stats_bot2(max_essais, nb_lettres, mots, nb_mots, nom_fichier, taille_echantillon);

    return 0;
}