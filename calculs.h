#ifndef CALCULS_H
#define CALCULS_H

struct donnees{
        int nb_lettres;
        char* lst_lettres;   //contient les lettres dans le mot
        int* lst_etats; //2 si vert, 1 si jaune, 0 pas d'infos
        int* lst_pos;   //contient la position associée à la lettre dans lst_lettres au même indice
        char lst_lettres_ban[26]; //contient les lettres bannies
        int occ_ban[26]; //nombre d'occurences à avoir pour écarter le mot (=mot non compatible)
    };

int lettre_est_dans(char* mot, char lettre);

int nb_occurences(char* mot, char lettre);

int correspondance_ltr_jaune(char* mot_test, struct donnees* data);

int correspondance_ltr_verte(char* mot_test, struct donnees* data);

int test_ltr_ban(char* mot_test, struct donnees* data);

void extraction_donnees(char* essai, int resultat[], struct donnees* data);


#endif //CALCULS_H