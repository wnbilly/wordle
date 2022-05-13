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
//Liste de leurs etats : -1 si on ne sait pas, 0 si pas dans le mot, 1 si dans le mot à la mauvaise place, 2 si dans le mot à la bonne place


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

//Retourne 1 si le mot est dans la liste,0 sinon
int mot_est_dans(char* liste[], int taille, char* mot)
{
    for (int i=0; i<taille; i++)
    {
        if (liste[i]==mot) return 0;
    }
    return 1;
}

//Retourne le nombre d'occurences d'une lettre dans un mot
int nb_occurences(char* mot, char lettre)
{
    int nb_lettres = strlen(mot);
    int nb_occur = 0;
    for (int i=0; i<nb_lettres; i++)
    {
        if (mot[i]==lettre) nb_occur++;
    }
    return nb_occur;
}

int nb_occurences_ban(char* essai, int* resultat, char lettre)
{
    int nb_lettres = sizeof(essai);
    int nb_occ_ban = 0;

    for (int i = 0; i<nb_lettres; i++)
    {
        if (essai[i]==lettre && resultat[i]>0 ) nb_occ_ban++;

    }
    return nb_occ_ban+1;
}


//Vérifie si les lettres jaunes contenues dans data correspondent au mot_test
int correspondance_ltr_jaune(char* mot_test, struct donnees* data)
{
    int nb_lettres = 5;

    int nb_jaunes = 0;
    int nb_corres = 0;
    int* trace = (int*) calloc(nb_lettres,sizeof(int)); //Pour se rappeler des lettres déjà détectées : 1 si déjà détectée, 0 sinon

    //On élimine les mots avec des lettres au mauvais endroit (ex : "porte" n'est pas compatible avec "o" en 
    //lettre jaune en 2e position car sinon le "o" serait vert)
    for (int j=0; j<nb_lettres; j++)
    {
        if (data->lst_etats[j]==1 && mot_test[data->lst_pos[j]]==data->lst_lettres[j]) 
        {
            free(trace);
            return 0;
        }
        if (data->lst_etats[j]==2) trace[data->lst_pos[j]]=1; //On marque les lettres vertes comme déjà traitées
    }

    for (int i=0; i<nb_lettres; i++) //indice data
    {
        if (data->lst_etats[i]==1) //Si c'est une lettre jaune
        {
            nb_jaunes++; //
            for (int k=0; k<nb_lettres; k++) //indice mot_test
            {
                if (mot_test[k]==data->lst_lettres[i] && trace[k]==0) //On cherche les correspondances en nombre de jaunes
                {
                    //printf("%c = %c | trace[%d] = %d\n",mot_test[k], lst_lettres[i], k, trace[k]);
                    trace[k] = 1;
                    nb_corres++; //Une correspondance est détectée
                    break;
                }
            }
        }
    }

    free(trace);
    
    if (nb_corres == nb_jaunes) 
    {
        //printf("%d corres sur %d jaunes --> ", nb_corres, nb_jaunes);
        //printf("%s comp avec data :\n", mot_test);
        //affichage_donnees(data);
        return 1;
    }
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
    //printf("%d lettres bannies\n", nb_lettres_ban);
    for (int i=0; i<nb_lettres_ban; i++)
    {
        if (lettre_est_dans(mot_test, data->lst_lettres_ban[i])==1 && nb_occurences(mot_test, data->lst_lettres_ban[i])>=data->occ_ban[i]) 
        {
            //printf("%d occurences de %c dans le mot %s\n",data->occ_ban[i], data->lst_lettres_ban[i], mot_test);
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
            if(lettre_est_dans(data->lst_lettres_ban, essai[i]) == 0) //On vérifie si la lettre est déjà dans les lettres vertes/jaunes
            { //On bannit les lettres en gris
                data->lst_lettres_ban[j_ban]=essai[i];
                data->occ_ban[j_ban]=nb_occurences_ban(essai ,resultat, essai[i]);
                j_ban++;
            }
        }
    }
}


//Vérifie la compatibilité entre un mot et une struct donnees
int verif_compatibilite(char* mot_test, struct donnees* data)
{

    if (correspondance_ltr_jaune(mot_test, data)==1 && correspondance_ltr_verte(mot_test, data)==1 && test_ltr_ban(mot_test, data)==1) return 1;
    else return 0;
}


//Vérifie la compatibilité entre un mot et un ensemble de struct donnees
int verif_compatibilite_complete(char* mot_test, struct donnees* all_data[], int nb_essais)
{
    for( int i=0; i<nb_essais; i++)
    {
        if (verif_compatibilite(mot_test, all_data[i])==0) return 0;
    }
    return 1;
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
    printf("LETTRES BAN\n");
    
    for (int i=0; i<data->nb_lettres; i++)
    {
        printf("%d ",data->occ_ban[i]);
    }
    printf("OCCURENCES POUR  BAN\n");
    printf("\n");
}

struct donnees* init_data()
{
    struct donnees* data = malloc(sizeof(struct donnees));
    
    data->nb_lettres = 5;
    data->lst_lettres = malloc(data->nb_lettres*sizeof(char));
    data->lst_etats = calloc(data->nb_lettres, sizeof(int));
    data->lst_pos = malloc(data->nb_lettres*sizeof(int));
    data->lst_etats[0] = -1;
    data->lst_etats[1] = -1;
    data->lst_etats[2] = -1;
    data->lst_etats[3] = -1;
    data->lst_etats[4] = -1;
    data->lst_pos[0] = -1;
    data->lst_pos[1] = -1;
    data->lst_pos[2] = -1;
    data->lst_pos[3] = -1;
    data->lst_pos[4] = -1;
    for (int i = 0;i<26; i++)
    {
        //data->lst_lettres_ban[i]=NULL;
        data->occ_ban[i] = 0;
    }
    return data;
}

void free_data(struct donnees* data)
{
    free(data->lst_lettres);
    free(data->lst_etats);
    free(data->lst_pos);
    free(data);
}

void init_data_array(struct donnees* all_data[], int max_essais)
{
    for(int i=0; i<max_essais; i++)
    {
        all_data[i]=init_data();
    }
}

//Copy les size_src éléments de array_src dans array_dest, le reste de array_dest est inchangé
void copy_array(char* array_dest[], int size_dest, char* array_src[], int size_src)
{
    if (size_src>size_dest)
    {
        printf("Erreur size_src < size_dest\n");
        exit(0);
    }

    if (sizeof(array_dest[0])!=sizeof(array_src[0]))
    {
        printf("Erreur : array_dest et array_src n'ont pas les mêmes tailles de mots\n");
        exit(0);
    }

    for(int i=0; i<size_src; i++)
    {
        /*
        printf("i : %d\n", i);
        printf("array_dest[i] = %s\n",array_dest[i]);
        printf("array_src[i] = %s\n",array_src[i]);
        */
        array_dest[i] = array_src[i];
    }
}

int mainca(int argc, char* argv[])
{
    //struct donnees* data = init_data();

    char* essai = "aeree";
    int resultat[5] = {0,2,0,1,0};
    int max_essais = 6;

    affichage_resultat(essai, resultat, 5);

    struct donnees* all_data[6];
    init_data_array(all_data, max_essais);

    extraction_donnees(essai, resultat, all_data[2]);

    affichage_donnees(all_data[2]);

    correspondance_ltr_jaune("teint", all_data[2]);

    /*
    printf("corres_verte : %d\n",correspondance_ltr_verte(mot_test, data));
    printf("corres_jaune : %d\n",correspondance_ltr_jaune(mot_test, data));
    printf("corres_ban: %d\n",test_ltr_ban(mot_test, data));
    */

    char* array_src[2];
    array_src[0]="aaaaa";
    array_src[1]="bbbbb";

    char* array_dest[4];
    array_dest[0]="aloha";
    array_dest[1]="salut";
    array_dest[2]="hello";
    array_dest[3]="ensta";

    printf("SRC\n");
    affichage_tableau_mots(array_src, 2);
    printf("DEST\n");
    affichage_tableau_mots(array_dest, 4);
    printf("/COPY\n");
    copy_array(array_dest, 4, array_src, 2);
    array_src[1]="error";
    printf("SRC\n");
    affichage_tableau_mots(array_src, 2);
    printf("DEST\n");
    affichage_tableau_mots(array_dest, 4);

    return 0;
}
