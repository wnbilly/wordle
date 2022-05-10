#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"
#include "entropie.h"


//Main du Wordle
//Compile avec gcc -Wall -Werror -Wfatal-errors -o JEU jeu.c acquisition.c tests_lettres.c && ./JEU

//piege donne de mauvais résultats

//Essai+resultat = données = mots probables

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

void affichage_gagne(int nb_essais)
{
    printf_fond_vert();
    printf("VOUS AVEZ GAGNE EN %d ESSAIS !\n", nb_essais+1);
    printf_standard();
}

void affichage_perdu(char* mot_cible)
{
    printf_fond_rouge();
    printf("PERDU, Le mot était ");
    printf_blanc();
    printf(" %s\n", mot_cible);
    printf_standard();
}

int main(int argc, char* argv[])
{
    //AFFICHAGE REGLES ET BIENVENUE ETC

    //PARAMETRES DE LA PARTIE
    int max_essais;
    int nb_lettres;
    int mode;

    printf("Mode de jeu (0: jouer, 1: bot) : ");
    
    scanf("%d", &mode);
/*
    printf("Nombre de lettres dans [4;12] :");
    
    scanf("%d", &nb_lettres);

    printf("Nombre d'essais :");
    
    scanf("%d", &max_essais);
*/

    nb_lettres = 5;
    max_essais = 6;


    //ACQUISITION MOTS
    char* nom_fichier = "liste_complete_triee.txt";
    int taille_test = 5070;
    char* mots[taille_test];//=malloc(taille_test*sizeof(char*));
    int nb_mots = extraction_mots(mots, nom_fichier, nb_lettres);
    
    //VARIABLES A MANIPULER
    char essai[nb_lettres+1]; //+1 pour détecter si l'utilisateur met trop de lettres

    //affichage_tableau_mots(mots, nb_mots);

    //INITIALISATION PARTIE
    char* mot_cible = "porte";//choix_mot(mots,nb_mots);

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;
    
    if (mode == 0)
    {
        //TOURS
        for (nb_essais=0; nb_essais<max_essais; nb_essais++)
        {
            //Le joueur entre sa proposition
            printf("Essai %d/%d. ", nb_essais+1, max_essais);
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
                affichage_gagne(nb_essais);
                break;
            } else {    //PAS GAGNE
                reset_resultat(resultat, nb_lettres);
            }
        }

        if (nb_essais == max_essais)
        {
            affichage_perdu(mot_cible);
        }

    } else if (mode == 1)
    {
        int** liste_patterns; //Initialisation de liste_patterns
        init_matrix(&liste_patterns, 243, 5);
        creation_liste_patterns(5, liste_patterns);

        essai[0] = 'a';
        essai[1] = 'e';
        essai[2] = 'r';
        essai[3] = 'e';
        essai[4] = 'e';


        char* mots_probables[5070];
        char* mots_probables_buffer[5070];
        int nb_mots_prob = extraction_mots(mots_probables, nom_fichier, nb_lettres);

        nb_mots_prob = nb_mots_prob;

        struct donnees* data = init_data();

        //On détecte les lettres à mettre en vert
        test_place(mot_cible,essai,resultat,nb_lettres);

        //on détecte les lettres à mettre en jaune
        test_lettre(mot_cible,essai,resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, data);

        affichage_donnees(data);

        for (nb_essais=0; nb_essais<max_essais; nb_essais++)
        {
            printf("Essai %d/%d. ", nb_essais+1, max_essais);

            int nb_mots_prob_buffer = liste_mots_prob(mots_probables_buffer, mots_probables, nb_mots_prob, data);
            //affichage_tableau_mots(mots_probables, nb_mots_prob);
            printf("nb_mots_prob_buffer : %d\n", nb_mots_prob_buffer);
            //printf("nb_mots_prob%d : %d\n",nb_essais, nb_mots_prob);
            //affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer);
            trouver_mot_h_max(mots, nb_mots, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);

            printf("essai :%s\n",essai);
            test_place(mot_cible,essai,resultat,nb_lettres);

            //on détecte les lettres à mettre en jaune
            test_lettre(mot_cible,essai,resultat, nb_lettres);

            affichage_resultat(essai, resultat, nb_lettres);

            //TEST VICTOIRE
                if (test_victoire(resultat, nb_lettres)==1)
                {           //GAGNE
                    affichage_gagne(nb_essais);
                    break;
                } else {    //PAS GAGNE
                    reset_resultat(resultat, nb_lettres);
                }
            
            *mots_probables = *mots_probables_buffer;
        }
            if (nb_essais == max_essais)
            {
                affichage_perdu(mot_cible);
            }
    }

    

    return 0;
}