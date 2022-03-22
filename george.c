#include <stdio.h>
#include <string.h>

int main()
{
    char* nom_ficher = 'dico.txt';
    char** mots=extraction_mots(nom_fichier,5);


}


char** extraction_mots(char* nom_fichier, int nb_lettres)
{
    FILE* fichier = fopen(nom_fichier);
    if (fichier == NULL ) 
    {
        printf("Erreur fichier\n");
        exit(0);
    }

    char* buffer;
    unsigned int = 0;
    while(!feof(f))
    {
        fscanf("%s",buffer);
        if (strlen(buffer) == nb_lettres)
        {
            extraction_mots[i]=buffer;
            printf("%s\n",buffer);
            i++;
        }
    } 


}