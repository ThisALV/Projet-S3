#include <utils_sd.h>

#include <stdlib.h>


// Les appels a calloc() plutot qu'a malloc permettent d'avoir tous les
// elements initialises a 0, et donc d'eviter les comportements innattendus

int* creer_tab_int(int dim) {
    return (int*) calloc(dim, sizeof(int));
}

int** creer_mat_int(int dim) {
    // Chaque ligne est un pointeur vers un tableau dynamique d'entiers
    int** lignes = (int**) malloc(sizeof(int*) * dim);

    // On initialise chacune de ces lignes avec un tableau de taille dim
    for (int i = 0; i < dim; i++)
        lignes[i] = creer_tab_int(dim);

    return lignes;
}

char** creer_tab_char_star(int dim) {
    return (char**) malloc(sizeof(char*) * dim);
}

char*** creer_mat_char_star(int lignes, int colonnes) {
    // Chaque ligne est un pointeur vers un tableau dynamique de chaines de caracteres
    char*** hauteur = (char***) malloc(sizeof(char**) * lignes);

    // On initialise chacune de ces lignes avec un tableau de taille colonnes
    for (int i = 0; i < lignes; i++)
        hauteur[i] = creer_tab_char_star(colonnes);

    return hauteur;
}

void creer_t_tab_int_dyn(t_tab_int_dyn* tab, int dim) {
    tab->elems = creer_tab_int(dim);
    tab->taille = dim;
}

void creer_t_mat_int_dyn(t_mat_int_dyn* mat, int dim) {
    mat->elems = creer_mat_int(dim);
    mat->dim = dim;
}

void creer_t_tab_char_star_dyn(t_tab_char_star_dyn* tab, int dim) {
    tab->elems = creer_tab_char_star(dim);
    tab->taille = dim;
}

void creer_t_mat_char_star_dyn(t_mat_char_star_dyn* mat, int lignes, int colonnes) {
    mat->elems = creer_mat_char_star(lignes, colonnes);
    mat->lignes = lignes;
    mat->colonnes = colonnes;
}
