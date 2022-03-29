#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "acquisition.h"
#include "tests_lettres.h"

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

int correspondance_ltr_jaune(char* mot_test, char lst_lettres[], int lst_etats[])
{
    int nb_lettres = strlen(mot_test);
    int nb_infos_ltr = strlen(lst_lettres);

    int nb_jaunes = 0;
    int nb_corres = 0;
    int* trace = (int*) calloc(nb_lettres,sizeof(int)); //Pour se rappeler des lettres déjà détectées : 1 si déjà détectée, 0 sinon

    for (int i=0; i<nb_infos_ltr; i++)
    {
        if (lst_etats[i]==1) //Si c'est une lettre jaune
        {
            nb_jaunes++; //
            for (int k=0; k<nb_lettres; k++)
            {
                if (mot_test[k]==lst_lettres[i] && trace[k]==0)
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

int correspondance_ltr_verte(char* mot_test, char lst_lettres[], int lst_etats[], int lst_pos[])
{
    int nb_infos_ltr = strlen(lst_lettres);

    for (int i=0; i<nb_infos_ltr; i++)
    {
        if (lst_etats[i]==2) //Si c'est une lettre verte
        {
            if (mot_test[lst_pos[i]]!=lst_lettres[i]) return 0;
        }
    }
    return 1;
}

int test_ltr_ban(char* mot_test, char* lst_lettres_ban)
{
    int nb_lettres_ban = strlen(lst_lettres_ban);
    printf("%d lettres bannies\n", nb_lettres_ban);
    for (int i=0; i<nb_lettres_ban; i++)
    {
        if (lettre_est_dans(mot_test, lst_lettres_ban[i])==1) 
        {
            printf("%c est dans le mot %s\n", lst_lettres_ban[i], mot_test);
            return 0; //Au moins une lettre bannie est dans le mot
        }
    }
    return 1; //Le mot ne contient pas de lettre bannie
}

int* extraction_donnees(char* essai, int resultat[], char lst_lettres[], int lst_etats[], int lst_pos[], char* lst_lettres_ban)
{
    int nb_lettres = strlen(essai);
    int indice_donnees = strlen(lst_lettres);
    for (int i=0; i<nb_lettres; i++)
    {
        if (resultat[i]!=0) 
        {
            lst_lettres[indice_donnees]=essai[i];
            lst_etats[indice_donnees]=resultat[i];
            lst_pos[indice_donnees]=i;
        } else { //On bannit les lettres en gris
            char temp_char[] = {essai[i]};
            strcat(lst_lettres_ban, temp_char);
        }
    }
    printf
}


char** liste_mots_prob(char* mots_a_tester[], int nb_mots, char lst_lettres[], int lst_etats[], int lst_pos[])
{
    char* mots_prob[];
    int indice_mot_prob = 0;
    for (int i=0; i<nb_mots; i++)
    {
        char* mot_test = mots_a_tester[i];
        if (correspondance_ltr_jaune(char* mot_test, char lst_lettres[], int lst_etats[])==1 && correspondance_ltr_verte(char* mot_test, char lst_lettres[], int lst_etats[], int lst_pos[])==1 && test_ltr_ban(mot_test, lst_lettres_ban)==1)
        {
            mots_prob[indice_mot_prob] = mot_test;
            indice_mot_prob++;
        }
    }
    return mots_prob;
}


int main(int argc, char* argv[])
{
    
    int nb_lettres = 5;
    
    char lst_lettres[nb_infos_ltr];
    int lst_etats[nb_infos_ltr]; //Equivalent de resultat
    int lst_pos[nb_infos_ltr]; //Position des lettres vertes, vaut -1 si lettre jaune

    char* mots_probables[] = liste_mots_prob

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