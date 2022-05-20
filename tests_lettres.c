#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests_lettres.h"

//Tests des lettres entre essais et mot à deviner

//Code pour résultat :

// 0 pour rien
// 1 pour bonne lettre à mauvaise place
// 2 pour bonne lettre à la bonne place

void printf_vert() //Passe les caractères de console en vert gras
{
    printf("\033[1;32m");
}

void printf_fond_vert() //Passe le fond de console en vert gras
{
    printf("\033[7;32m");
}

void printf_jaune() //Passe les caractères de console en jaune gras
{
    printf("\033[1;33m");
}

void printf_rouge() //Passe les caractères de console en rouge gras
{
    printf("\033[1;31m");
}

void printf_fond_rouge() //Passe le fond de console en rouge gras
{
    printf("\033[7;31m");
}

void printf_blanc() //Passe les caractères de console en blanc gras
{
    printf("\033[1;37m");
}

void printf_standard() //Passe les caractères de console en format standard
{
    printf("\033[0;37m");
}

void affichage_debug(int tableau[], int taille)
{
    for (int j=0; j<taille; j++)
    {
        printf(" %d ",tableau[j]);
    }
    printf("\n");
}

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

void affichage_regles()
{
    printf_blanc();
    printf("-> Règles de Wordle :\n");
    printf_standard();
    printf("    L'utilisateur dispose de 6 essais pour deviner un mot de 5 lettres.\n");
    printf("    A chaque essai, le jeu indique les lettres contenues dans le mot joué et dans le mot à deviner.\n");
    
    printf("    La couleur");
    printf_vert();
    printf(" verte ");
    printf_standard();
    printf("correspond aux lettres contenues dans les deux mots et à la bonne place.\n");

    printf("    La couleur");
    printf_jaune();
    printf(" jaune ");
    printf_standard();
    printf("indique les lettres communes aux deux mots mais à la mauvaise place.");
    printf("    La couleur blanche indique les lettres qui ne sont pas dans le mot à deviner.\n\n");

    printf("Exemple si le joueur joue renes et doit deviner nerfs :");
    int res_regles[] = {1,2,1,0,2};
    affichage_resultat("renes", res_regles, 5);
}

int choix_mode()
{
    int mode = 0;
    printf("\n-------------------\n\n");
    printf_blanc();
    printf("-> Modes de jeu disponibles :\n");  
    printf_standard();
    printf("    0: jouer soi-même\n");  
    printf("    1: bot max E[h]\n");
    printf("    2: bot entropique Otanpi qui ne joue que des coups potentiellement gagnants\n");
    printf("    3: bot naïf à mémoire\n");  
    printf("    4: bot Train-Coule\n\n");
    printf("Veuillez entrer votre choix : ");
    scanf("%d", &mode);
    return mode;
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

void affichage_resultat(char* essai, int resultat[], int nb_lettres)
{
    for (int i = 0; i<nb_lettres; i++)
    {
        if (resultat[i]==0)
        {
            printf_standard();
            printf("%c", essai[i]);
        } 
        else if (resultat[i]==1)
        {
            printf_jaune();
            printf("%c", essai[i]);
        } 
        else 
        {
            printf_vert();
            printf("%c", essai[i]);
        }
    }
    printf_standard();
    printf("\n");
}

//Remplis les 2 (lettres verts) de resultat
void test_place(char* mot, char* essai, int resultat[], int nb_lettres)
{
    for (int i = 0; i<nb_lettres; i++)
    {
        if (essai[i]==mot[i])
            resultat[i]=2;
    }
}

//Remplis les 1 (lettres jaunes) de resultat
void test_lettre(char* mot, char* essai, int resultat[], int nb_lettres)
{
    int* trace = (int*) calloc(nb_lettres,sizeof(int)); //Pour se rappeler des lettres déjà détectées : 1 si déjà détectée, 0 sinon

    for (int j = 0; j<nb_lettres; j++)
    {
        if (resultat[j]==2) trace[j]=1;

    }

    for (int i=0; i<nb_lettres; i++) //i indice de l'essai
    {
        for (int k=0; k<nb_lettres; k++) //indice du mot à deviner
        {
            //if (i
            if(i!=k && essai[i]==mot[k] && trace[k]==0 && resultat[i]==0)
            {
                resultat[i]=1;
                trace[k]=1;
            }
        }
    }
    free(trace);
}

//Vérifie si le mot joué par le joueur est dans le dictionnaire du jeus
int test_existence_mot(char *mots[], char* essai, int nb_mots)
{
    for (int i=0; i<nb_mots; i++)   //AMELIORABLE SI LA LISTE EST TRIEE PAR ORDRE ALPHABETIQUE
        {
            if (strcmp(mots[i],essai)==0)
            {
                return 1;
            }
        }
    return 0;
}

//Permet à l'utilisateur d'entrer son essai via le clavier et vérifie si le mot appartient au dictionnaire de jeu
void acquisition_clavier(char* essai, int nb_lettres)
{
    printf("Veuillez entrer vos %d lettres\n\n -> ", nb_lettres);
    printf_blanc();
    scanf("%s",essai);
    if (strlen(essai)!=nb_lettres)
    {
        printf("Erreur : MAUVAIS NOMBRE DE LETTRES\n\n");
        acquisition_clavier(essai, nb_lettres);
    }
    printf_standard();
    printf("\n");
}

/*
int main(int argc, char* argv[])
{
    char* mot="PORTE";
    int nb_lettres=5;
    char essai[nb_lettres+1]; //+1 pour détecter si l'utilisateur met trop de lettres
    //char* essai;
    acquisition_clavier(essai, nb_lettres);
    
    int* resultat=(int*) calloc(nb_lettres,sizeof(int));

    test_place(mot,essai,resultat,nb_lettres);
    test_lettre(mot,essai,resultat, nb_lettres);

    //affichage_debug(resultat,nb_lettres);

    affichage_resultat(essai, resultat, nb_lettres);
    return 0;
}
*/