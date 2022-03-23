#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Tests des lettres entre essais et mot à deviner

//Code pour résultat :

// 0 pour rien
// 1 pour bonne lettre à mauvaise place
// 2 pour bonne lettre à la bonne place

void printf_vert()
{
    printf("\033[1;32m");
}

void printf_jaune()
{
    printf("\033[1;33m");
}

void printf_blanc()
{
    printf("\033[1;37m");
}

void printf_standard()
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

void affichage_resultat(char* essai, int resultat[], int nb_lettres)
{
    for (int i = 0; i<nb_lettres; i++)
    {
        if (resultat[i]==0)
        {
            printf_blanc();
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

void test_place(char* mot, char* essai, int resultat[], int nb_lettres)
{
    for (int i = 0; i<nb_lettres; i++)
    {
        if (essai[i]==mot[i])
            resultat[i]=2;
    }
}

void test_lettre(char* mot, char* essai, int resultat[], int nb_lettres)
{
    int* trace=(int*) calloc(nb_lettres,sizeof(int)); //Pour se rappeler des lettres déjà détectées : 1 si déjà détectée, 0 sinon
    for (int i=0; i<nb_lettres; i++)
    {
        for (int k=0; k<nb_lettres; k++)
        {
            if (essai[i]==mot[k] && trace[k]==0 && resultat[k]!=2)
            {
                resultat[i]=1;
            }
        }
    }
}

void acquisition_clavier(char* essai, int nb_lettres)
{
    printf_blanc();
    printf("Veuillez taper vos %d lettres\n -> ", nb_lettres);
    scanf("%s",essai);
    if (strlen(essai)!=nb_lettres)
    {
        printf("Erreur : MAUVAIS NOMBRE DE LETTRES\n\n");
        acquisition_clavier(essai, nb_lettres);
    }
    printf_standard();
}

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

}