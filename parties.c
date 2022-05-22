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
    printf("========== Bot2 : Bot Otanpi qui peut jouer des mots qu'on sait non-gagnants\n");
    //VARIABLES A MANIPULER
    char essai[nb_lettres];

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* all_data[max_essais];
    init_data_array(all_data, max_essais);

    essai[0] = 'a';//'t'; //Pour changer entre anglais et français
    essai[1] = 'e';//'a'; //Uniquemenet pour faire les stats
    essai[2] = 'r';//'r';
    essai[3] = 'e';//'i';
    essai[4] = 'e';

    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        if (nb_essais!=0)
        {
            nb_mots_prob_buffer = liste_mots_prob(mots_probables_buffer, mots_probables, nb_mots_prob, all_data, nb_essais);

            printf("nb_mots_prob_buffer : %d\n", nb_mots_prob_buffer);
            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;
            //Recherche d'information en début de partie
            if (nb_essais<max_essais-3) trouver_mot_h_max(mots, nb_mots, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);
            //On tente de gagner en fin de partie
            else trouver_mot_h_max(mots_probables_buffer, nb_mots_prob_buffer, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);
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
            //free_data_array(all_data, max_essais);
            free_matrix(liste_patterns, 243, 5);
            free(resultat);;
            return -1; //Le joueur a perdu
        }

    //free_data_array(all_data, max_essais);
    free_matrix(liste_patterns, 243, 5);
    free(resultat);
    return nb_essais+1;    
}

//Lance une partie avec le bot qui ne joue que des mots probables
int partie_bot2(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier)
{
    printf("========== Bot2 : Bot Otanpi qui ne joue que des mots potentiellement gagnants\n");
    //VARIABLES A MANIPULER
    char essai[nb_lettres];

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* data = init_data();

    essai[0] = 'a';//'t'; //Pour changer entre anglais et français
    essai[1] = 'e';//'a'; //Uniquemenet pour faire les stats
    essai[2] = 'r';//'r';
    essai[3] = 'e';//'i';
    essai[4] = 'e';

    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        if (nb_essais!=0)
        {
            nb_mots_prob_buffer = liste_mots_prob1(mots_probables_buffer, mots_probables, nb_mots_prob+1, data);

        printf("nb_mots_prob_buffer : %d\n", nb_mots_prob_buffer);

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
    printf("========== Bot3 : Bot naïf à mémoire\n");
    //VARIABLES A MANIPULER
    
    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* data = init_data();

    char* essai;

    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;
    nb_mots_prob_buffer = nb_mots_prob;

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        nb_mots_prob_buffer = liste_mots_prob1(mots_probables_buffer, mots_probables, nb_mots_prob, data);

        printf("nb_mots_prob_buffer : %d\n", nb_mots_prob_buffer);

        copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
        nb_mots_prob = nb_mots_prob_buffer;


        essai = choix_mot(mots_probables, nb_mots_prob);

        printf("essai :%s\n",essai);

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, data);

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
    printf("========== Bot4 : bot Train-Coule + Otanpi\n\n");

    //VARIABLES A MANIPULER
    char essai[nb_lettres];

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* data = init_data();

    essai[0] = 't';//'t';
    essai[1] = 'r';//'r';
    essai[2] = 'a';//'a';
    essai[3] = 'i';//'i';
    essai[4] = 'n';//'n';

    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        if (nb_essais!=0)
        {
            nb_mots_prob_buffer = liste_mots_prob1(mots_probables_buffer, mots_probables, nb_mots_prob+1, data);
            printf("nb_mots_prob_buffer : %d\n", nb_mots_prob_buffer);

            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;

            if (nb_essais==1)
            {
                essai[0] = 'c';//'c';
                essai[1] = 'o';//'o';
                essai[2] = 'u';//'u';
                essai[3] = 'l';//'l';
                essai[4] = 'e';//'e';
            }
            else trouver_mot_h_max(mots_probables_buffer, nb_mots_prob_buffer, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);
        }

        printf("essai :%s\n",essai);

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, data);

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

//Le bot joue TRAIN puis COULE puis le bot3 naïf à mémoire prend le relais
int partie_bot5(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier)
{
    printf("========== Bot5 : bot Train-Coule + bot naïf à mémoire\n\n");

    //VARIABLES A MANIPULER
    char essai[nb_lettres];

    int* resultat = (int*) calloc(nb_lettres,sizeof(int));
    int nb_essais = 0;

    int** liste_patterns; //Initialisation de liste_patterns
    init_matrix(&liste_patterns, 243, 5);
    creation_liste_patterns(5, liste_patterns);

    struct donnees* data = init_data();

    essai[0] = 't';//'t';
    essai[1] = 'r';//'r';
    essai[2] = 'a';//'a';
    essai[3] = 'i';//'i';
    essai[4] = 'n';//'n';

    char* mots_probables[nb_mots];
    char* mots_probables_buffer[nb_mots];
    int nb_mots_prob = nb_mots;
    int nb_mots_prob_buffer;

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        if (nb_essais!=0)
        {
            nb_mots_prob_buffer = liste_mots_prob1(mots_probables_buffer, mots_probables, nb_mots_prob+1, data);

            printf("nb_mots_prob = %d\n", nb_mots_prob_buffer);

            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;

            if (nb_essais==1)
            {
                essai[0] = 'c';//'c';
                essai[1] = 'o';//'o';
                essai[2] = 'u';//'u';
                essai[3] = 'l';//'l';
                essai[4] = 'e';//'e';
            }
            else strcpy(essai, choix_mot(mots_probables, nb_mots_prob));
        }

        printf("essai :%s\n",essai);

        remplissage_resultat(mot_cible, essai, resultat, nb_lettres);

        affichage_resultat(essai, resultat, nb_lettres);

        extraction_donnees(essai, resultat, data);

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