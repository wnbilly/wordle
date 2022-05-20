#ifndef TESTS_LETTRES_H
#define TESTS_LETTRES_H

void printf_vert();

void printf_fond_vert();

void printf_jaune();

void printf_rouge();

void printf_fond_rouge();

void printf_gris();

void printf_blanc();

void printf_standard();

int test_victoire(int resultat[], int nb_lettres);

void remplissage_resultat(char* mot_cible, char* essai, int resultat[], int nb_lettres);

void reset_resultat(int resultat[], int nb_lettres);

void affichage_regles();

int choix_mode();

void affichage_gagne(int nb_essais);

void affichage_perdu(char* mot_cible);

void affichage_debug(int tableau[], int taille);

void affichage_resultat(char* essai, int resultat[], int nb_lettres);

void test_place(char* mot, char* essai, int resultat[], int nb_lettres);

void test_lettre(char* mot, char* essai, int resultat[], int nb_lettres);

int test_existence_mot(char *mots[], char* essai, int nb_mots);

void acquisition_clavier(char* essai, int nb_lettres);


#endif //TESTS_LETTRES_H