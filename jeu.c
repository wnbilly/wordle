#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "acquisition.h"
#include "tests_lettres.h"

//Main du Wordle
//Compile avec gcc -Wall -Werror -Wfatal-errors -o JEU jeu.c acquisition.c tests_lettres.c && ./JEU


/*
void initialisation_partie(char* mot_cible, )


void tour()
*/

int test_victoire(int resultat[], int nb_lettres)
{
    for (int i=0; i<nb_lettres; i++)
    {
        if (resultat[i]!=2)
        {
            return 0; //Pas gagné
        }
    }
    return 1; //Gagné
}

void reset_resultat(int resultat[], int nb_lettres)
{
    for (int i=0; i<nb_lettres; i++)
    {
        resultat[i]=0;
    }
}

int main(int argc, char* argv[])
{
    //AFFICHAGE REGLES ET BIENVENUE ETC

    //PARAMETRES DE LA PARTIE
    int max_essais = 6;
    int nb_lettres = 5;

    //ACQUISITION MOTS
    char* nom_fichier = "liste_complete_triee.txt";
    int taille_test = 5100;
    char* mots[taille_test];//=malloc(taille_test*sizeof(char*));
    int nb_mots = extraction_mots(mots, nom_fichier,5);
    
    //VARIABLES A MANIPULER
    char essai[nb_lettres+1]; //+1 pour détecter si l'utilisateur met trop de lettres

    //affichage_tableau_mots(mots, nb_mots);

    //INITIALISATION PARTIE
    char* mot_cible = choix_mot(mots,nb_mots);

    int* resultat=(int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;
    
    //TOURS
    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        //Le joueur entre sa proposition
        acquisition_clavier(essai, nb_lettres);

        //On vérifie si le mot est dans la liste
        while (test_existence_mot(mots, essai, nb_mots)==0)
        {
            printf("Ce mot n'est pas dans la liste !\n");
            acquisition_clavier(essai, nb_lettres);
        }

        //On détecte les lettres à mettre en vert
        test_place(mot_cible,essai,resultat,nb_lettres);
        //on détecte les lettres à mettre en jaune
        test_lettre(mot_cible,essai,resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        //TEST VICTOIRE
        if (test_victoire(resultat, nb_lettres)==1)
        {           //GAGNE
            printf_fond_vert();
            printf("VOUS AVEZ GAGNE EN %d ESSAIS !\n", nb_essais);
            printf_standard();
            break;
        } else {    //PAS GAGNE
            reset_resultat(resultat, nb_lettres);
        }
    }

    if (nb_essais == max_essais)
    {
        printf_fond_rouge();
        printf("PERDU, Le mot était ");
        printf_blanc();
        printf("%s\n", mot_cible);
        printf_standard();
    }
    

    return 0;
}