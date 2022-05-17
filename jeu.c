#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "acquisition.h"
#include "tests_lettres.h"
#include "calculs.h"
#include "entropie.h"


//Main du Wordle
/*Compile avec 
gcc -Wall -Werror -Wfatal-errors -o JEU jeu.c acquisition.c tests_lettres.c calculs.c entropie.c -lm && ./JEU
*/

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

void remplissage_resultat(char* mot_cible, char* essai, int resultat[], int nb_lettres)
{
    //On détecte les lettres à mettre en vert
    test_place(mot_cible,essai,resultat,nb_lettres);

    //on détecte les lettres à mettre en jaune
    test_lettre(mot_cible,essai,resultat, nb_lettres);
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
        printf("Essai %d/%d. ", nb_essais+1, max_essais);
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

//Lance une partie jouée par un humain, retourne le nombre de coups pour gagner (-1 si perdu)
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
            //affichage_tableau_mots(mots_probables, nb_mots_prob);
            printf("nb_mots_prob_buffer : %d\n", nb_mots_prob_buffer);
            //printf("nb_mots_prob%d : %d\n",nb_essais, nb_mots_prob);
            printf("==================BUFFER==============%d mots\n", nb_mots_prob_buffer);
            //affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer);
            trouver_mot_h_max(mots, nb_mots, mots_probables_buffer, nb_mots_prob_buffer, essai, liste_patterns);

            //*mots_probables = *mots_probables_buffer;
            copy_array(mots_probables, nb_mots_prob, mots_probables_buffer, nb_mots_prob_buffer);
            nb_mots_prob = nb_mots_prob_buffer;

            printf("==================PROB==============%d mots\n", nb_mots_prob);
            //affichage_tableau_mots(mots_probables, nb_mots_prob);
            printf("==================BUFFER==============%d mots\n", nb_mots_prob_buffer);
            affichage_tableau_mots(mots_probables_buffer, nb_mots_prob_buffer+1);
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
            return -1; //Le joueur a perdu
        }

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

    essai[0] = 'a';
    essai[1] = 'e';
    essai[2] = 'r';
    essai[3] = 'e';
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

        affichage_donnees(data);

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

    //affichage_tableau_mots(mots, nb_mots);
    printf("==========\n");

    copy_array(mots_probables, nb_mots, mots, nb_mots); //Avant un quelconque essai, tous les mots sont probables

    srand(time(0));
    int random;

    for (nb_essais=0; nb_essais<max_essais; nb_essais++)
    {
        printf("Essai %d/%d. ", nb_essais+1, max_essais);

        random = rand()%nb_mots;
        essai = mots[random];

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

        affichage_donnees(data);

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

int mainje(int argc, char* argv[])
{
    //AFFICHAGE REGLES ET BIENVENUE ETC

    //PARAMETRES DE LA PARTIE
    int max_essais;
    int nb_lettres;
    int mode;

    printf("Mode de jeu (0: jouer, 1: bot max E[h], 2: bot qui ne joue que des coups déduits, 3:random parmi probables) : ");
    
    scanf("%d", &mode);
/*
    printf("Nombre de lettres dans [4;12] :");
    
    scanf("%d", &nb_lettres);
*/
    printf("Nombre d'essais :");
    
    scanf("%d", &max_essais);

    nb_lettres = 5;
    //max_essais = 6;

    //ACQUISITION MOTS
    char* nom_fichier = "liste_complete_triee.txt";
    int taille_test = 5070;
    char* mots[taille_test];
    int nb_mots = extraction_mots(mots, nom_fichier, nb_lettres);

    //CHOIX ALEATOIRE DU MOT A DEVINER
    char* mot_cible = choix_mot(mots,nb_mots);


    
    if (mode == 0)
    {
        partie_joueur(max_essais, nb_lettres, mot_cible, mots, nb_mots);

    } else if (mode == 1) //le bot peut tenter des mots non compatibles
    {
        printf("mot_cible : %s\n", mot_cible);
        partie_bot1(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier);     
    } 
    else if (mode == 2) //le bot ne tente que des mots probables
    {
        printf("mot_cible : %s\n", mot_cible);
        partie_bot2(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier); 
    }
    else if (mode == 3) //le bot tente aléatoirement parmi les mots probables
    {
        printf("mot_cible : %s\n", mot_cible);
        partie_bot3(max_essais, nb_lettres, mot_cible, mots, nb_mots, nom_fichier); 
    }

    return 0;
}