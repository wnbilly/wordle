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

int correspondance_ltr_jaune(char* mot_test, struct donnees *data)
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
        if (lettre_est_dans(mot_test, data->lst_lettres_ban[i])==1 && nb_occurences(mot_test, data->occ_ban[i])) 
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
    int indice_ban = 0;

    for (int i=0; i<nb_lettres; i++) //i indice resultat
    {
        for (int k=0; k<nb_lettres, k++) //k indice dans les donnees
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
                occ_ban[i]=nb_occurences(essai, essai[i])
            }
        }
    }
}

void affichage_donnees(struct donnees* data)
{
    printf("LETTRES DANS MOT\n");
    for (int i=0, i<data->nb_lettres, i++)
    {
        printf("%s ",data->lst_lettres[i]);
    }
    printf("ETATS LETTRES\n");
    for (int i=0, i<data->nb_lettres, i++)
    {
        printf("%d ",data->lst_etats[i]);
    }
    printf("POSITIONS\n");
    for (int i=0, i<data->nb_lettres, i++)
    {
        printf("%s ",data->lst_pos[i]);
    }
    printf("LETTRES BAN\n");
    for (int i=0, i<data->nb_lettres, i++)
    {
        printf("%s ",data->lst_lettres_ban[i]);
    }
    printf("\n");
    printf("OCCURENCES POUR  BAN\n");
    for (int i=0, i<data->nb_lettres, i++)
    {
        printf("%s ",data->occ_ban[i]);
    }
    printf("\n");

}

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


int main(int argc, char* argv[])
{
    char* essai = "porte";

    int nb_lettres = 5;
    
    //FAIRE STRUCT DONNEES

    struct donnees* data;

    int nb_infos_ltr = extraction_donnees(essai, resultat, data); //Nbr d'infos données par le résultat et le mot-essai
    
    char* nom_fichier = "liste_complete_triee.txt";

    int taille_test = 6000;
    char* mots[taille_test];
    char* mots_probables[taille_test];

    int nb_mots = extraction_mots(mots, nom_fichier, nb_lettres);

    int nb_mots_probables = liste_mots_prob(mots_probables, mots, nb_mots, lst_lettres, lst_etats, lst_pos, lst_lettres_ban);

    affichage_tableau_mots(mots_probables,nb_mots_probables);

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