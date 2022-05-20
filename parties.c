#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"
#include "entropie.h"

//.c qui regroupe les déroulements de parties pour le joueur et les différents bots

//Lance une partie jouée par un humain, retourne le nombre de coups pour gagner (-1 si perdu)
int partie_joueur(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots)
{
    //VARIABLES A MANIPULER
    char essai[nb_lettres]; //Variable du futur mot tenté par le joueur

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    //TOURS
    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        //Le joueur entre sa proposition
        printf("\nEssai %d/%d. ", nb_essais+1, max_essais);
        acquisition_clavier(essai, nb_lettres);
        
        //On vérifie si le mot est dans la liste
        while (test_existence_mot(mots, essai, nb_mots)==0)
        {
            printf("Ce mot n'est pas dans la liste !\n");
            acquisition_clavier(essai, nb_lettres);
        }

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

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
        free(resultat);
        return -1; //Le joueur a perdu
    }

    return nb_essais;
}

//Lance une partie jouée par le bot qui peut jouer des mots non probables, retourne le nombre de coups pour gagner (-1 si perdu)
int partie_bot1(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier)
{
    //VARIABLES A MANIPULER
    char essai[nb_lettres];

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* all_data[max_essais];
    init_data_array(all_data, max_essais);

    essai[0] = 'a';
    essai[1] = 'e';
    essai[2] = 'r';
    essai[3] = 'e';
    essai[4] = 'e';


    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;

    char* mots_joues[max_essais];
    mots_joues[0]=essai;

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        if (nb_essais!=0)
        {
            nb_mots_prob_buffer = liste_mots_prob(mots_probables_buffer, mots_probables, nb_mots_prob, all_data, mots_joues, nb_essais);

            printf("==================PROB==============%d mots\n", nb_mots_prob);
            affichage_tableau_mots(mots_probables, nb_mots_prob);
            printf("nb_mots_prob_buffer : %d\n", nb_mots_prob_buffer);
            //printf("nb_mots_prob%d : %d\n",nb_essais, nb_mots_prob);
            printf("==================BUFFER==============%d mots\n", nb_mots_prob_buffer);
            affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer);
            

            //*mots_probables = *mots_probables_buffer;
            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;

            printf("==================PROB==============%d mots\n", nb_mots_prob);
            //affichage_tableau_mots(mots_probables, nb_mots_prob);
            printf("==================BUFFER==============%d mots\n", nb_mots_prob_buffer);
            affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer+1);

            trouver_mot_h_max(mots, nb_mots, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);
        }

        printf("essai :%s\n",essai);

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, all_data[nb_essais]);

        affichage_donnees(all_data[nb_essais]);


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
            free(resultat);;
            return -1; //Le joueur a perdu
        }

    free(resultat);
    return nb_essais+1;    
}

//Lance une partie avec le bot qui ne joue que des mots probables
int partie_bot2(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier)
{
    //VARIABLES A MANIPULER
    char essai[nb_lettres];

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* data = init_data();

    essai[0] = 't';
    essai[1] = 'a';
    essai[2] = 'r';
    essai[3] = 'i';
    essai[4] = 'e';


    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;

    //affichage_tableau_mots(mots, nb_mots);
    printf("==========\n");

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        if (nb_essais!=0)
        {
            nb_mots_prob_buffer = liste_mots_prob1(mots_probables_buffer, mots_probables, nb_mots_prob+1, data);
/*
            if (nb_essais>1)
            {
                printf("==================PROB-1==============%d mots\n", nb_mots_prob);
                affichage_tableau_mots(mots_probables, nb_mots_prob);
            }
            */
            //printf("==================PROB==============%d mots\n", nb_mots_prob_buffer);
            //affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer);

            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;

            trouver_mot_h_max(mots_probables_buffer, nb_mots_prob_buffer, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);
        }

        printf("essai :%s\n",essai);

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, data);

        //affichage_donnees(data);

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
            free(resultat);
            return -1; //Le joueur a perdu
        }

    free(resultat);
    return nb_essais+1;    
}

//Lance une partie avec le bot qui joue aléatoirement parmi les mots probables (les mots probables sont mis à jour à chaque essai)
int partie_bot3(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier)
{
    //VARIABLES A MANIPULER
    
    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* data = init_data();
/*
    essai[0] = 'a';
    essai[1] = 'e';
    essai[2] = 'r';
    essai[3] = 'e';
    essai[4] = 'e';
*/
    char* essai;

    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;
    nb_mots_prob_buffer = nb_mots_prob;
    //affichage_tableau_mots(mots, nb_mots);
    printf("==========\n");

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        essai = choix_mot(mots_probables, nb_mots_prob+1); //choix_mot(mots, nb_mots); pour bot random dans le dico

        if (nb_essais>=0)
        {
            nb_mots_prob_buffer = liste_mots_prob1(mots_probables_buffer, mots_probables, nb_mots_prob, data);
/*
            if (nb_essais>1)
            {
                printf("==================PROB-1==============%d mots\n", nb_mots_prob);
                affichage_tableau_mots(mots_probables, nb_mots_prob);
            }
            */
            //printf("==================PROB==============%d mots\n", nb_mots_prob_buffer);
            //affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer);

            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;
        }

        printf("essai :%s\n",essai);

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, data);

        //affichage_donnees(data);

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
            return -1; //Le joueur a perdu
        }

    return nb_essais+1;    
}

//Le bot joue TRAIN puis COULE puis le bot2 prend le relais
int partie_bot4(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier)
{
    //VARIABLES A MANIPULER
    char essai[nb_lettres];

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* data = init_data();

    essai[0] = 't';
    essai[1] = 'r';
    essai[2] = 'a';
    essai[3] = 'i';
    essai[4] = 'n';


    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;

    //affichage_tableau_mots(mots, nb_mots);
    printf("==========\n");

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        if (nb_essais!=0)
        {
            nb_mots_prob_buffer = liste_mots_prob1(mots_probables_buffer, mots_probables, nb_mots_prob+1, data);
/*
            if (nb_essais>1)
            {
                printf("==================PROB-1==============%d mots\n", nb_mots_prob);
                affichage_tableau_mots(mots_probables, nb_mots_prob);
            }
            */
            //printf("==================PROB==============%d mots\n", nb_mots_prob_buffer);
            //affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer);

            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;

            if (nb_essais==1)
            {
                essai[0] = 'c';
                essai[1] = 'o';
                essai[2] = 'u';
                essai[3] = 'l';
                essai[4] = 'e';
            }
            else trouver_mot_h_max(mots_probables_buffer, nb_mots_prob_buffer, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);
        }

        printf("essai :%s\n",essai);

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, data);

        //affichage_donnees(data);

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
            free(resultat);
            return -1; //Le joueur a perdu
        }

    free(resultat);
    return nb_essais+1;    
}