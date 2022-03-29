#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "acquisition.h"


//Acquisition des mots de nb_lettres dans un dictionnaire + Choix aléatoire d'un mot
// 4 <= nb_lettres <= 12

void affichage_tableau_mots(char* tableau[], int taille) //Pour test/debug
{
    for (int j=0; j<=taille; j++)
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
    printf("Fin d'acquisition des mots : %d mots à %d lettres\n",i,nb_lettres);
    return i-1;
}

char* choix_mot(char* mots[], int nb_mots)
{
    srand(time(0));
    int random = rand()%nb_mots;
    //printf("%de mot choisi : %s\n",random,mots[random]);
    return mots[random];
}
/*
int main(int argc, char* argv[])
{
    char* nom_fichier = "liste_francais.txt";
    int taille_test=5000;
    char* mots[taille_test];//=malloc(taille_test*sizeof(char*));

    int nb_mots = extraction_mots(mots, nom_fichier,5);
    
    affichage_tableau_mots(mots, nb_mots);
    choix_mot(mots,nb_mots);

    return 0;
}
*/