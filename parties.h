#ifndef PARTIES_H
#define PARTIES_H

int partie_joueur(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots);

int partie_bot1(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier);

int partie_bot2(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier);

int partie_bot3(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier);

int partie_bot4(int max_essais, int nb_lettres, char* mot_cible, char* mots[], int nb_mots, char* nom_fichier);

#endif //PARTIES_C