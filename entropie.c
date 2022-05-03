#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"
#include "entropie.h"

//Code contenant les fonctions nécessaires au calcul des probabilités

/*Compile avec 

gcc -Wall -Werror -Wfatal-errors -o ENTROPIE entropie.c calculs.c acquisition.c tests_lettres.c -lm && ./ENTROPIE

*/

//Remplit la liste de mots compatibles avec les données parmi les mots_a_tester et renvoie le nombre de mots compatibles
int liste_mots_prob(char* mots_probables[], char* mots_a_tester[], int nb_mots_a_tester, struct donnees* data)
{
    int indice_mot_prob = 0;
    for (int i=0; i<nb_mots_a_tester; i++)
    {
        if (verif_compatibilite(mots_a_tester[i], data) == 1)
        {
            mots_probables[indice_mot_prob] = mots_a_tester[i];
            indice_mot_prob++;
        }
    }
    return indice_mot_prob-1; //Renvoie le nombre de mots compatibles
}

//Retourne le nombre de mots compatibles avec les données
int nombre_mots_prob(char* mots_a_tester[], int nb_mots_a_tester, struct donnees* data)
{
    int nb_mots_prob = 0;
    for (int i=0; i<nb_mots_a_tester; i++)
    {
        if (verif_compatibilite(mots_a_tester[i], data) == 1)
        {
            nb_mots_prob++;
        }
    }
    return nb_mots_prob-1; //Renvoie le nombre de mots compatibles
}


float calcul_entropie_mot(char* mot, char* mots_a_tester[], int nb_mots_a_tester, int** liste_patterns) //FIX NB DE MOTS A TESTER
{
    //int nb_lettres = 5;
    
    float h = 0;
    float p = 0;

    for (int j = 0; j<243; j++)
    {
        struct donnees* data = init_data();
        //affichage_debug(liste_patterns[j],5);
        extraction_donnees(mot, liste_patterns[j], data);
        //printf("%s\n",mot);
        //printf("nb_mots_probables : %d\n", nombre_mots_prob(mots_a_tester, nb_mots_a_tester, data));
        p = ((float) nombre_mots_prob(mots_a_tester, nb_mots_a_tester, data)+1)/nb_mots_a_tester;
        //printf("p = %f\n",p);
        if (p!=0) h = h-p*log(p);
        free(data);
    }
    printf("H(%s) = %f\n", mot, h);
    return h;
}


void init_matrix(int ***matrix, int n, int p) //Pour l'utiliser, on initialise int** maxtrix;
{
    *matrix = malloc(sizeof(int *) * n);

    for (int i = 0; i < n; i++)
    {
        *(*matrix + i) = malloc(sizeof(int) * p);
    }
}

float trouver_mot_h_max(char* mots_a_tester[], int nb_mots_a_tester, char mot_h_max[], int** liste_patterns)
{
    float h_max = 0;
    for (int k=0; k<nb_mots_a_tester; k++)
    {
        float h_temp = calcul_entropie_mot(mots_a_tester[k], mots_a_tester, nb_mots_a_tester, liste_patterns);
        if (h_temp>h_max)
        {
            h_max = h_temp;
            mot_h_max = mots_a_tester[k];
        }
    }
    return h_max;
}

void creation_liste_patterns(int nb_lettres, int** liste_patterns) //OU BASE 3
{
    for (int n=0; n<243;)
    {
        //printf("pattern %d :\n",n+1);
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                for (int k=0; k<3; k++)
                {
                    for (int l=0; l<3; l++)
                    {
                        for (int m=0; m<3; m++)
                        {
                            liste_patterns[n][0] = i;
                            liste_patterns[n][1] = j;
                            liste_patterns[n][2] = k;
                            liste_patterns[n][3] = l;
                            liste_patterns[n][4] = m;
                            n++;
                            //affichage_debug(liste_patterns[n],5);
                        }
                    }
                }
            }
        }
    }
}

void affichage_liste_patterns(int **liste_patterns)
{
    printf("=============\n");
    for (int i=0; i<243; i++)
    {
        affichage_debug(liste_patterns[i], 5);
    }
    printf("=============\n");
}

int main(int argc, char* argv[])
{
    //FAIRE STRUCT DONNEES

    struct donnees* data = malloc(sizeof(struct donnees));
    
    data->nb_lettres = 5;
    data->lst_lettres = malloc(data->nb_lettres*sizeof(char));
    data->lst_etats = calloc(data->nb_lettres, sizeof(int));
    data->lst_pos = malloc(data->nb_lettres*sizeof(int));
    data->lst_pos[0] = -1;
    data->lst_pos[1] = -1;
    data->lst_pos[2] = -1;
    data->lst_pos[3] = -1;
    data->lst_pos[4] = -1;
    //data->lst_lettres_ban = malloc(26*sizeof(char));
    //data->occ_ban = calloc(data->nb_lettres, sizeof(int));

    char* nom_fichier = "liste_complete_triee.txt";

    int taille_test = 7000;
    char* mots_a_tester[taille_test];
    char* mots_probables[taille_test];

    int nb_mots_a_tester = extraction_mots(mots_a_tester, nom_fichier, data->nb_lettres);

    char* essai = choix_mot(mots_a_tester, nb_mots_a_tester);
    int resultat[5] = {0,1,0,2,0};

    int** liste_patterns;
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(data->nb_lettres, liste_patterns);

    affichage_resultat(essai, resultat, 5);

    extraction_donnees(essai, resultat, data);

    affichage_donnees(data);

    int nb_mots_probables = liste_mots_prob(mots_probables, mots_a_tester, nb_mots_a_tester, data);

    printf("nb_mots_probables : %d\n", nb_mots_probables+1);

    
    //affichage_tableau_mots(mots_probables, nb_mots_probables);
    
    char mot_h_max[5];

    float h_max = trouver_mot_h_max(mots_a_tester, nb_mots_a_tester, mot_h_max, liste_patterns);

    printf("Mot entropie max est %s avec %f\n", mot_h_max, h_max);
    

    free(*liste_patterns);

    return 0;
}