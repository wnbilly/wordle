#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void affichage(char* tableau[], int* taille)
{
    for (int j=0; j<*taille; j++)
    {
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
            mots[i] = buffer;
            //printf("%s\n",buffer);
            //printf("mots à %d lettres : %d\n",nb_lettres,i);
            //affichage(mots, i);
            i++;
        }
    } 
    printf("%d\n",i);
    *nb_mots = i;
    printf("Fin d'acquisition des mots.\n");
}



int main(int argc, char* argv[])
{
    char* nom_fichier = "liste_francais.txt";
    int* nb_mots = 0;
    int taille_test=5000;
    char* mots[taille_test];
    //int n_test = 5000;
    //char** mots = malloc(n_test*sizeof(char*));
    

    //mots = (char**) malloc(n_test*sizeof(char*)); //5000 pour tester
    //for(int k=0; k<n_test; k++) mots[k] = (char*) malloc(5*sizeof(char));
    extraction_mots(mots, nom_fichier,5, nb_mots);
    printf("Acq ok\n");
    printf("%d\n", *nb_mots);
    affichage(mots, nb_mots);

    return 0;
}
