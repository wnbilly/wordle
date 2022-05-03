#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"

//Code contenant les fonctions nécessaires au calcul des probabilités


//retourne la liste de mots compatibles avec les données parmi les mots_a_tester
int liste_mots_prob(char* mots_probables[], char* mots_a_tester[], int nb_mots, struct donnees* data)
{
    int indice_mot_prob = 0;
    for (int i=0; i<nb_mots; i++)
    {
        char* mot_test = mots_a_tester[i];
        if (correspondance_ltr_jaune(mot_test, data)==1 && correspondance_ltr_verte(mot_test, data)==1 && test_ltr_ban(mot_test, data)==1)
        {
            mots_probables[indice_mot_prob] = mot_test;
            indice_mot_prob++;
        }
    }
    return indice_mot_prob-1;
}

//Retourne le nombre de mots compatibles avec mot+pattern
//A REVOIR
int nombre_mots_prob(char* mot, int pattern[], char* mots_a_tester[], int nb_mots_test)
{
    int nb_prob = 0;
    //int nb_lettres = 5;
    for (int i=0; i<nb_mots_test; i++)
    {
        char* mot_test = mots_a_tester[i];
        struct donnees* data = malloc(sizeof(struct donnees));
        extraction_donnees(mot_test, pattern, data);
        if (correspondance_ltr_jaune(mot_test, data)==1 && correspondance_ltr_verte(mot_test, data)==1 && test_ltr_ban(mot_test, data)==1)
        {
            nb_prob++;
        }
    }
    return nb_prob;
}


int** creation_liste_patterns(int nb_lettres) //OU BASE 3
{
    int** liste_patterns = calloc(5*243, sizeof(int));
    for (int n=0; n<243; n++)
    {
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
                        }
                    }
                }
            }
        }
    }
    return liste_patterns;
}


float calcul_entropie_mot(char* mot, int nb_mots, char* mots_a_tester[])
{
    int nb_lettres = 5;
    int** liste_patterns = creation_liste_patterns(nb_lettres);
    float h = 0;
    float p = 0;
    for (int j = 0; j<243; j++)
    {
        int* pattern = liste_patterns[j];
        p = nombre_mots_prob(mot, pattern, mots_a_tester, nb_mots)/nb_mots;
        h = h+p*log(p);
    }
    return h;
}

int trouver_mot_h_max(char* mots_a_tester[], int nb_mots_a_tester, struct donnees* data, char mot_h_max[])
{
    float h_max = 0;
    for (int k=0; k<nb_mots_a_tester; k++)
    {
        float h_temp = calcul_entropie_mot(mots_a_tester[k],nb_mots_a_tester, mots_a_tester);
        if (h_temp>h_max)
        {
            h_max = h_temp;
            mot_h_max = mots_a_tester[k];
        }
    }
    return h_max;
}


int main(int argc, char* argv[])
{
    char* essai = "porte";
    
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
    char* mots[taille_test];
    char* mots_probables[taille_test];
    int nb_mots_test = sizeof(mots)/sizeof(essai);
    char mot_h_max[5];

    int nb_mots = extraction_mots(mots, nom_fichier, data->nb_lettres);

    trouver_mot_h_max(mots, nb_mots_test, data, mot_h_max);

    printf("mot_hç_max est %s", mot_h_max);

    int nb_mots_probables = liste_mots_prob(mots_probables, mots, nb_mots, data);

    printf("nb_mots_probables : %d", nb_mots_probables);