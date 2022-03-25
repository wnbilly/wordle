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

int correspondance_ltr_jaune(char* mot_test, char lst_lettres[], int lst_etats[], int lst_pos[], int nb_infos_ltr)
{
    int nb_lettres = strlen(mot_test);

    for (int i=0; i<nb_infos_ltr; i++)
    {
        if (lst_etats[i]==1) //Si c'est une lettre jaune
        {
            for (int k=0; k<nb_lettres; k++)
            {
                if (mot_test[k]==lst_lettres[i])
            }
        }
    }
    return 1;
}

int correspondance_ltr_verte(char* mot_test, char lst_lettres[], int lst_etats[], int lst_pos[], int nb_infos_ltr)
{
    for (int i=0; i<nb_infos_ltr; i++)
    {
        if (lst_etats[i]==2) //Si c'est une lettre verte
        {
            if (mot_test[lst_pos[i]]!=lst_lettres[i]) return 0;
        }
    }
    return 1;
}

/*
int est_mot_probable(char* mot_cible, char* mot_test, int resultat[], int nb_lettres)
{

}

void liste_mots_prob(char* mot_cible, int lst_etats[], char lst_lettres[], char *lst_mots[])
{

}
*/

int main(int argc, char* argv[])
{
    
    //int nb_lettres = 5;
    int nb_infos_ltr = 3;  //Nombre d'infos 
    /*
    char lst_lettres[nb_infos_ltr];
    int lst_etats[nb_infos_ltr]; //Equivalent de resultat
    int lst_pos[nb_infos_ltr]; //Position des lettres vertes, vaut -1 si lettre jaune
    */      
    char lst_lettres[] = {'a', 'o'};
    int lst_etats[] = {2, 2}; //Equivalent de resultat
    int lst_pos[] = {4, 2};
    
    //char* mot_cible = "porte";
    char* mot_test = "aloha";

    printf("%d\n",correspondance_ltr_verte(mot_test, lst_lettres, lst_etats, lst_pos, nb_infos_ltr));


    return 0;
}