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
int liste_mots_prob(char* mots_probables[], char* mots_a_tester[], int nb_mots_a_tester, struct donnees* all_data[], char* mots_bannis[], int nb_essais)
{
    int indice_mot_prob = 0;
    for (int i=0; i<nb_mots_a_tester; i++)
    {
        if (verif_compatibilite_complete(mots_a_tester[i], all_data, nb_essais-1) == 1 && mot_est_dans(mots_bannis, nb_essais, mots_a_tester[i])==0)
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
        /*pour verif valeurs
        printf("%s\n",mot);
        affichage_debug(liste_patterns[j],5);
        affichage_donnees(data);
        */
        //printf("nb_mots_probables : %d\n", nombre_mots_prob1(mots_a_tester, nb_mots_a_tester, data));
        p = ((float) nombre_mots_prob1(mots_a_tester, nb_mots_a_tester, data))/nb_mots_a_tester;
        //printf("p = %f\n",p);
        if (p!=0) h = h-p*log2f(p); //Je note h mais on calcule l'espérance de l'entropie h en réalité
        //free_data(data); //enlever ce free ne crée pas de fuite de mémoire
    }
    //printf("H(%s) = %f\n", mot, h);
    return h;
}

//gedit --encoding=UTF-8 file.txt

//roula 4.324048 avec free_data(data) mis
//racla 3.489402 avec free_data log.log
//renee 9.136898 sans free_data log1.log
//perte 10.589965 sans free_data log2.log

//aeree 9.114610 sans free_data log3.log
//renee 9.346533 sans free_data log4.log
//renee 9.244974 sans free_data log5.log avec aeree à 9.098337

//Observations : certains mot (comme tarie) ont la même entropie lors des runs 3 4 5 : H(tarie) = 7.857382

//après modif de extraction_donnees
//aeree 9.244666 sans free_data log6.log, H(tarie) = 7.857382
//aeree 9.244666 sans free_data log7.log, H(tarie) = 7.857382

//log changé en log2
//aeree 13.337233 sans free_data log8.log
//nanan 9.734656 avec free_data log9.log
//nanan 9.734656 avec free_data log10.log    les entropies sont égales pour chaque mot avec free_data

//test_lettre et test_ltr_jaune corrigés

//correspondance_ltr_jaune amélioré (trace des lettres vertes)
//aeree 10.334809 dans nvlog1.log   H(tarie) = 6.329237     H(TARIE) = 6.32896        H(AEREE) = 5.00145

void init_matrix(int ***matrix, int n, int p) //Pour l'utiliser, on initialise int** maxtrix;
{
    *matrix = malloc(sizeof(int *) * n);

    for (int i = 0; i < n; i++)
    {
        *(*matrix + i) = malloc(sizeof(int) * p);
    }
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
            //*mot_h_max = mots_a_tester[k];
            printf_rouge();
            //printf("%s nv max : %f\n",mot_h_max, h_max);
            printf_standard();
        }
    }
    printf("mot_h_max : %s avec %f\n", mot_h_max, h_max);
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

int mainen(int argc, char* argv[])
{
    //FAIRE STRUCT DONNEES

    //struct donnees* data = init_data();
    
    char* nom_fichier = "liste_complete_triee.txt";

    int taille_test = 7000;
    char* mots_a_tester[taille_test];
    //char* mots_probables[taille_test];

    int nb_mots_a_tester = extraction_mots(mots_a_tester, nom_fichier, 5);

    int** liste_patterns;
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    /* TEST extraction_donnees + nombre_mots_prob1
    affichage_resultat(essai, resultat, 5);

    extraction_donnees(essai, resultat, data);

    affichage_donnees(data);

    int nb_mots_probables = liste_mots_prob1(mots_probables, mots_a_tester, nb_mots_a_tester, data);

    printf("nb_mots_probables : %d\n", nb_mots_probables+1);

    */

    //affichage_tableau_mots(mots_probables, nb_mots_probables);
    
    

    calcul_entropie_mot("aeree", mots_a_tester, nb_mots_a_tester, liste_patterns);
/*
    char mot_h_max[5];
    trouver_mot_h_max(mots_a_tester, nb_mots_a_tester, mots_a_tester, nb_mots_a_tester, mot_h_max, liste_patterns);
*/
    free(*liste_patterns);

    return 0;
}