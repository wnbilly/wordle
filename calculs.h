#ifndef CALCULS_H
#define CALCULS_H

struct donnees{
        int nb_lettres;
        char lst_lettres[nb_lettres];   //contient les lettres dans le mot
        int lst_etats[nb_lettres]; //2 si vert, 1 si jaune, 0 pas d'infos
        int lst_pos[nb_lettres];    //contient la position associée à la lettre dans lst_lettres au même indice
        char lst_lettres_ban[26]; //contient les lettres bannies
        int occ_ban[26]; //nombre d'occurences à avoir pour écarter le mot (=mot non compatible)
    };

int lettre_est_dans(char* mot, char lettre, int nb_lettres);

int correspondance_ltr_jaune(char* mot_test, char lst_lettres[], int lst_etats[]);

int correspondance_ltr_verte(char* mot_test, char lst_lettres[], int lst_etats[], int lst_pos[]);

int* extraction_donnees(char* essai, int resultat[], char lst_lettres[], int lst_etats[], int lst_pos[], char* lst_lettres_ban);


#endif //CALCULS_H