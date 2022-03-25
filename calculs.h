#ifndef CALCULS_H
#define CALCULS_H

int lettre_est_dans(char* mot, char lettre, int nb_lettres);

int correspondance_ltr_jaune(char* mot_test, char lst_lettres[], int lst_etats[]);

int correspondance_ltr_verte(char* mot_test, char lst_lettres[], int lst_etats[], int lst_pos[]);

int* extraction_donnees(char* essai, int resultat[], char lst_lettres[], int lst_etats[], int lst_pos[], char* lst_lettres_ban);


#endif //CALCULS_H