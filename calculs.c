#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "acquisition.h"
#include "tests_lettres.h"

//Code permettant de calculer les probabilités etc

//Liste des lettres dans la réponse => char lst_ltr[nb_lettres]
//Liste de leurs etats : 0 si pas dans le mot, 1 si dans le mot à la mauvaise place, 2 si dans le mot à la bonne place
int lettre_est_dans(char* mot, char lettre, int nb_lettres)
{
    for (int i=0; i<nb_lettres; i++)
    {
        if (mot[i]==lettre) return 1;
    }
    return 0;
}

int correspondance_ltr_verte(char* mot, int lst_etats[], char lst_lettres[])
{

}

int est_mot_probable(char* mot_cible, char* mot_rest, int resultat[], int nb_lettres)
{

}

void liste_mots_prob(char* mot_cible, int lst_etats[], char lst_lettres[], char *lst_mots[])
{

}

int main(int argc, char* argv[])
{
    int nb_lettres;

    char lst_ltr[nb_lettres];
    int lst_etats[nb_lettres]; //Equivalent de resultat



    return 0;
}