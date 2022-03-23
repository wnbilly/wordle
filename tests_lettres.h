#ifndef TESTS_LETTRES_H
#define TESTS_LETTRES_H

void printf_vert();

void printf_jaune();

void printf_blanc();

void printf_standard();

void affichage_resultat(char* essai, int resultat[], int nb_lettres);

void test_place(char* mot, char* essai, int resultat[], int nb_lettres);

void test_lettre(char* mot, char* essai, int resultat[], int nb_lettres);

void acquisition_clavier(char* essai, int nb_lettres);


#endif //TESTS_LETTRES_H