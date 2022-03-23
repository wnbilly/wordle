#ifndef ACQUISITION_H
#define ACQUISITION_H

void affichage_tableau_mots(char* tableau[], int taille);

extern int extraction_mots(char* mots[], char* nom_fichier, int nb_lettres);

extern char* choix_mot(char* mots[], int nb_mots);

#endif //ACQUISITION_H