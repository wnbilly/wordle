#ifndef JEU_H
#define JEU_H

int test_victoire(int resultat[], int nb_lettres);

void remplissage_resultat(char* mot_cible, char* essai, int resultat[], int nb_lettres);

void reset_resultat(int resultat[], int nb_lettres);

void affichage_gagne(int nb_essais);

void affichage_perdu(char* mot_cible);

int partie_bot1(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier);

int partie_bot2(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier);

#endif //JEU_C