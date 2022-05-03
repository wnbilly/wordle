#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"

//Code permettant de réaliser les vérifications nécessaires au calcul des probabilités etc

//gcc -Wall -Werror -Wfatal-errors -o CALCULS calculs.c acquisition.c tests_lettres.c -lm && ./CALCULS

//Liste des lettres dans la réponse => char lst_ltr[nb_lettres]
//Liste de leurs etats : 0 si pas dans le mot, 1 si dans le mot à la mauvaise place, 2 si dans le mot à la bonne place

//Retourne 1 si la lettre est dans le mot, 0 sinon
int lettre_est_dans(char* mot, char lettre) //Renvoie 1 si la lettre est dans le mot, 0 sinon
{
    int nb_lettres = 5;
    for (int i=0; i<nb_lettres; i++)
    {
        if (mot[i]==lettre) return 1;
    }
    return 0;
}

//Retourne le nombre d'occurences d'une lettre dans un mot
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

//Vérifie si les lettres jaunes contenues dans data correspondent au mot_test
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

//Vérifie si les lettres vertes contenues dans data correspondent au mot_test
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

//Vérifie si le mot_test contient les lettres bannies contenues dans data
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
    int nb_lettres = 5;
    //int indice_ban = 0;
    int j = 0; //indice de remplissage de data
    int j_ban = 0; //indice de remplissage de ban
    for (int i=0; i<nb_lettres; i++) //i indice resultat
    {
         //k indice dans les donnees
        if (resultat[i]!=0) //Si resultat indique que la i_eme lettre de essai est jaune ou verte
        {
            data->lst_lettres[j]=essai[i];
            data->lst_etats[j]=resultat[i];
            data->lst_pos[j]=i;
            j++;
        } //Si la i_eme lettre est grise
        else 
        {
            if(lettre_est_dans(data->lst_lettres, essai[i]) == 1 && lettre_est_dans(data->lst_lettres_ban, essai[i]) == 0) //On vérifie si la lettre est déjà dans les lettres vertes/jaunes
            { //On bannit les lettres en gris
                data->lst_lettres_ban[j_ban]=essai[i];
                data->occ_ban[j_ban]=nb_occurences(essai, essai[i]);
                j_ban++;
            }
            else if(lettre_est_dans(data->lst_lettres, essai[i]) == 0 && lettre_est_dans(data->lst_lettres_ban, essai[i]) == 0)
            {
                char temp_char[] = {essai[i]};
                strcat(data->lst_lettres_ban, temp_char);
                data->occ_ban[j_ban]=1;
                j_ban++;
            }
        }
    }
}

//Affiche le contenu de data
void affichage_donnees(struct donnees* data)
{
    ;
    for (int i=0; i<(data->nb_lettres); i++)
    {
        printf("%c ",data->lst_lettres[i]);
    }
    printf("LETTRES DANS MOT\n");
    
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%d ",data->lst_etats[i]);
    }
    printf("ETATS LETTRES\n");
    
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%d ",data->lst_pos[i]);
    }
    printf("POSITIONS\n");
    
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%c ",data->lst_lettres_ban[i]);
    }
    printf("\n");
    printf("LETTRES BAN\n");
    
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%d ",data->occ_ban[i]);
    }
    printf("OCCURENCES POUR  BAN\n");
    printf("\n");
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


    //affichage_tableau_mots(mots_probables,nb_mots_probables);

    /*TEST CORRES

    char lst_lettres[] = {'a', 'o', 'h'};
    int lst_etats[] = {1, 1, 1, 1, 1}; //Equivalent de resultat dans les autres codes
    int lst_pos[] = {4, 1, 0, 3, 1};
    char lst_lettres_ban[] = {'t', 'b'};
    */

    //char* mot_cible = "porte";
    //char* mot_test = "aloha";

    char* essai = "aloha";
    int resultat[5] = {0,1,0,2,0};

    affichage_resultat(essai, resultat, 5);

    extraction_donnees(essai, resultat, data);

    affichage_donnees(data);

    /*
    printf("corres_verte : %d\n",correspondance_ltr_verte(mot_test, data));
    printf("corres_jaune : %d\n",correspondance_ltr_jaune(mot_test, data));
    printf("corres_ban: %d\n",test_ltr_ban(mot_test, data));
    */

    return 0;
}