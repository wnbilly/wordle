#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void affichage(char* tableau[], int taille) //Pour test/debug
{
    for (int j=0; j<taille; j++)
    {
        printf("%d\n",j);
        printf("%s\n",tableau[j]);
    }
}

int extraction_mots(char* mots[], char* nom_fichier, int nb_lettres)
{
    FILE* fichier = fopen(nom_fichier,"r");
    if (fichier == NULL) 
    {
        printf("Erreur fichier\n");
        exit(0);
    }

    char* buffer = malloc(30); //30 car le plus long mot fr fait 27 lettres + marge
    int i = 0;
    while(!feof(fichier))
    {
        fscanf(fichier, "%s",buffer);
        if (strlen(buffer) == nb_lettres)
        {
            printf("+%s\n",buffer);
            mots[i] = buffer;
            i++;
        }
    } 
    printf("Fin d'acquisition des mots : %d mots à %d lettres\n",i,nb_lettres);
    return i-1;
}

char* choix_mot(char* mots[], int nb_mots)
{
    srand(time(0));
    int random = rand()%nb_mots+1;
    printf("random = %d\n",random);
    return mots[random];
}

int main(int argc, char* argv[])
{
    char* nom_fichier = "liste_francais.txt";
    int taille_test=5000;
    char* mots[taille_test];

    int nb_mots = extraction_mots(mots, nom_fichier,5);
    printf("%d\n", nb_mots);
    affichage(mots, nb_mots);
    choix_mot(mots,nb_mots);

    return 0;
}
