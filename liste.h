#ifndef LISTE_H
#define LISTE_H


struct liste_mots {
    int nb_elems;
    struct cell* liste;
}

struct cell {
    char* data;
    struct cell* next
}














#endif //LISTE_H