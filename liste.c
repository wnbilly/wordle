#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"
#include "liste.h"

//Codes nécessaires à l'utilisation de listes

struct liste_mots* ajout_tete(struct liste_mots* liste_accueil, char* nouv_mot)
{
    struct cell* tmp;
    tmp->data = nouv_mot;
    tmp->next = liste_accueil->liste;
    liste_accueil->liste = tmp;
    return liste_accueil;
}

void print_list(struct liste_mots*)
{

    while (tmp->next != NULL)
    {
        
    }
}

int main(int argc, char* argv[])
{
    struct cell* mot2 = {"arbre", NULL};
    struct cell* mot1 = {"porte", mot2};
    struct liste_mots* lst_mots = {2, mot1};


    return 0;
}