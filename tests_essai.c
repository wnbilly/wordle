#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Code pour résultat :

// 0 pour rien
// 1 pour bonne lettre à mauvaise place
// 2 pour bonne lettre à la bonne place

void test_place(char* mot, char* essai, int resultat[nb_lettres], int nb_lettres)
{
    for (int i = 0; i<nb_lettres; i++)
    {
        if (strcmp(essai[i],mot[i])==0)
            resultat[i]=2;
    }
}



int main(int argc, char* argv[])
{
    char* mot="PORTE";
    char* essai="PESTO";
    int nb_lettres=5;
    int resultat[nb_lettres]={0,0,0,0,0};

}