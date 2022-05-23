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

//Pour lire les logs : gedit --encoding=UTF-8 file.txt

//Remplit la liste de mots compatibles avec UN struct donnees parmi les mots_a_tester et renvoie le nombre de mots compatibles
int liste_mots_prob1(char* mots_probables[], char* mots_a_tester[], int nb_mots_a_tester, struct donnees* data)
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
    if (indice_mot_prob==0) return 0;
    else return indice_mot_prob; //Renvoie le nombre de mots compatibles
}


//Remplit la liste mots_probables avec l'ensemble des données parmi les mots_a_tester et renvoie le nombre de mots probables
int liste_mots_prob(char* mots_probables[], char* mots_a_tester[], int nb_mots_a_tester, struct donnees* all_data[], int nb_essais)
{
    int indice_mot_prob = 0;
    for (int i=0; i<nb_mots_a_tester; i++)
    {
        if (verif_compatibilite_complete(mots_a_tester[i], all_data, nb_essais) == 1)
        {
            mots_probables[indice_mot_prob] = mots_a_tester[i];
            indice_mot_prob++;
        }
    }
    if (indice_mot_prob==0) return 0;
    else return indice_mot_prob; //Renvoie le nombre de mots compatibles
}

//Retourne le nombre de mots compatibles avec un struct donnees
int nombre_mots_prob1(char* mots_a_tester[], int nb_mots_a_tester, struct donnees* data)
{
    int nb_mots_prob = 0;
    for (int i=0; i<nb_mots_a_tester; i++)
    {
        if (verif_compatibilite(mots_a_tester[i], data) == 1)
        {
            /* VERIF DES TESTS DE LETTRES
            printf("%s comp avec data :\n", mots_a_tester[i]);
            affichage_donnees(data);
            */
            nb_mots_prob++;
        }
    }
    //printf("nb_mots_prob=%d\n",nb_mots_prob);
    return nb_mots_prob; //Renvoie le nombre de mots compatibles
}

//Retourne le nombre de mots compatibles avec un ensemble de struct donnees
int nombre_mots_prob(char* mots_a_tester[], int nb_mots_a_tester, struct donnees* all_data[], int nb_essais)
{
    int nb_mots_prob = 0;
    for (int i=0; i<nb_mots_a_tester; i++)
    {
        if (verif_compatibilite_complete(mots_a_tester[i], all_data, nb_essais) == 1)
        {
            /* VERIF DES TESTS DE LETTRES
            printf("%s comp avec data :\n", mots_a_tester[i]);
            affichage_donnees(data);
            */
            nb_mots_prob++;
        }
    }
    //printf("nb_mots_prob=%d\n",nb_mots_prob);
    return nb_mots_prob; //Renvoie le nombre de mots compatibles
}

//Calcule l'entropie de mot relativement aux mots_a_tester et au nombre de mots a tester
float calcul_entropie_mot(char* mot, char* mots_a_tester[], int nb_mots_a_tester, int** liste_patterns) //FIX NB DE MOTS A TESTER
{
    //int nb_lettres = 5;
    float h = 0;
    float p = 0;
    for (int j = 0; j<243; j++)
    {
        struct donnees* data = init_data();
        extraction_donnees(mot, liste_patterns[j], data);
        p = ((float) nombre_mots_prob1(mots_a_tester, nb_mots_a_tester, data))/nb_mots_a_tester;
        if (p!=0) h = h-p*log2f(p);
        //free_data(data); //enlever ce free ne crée pas de fuite de mémoire mais donne des résultats incohérents
    }
    //printf("H(%s) = %f\n", mot, h);
    return h;
}

//Initialise une matrice allouée dynamiquement de taille n*p
void init_matrix(int ***matrix, int n, int p) //Pour l'utiliser, on initialise int** maxtrix;
{
    *matrix = malloc(sizeof(int *) * n);

    for (int i = 0; i < n; i++)
    {
        *(*matrix + i) = malloc(sizeof(int) * p);
    }
}

//Free matrice n*p
void free_matrix(int **matrix, int n, int p)
{
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    //free(*matrix); //malloc_consolidate() error si décommenté
}

//Calcul entropie en testant les mots du gros dico et en trouvant les mots compatibles dans la liste des mots compatibles
float trouver_mot_h_max(char* dico[], int nb_mots_dico, char* mots_a_tester[], int nb_mots_a_tester, char* mot_h_max, int** liste_patterns)
{
    float h_max = -1; //On commence à -1 car s'il ne reste qu'un mot, son entropie sera 0>1
    for (int k=0; k<nb_mots_dico; k++)
    {
        //printf("%d | ",k);
        float h_temp = calcul_entropie_mot(dico[k], mots_a_tester, nb_mots_a_tester, liste_patterns);
        if (h_temp>h_max)
        {
            h_max = h_temp;
            strcpy(mot_h_max, dico[k]);
            //printf("%s nv max : %f\n",mot_h_max, h_max);
        }
    }
    //printf("mot_h_max : %s avec %f\n", mot_h_max, h_max);
    return h_max;
}

//Remplit liste _patterns avec la liste des 243 patterns
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