#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"

//Code permettant de calculer les probabilités etc

//Liste des lettres dans la réponse => char lst_ltr[nb_lettres]
//Liste de leurs etats : 0 si pas dans le mot, 1 si dans le mot à la mauvaise place, 2 si dans le mot à la bonne place

int lettre_est_dans(char* mot, char lettre) //Renvoie 1 si la lettre est dans le mot, 0 sinon
{
    int nb_lettres = strlen(mot);
    for (int i=0; i<nb_lettres; i++)
    {
        if (mot[i]==lettre) return 1;
    }
    return 0;
}

int nb_occurences(char* mot, char lettre) //Renvoie le nombre d'occurences de la lettre dans le mot
{
    int nb_lettres = strlen(mot);
    int nb_occur = 0;
    for (int i=0; i<nb_lettres; i++)
    {
        if (mot[i]==lettre) nb_occur++;
    }
    return nb_occur;
}

int correspondance_ltr_jaune(char* mot_test, struct donnees* data)
{
    int nb_lettres = strlen(mot_test);

    int nb_jaunes = 0;
    int nb_corres = 0;
    int* trace = (int*) calloc(nb_lettres,sizeof(int)); //Pour se rappeler des lettres déjà détectées : 1 si déjà détectée, 0 sinon

    for (int i=0; i<nb_lettres; i++)
    {
        if (data->lst_etats[i]==1) //Si c'est une lettre jaune
        {
            nb_jaunes++; //
            for (int k=0; k<nb_lettres; k++)
            {
                if (mot_test[k]==data->lst_lettres[i] && trace[k]==0)
                {
                    //printf("%c = %c | trace[%d] = %d\n",mot_test[k], lst_lettres[i], k, trace[k]);
                    trace[k] = 1;
                    nb_corres++; //Une correspondance est détectée
                    break;
                }
            }
        }
    }

    printf("%d corres sur %d jaunes --> ", nb_corres, nb_jaunes);

    if (nb_corres == nb_jaunes) return 1;
    else return 0;
}

int correspondance_ltr_verte(char* mot_test, struct donnees* data)
{
    int nb_infos_ltr = strlen(data->lst_lettres);

    for (int i=0; i<nb_infos_ltr; i++)
    {
        if (data->lst_etats[i]==2) //Si c'est une lettre verte
        {
            if (mot_test[data->lst_pos[i]]!=data->lst_lettres[i]) return 0;
        }
    }
    return 1;
}

int test_ltr_ban(char* mot_test, struct donnees* data)
{
    int nb_lettres_ban = strlen(data->lst_lettres_ban);
    printf("%d lettres bannies\n", nb_lettres_ban);
    for (int i=0; i<nb_lettres_ban; i++)
    {
        if (lettre_est_dans(mot_test, data->lst_lettres_ban[i])==1 && nb_occurences(mot_test, data->lst_lettres_ban[i])>=data->occ_ban[i]) 
        {
            printf("%d occurences de %c dans le mot %s\n",data->occ_ban[i], data->lst_lettres_ban[i], mot_test);
            return 0; //Au moins une lettre bannie est dans le mot
        }
    }
    return 1; //Le mot ne contient pas de lettre bannie
}

//Remplit lst_lettres, lst_etats, lst_pos, lst_lettres_ban à partir d'essai et de resultat
void extraction_donnees(char* essai, int resultat[], struct donnees* data)
{
    int nb_lettres = strlen(essai);
    //int indice_ban = 0;

    for (int i=0; i<nb_lettres; i++) //i indice resultat
    {
        for (int k=0; k<nb_lettres; k++) //k indice dans les donnees
        {
            if (resultat[i]!=0) 
            {
                data->lst_lettres[k]=essai[i];
                data->lst_etats[k]=resultat[i];
                data->lst_pos[k]=i;

            } 
            else if(lettre_est_dans(data->lst_lettres, essai[i]) == 1) //On vérifie si la lettre est déjà dans les lettres vertes/jaunes
            { //On bannit les lettres en gris
                char temp_char[] = {essai[i]};
                strcat(data->lst_lettres_ban, temp_char);
                data->occ_ban[i]=nb_occurences(essai, essai[i]);
            }
        }
    }
}

void affichage_donnees(struct donnees* data)
{
    printf("LETTRES DANS MOT\n");
    for (int i=0; i<(data->nb_lettres); i++)
    {
        printf("%c ",data->lst_lettres[i]);
    }
    printf("ETATS LETTRES\n");
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%d ",data->lst_etats[i]);
    }
    printf("POSITIONS\n");
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%d ",data->lst_pos[i]);
    }
    printf("LETTRES BAN\n");
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%c ",data->lst_lettres_ban[i]);
    }
    printf("\n");
    printf("OCCURENCES POUR  BAN\n");
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%d ",data->occ_ban[i]);
    }
    printf("\n");
}

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


int calcul_entropie_mot(char* mot, int nb_mots, char* mots_a_tester[])
{
    int nb_lettres = 5;
    int** liste_patterns = creation_liste_patterns(nb_lettres);
    int h = 0;
    int p = 0;
    int j;
    for (j = 0; j<243; j++);
    {
        int pattern[] = liste_patterns[j];
        p = nombre_mots_prob(mot, pattern, mots_a_tester, nb_mots)/nb_mots;
        h = h+p*log(p);
    }
}

int trouver_mot_h_max(char* mots_a_tester[], struct donnees* data, char mot_h_max[])
{
    int nb_mots_a_tester = sizeof(mots_a_tester)/sizeof(mots_a_tester[0]);
    int h_max = 0;
    for (int k=0; k<nb_mots_a_tester; k++)
    {
        int h_temp = calcul_entropie_mot(mots_a_tester[k],nb_mots_a_tester, mots_a_tester);
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

    trouver_mot_h_max(mots, data, mot_h_max);

    print("mot_hç_max est %s", mot_h_max);

    int nb_mots_probables = liste_mots_prob(mots_probables, mots, nb_mots, data);

    //affichage_tableau_mots(mots_probables,nb_mots_probables);

    /* TEST CORRES  
    char lst_lettres[] = {'a', 'o', 'h'};
    int lst_etats[] = {1, 1, 1, 1, 1}; //Equivalent de resultat dans les autres codes
    int lst_pos[] = {4, 1, 0, 3, 1};
    char lst_lettres_ban[] = {'t', 'b'};
    
    char* mot_cible = "porte";
    char* mot_test = "aloha";

    printf("corres_verte : %d\n",correspondance_ltr_verte(mot_test, lst_lettres, lst_etats, lst_pos));
    printf("corres_jaune : %d\n",correspondance_ltr_jaune(mot_test, lst_lettres, lst_etats));
    printf("corres_ban: %d\n",test_ltr_ban(mot_test, lst_lettres_ban));
    */
    
    return 0;
}