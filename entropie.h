#ifndef ENTROPIE_H
#define ENTROPIE_H

int liste_mots_prob1(char* mots_probables[], char* mots_a_tester[], int nb_mots_a_tester, struct donnees* data);

int liste_mots_prob(char* mots_probables[], char* mots_a_tester[], int nb_mots_a_tester, struct donnees* all_data[], int nb_essais);

int nombre_mots_prob(char* mots_a_tester[], int nb_mots_a_tester, struct donnees* data);

float calcul_entropie_mot(char* mot, char* mots_a_tester[], int nb_mots_a_tester, int** liste_patterns);

void init_matrix(int ***matrix, int n, int p);

float trouver_mot_h_max(char* dico[], int nb_mots_dico, char* mots_a_tester[], int nb_mots_a_tester, char mot_h_max[], int** liste_patterns);

void creation_liste_patterns(int nb_lettres, int* liste_patterns[]);

void affichage_liste_patterns(int **liste_patterns);


#endif //ENTROPIE_H