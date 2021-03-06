#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "acquisition.h"


//Acquisition des mots de nb_lettres dans un dictionnaire + Choix aléatoire d'un mot
// 4 <= nb_lettres <= 12

void affichage_tableau_mots(char* tableau[], int taille) //Pour test/debug
{
    for (int j=0; j<taille; j++)
    {
        //printf("%d\n",j);
        printf("%s\n",tableau[j]);
    }
}

int extraction_mots(char* mots[], char* nom_fichier, int nb_lettres) //FONCTION A MODIFIER SI PLUSIEURS LISTES PAR NB DE LETTRES
{

    int taille_additionnelle = strlen(nom_fichier);
    char chemin[12+taille_additionnelle];

    strcpy(chemin, "listes_mots/");
    strcat(chemin, nom_fichier);

    //printf("Chemin : %s\n", chemin);

    FILE* fichier = fopen(chemin,"r");
    if (fichier == NULL) 
    {
        printf("Erreur fichier\n");
        exit(0);
    }

    int i = 0;
    while(!feof(fichier))
    {
        char* buffer = malloc(30); //30 car le plus long mot fr fait 27 lettres + marge
    
        fscanf(fichier, "%s",buffer);
        if (strlen(buffer) == nb_lettres)
        {
            //printf("+%s\n",buffer);
            mots[i] = buffer;
            i++;
        }
    } 
    fclose(fichier);
    //printf("Fin d'acquisition des mots : %d mots à %d lettres\n",i,nb_lettres);
    return i-1;
}

char* choix_mot(char* mots[], int nb_mots)
{
    if (nb_mots==0) return "train"; //Pour éviter erreur aléatoire de %0
    int random = rand()%nb_mots;
    //printf("%de mot choisi : %s\n",random,mots[random]);
    return mots[random];
}