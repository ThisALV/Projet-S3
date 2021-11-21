#include <utils_sd.h>

#include <stdlib.h>

// Utilisee sur les dimensions des SDD pour verifier leur validite
#define VERIFIER_DIM(dim) if (dim < 1) return NULL
// Utilisee sur les pointeurs pour verifier que de la memoire a ete allouee
#define VERIFIER_MEM(ptr) if (ptr == NULL) return NULL


// Les appels a calloc() plutot qu'a malloc permettent d'avoir tous les
// elements initialises a 0, et donc d'eviter les comportements innattendus

int* creer_tab_int(int dim) {
    VERIFIER_DIM(dim);

    return (int*) calloc(dim, sizeof(int));
}

int** creer_mat_int(int dim) {
    VERIFIER_DIM(dim);

    // Chaque ligne est un pointeur vers un tableau dynamique d'entiers
    int** lignes = (int**) malloc(sizeof(int*) * dim);
    // Avant d'ecrire dans ce tableau, on verifie qu'il a bien ete alloue
    VERIFIER_MEM(lignes);

    // On initialise chacune de ces lignes avec un tableau de taille dim
    for (int i = 0; i < dim; i++) {
        int* ligne =  creer_tab_int(dim);
        // On verifie que chaque ligne soit allouee correctement
        VERIFIER_MEM(ligne);

        lignes[i] = ligne;
    }

    return lignes;
}

void detruire_mat_int(int** mat, int dim) {
    for (int i = 0; i < dim; i++) {
        // On desalloue chaque ligne
        free(mat[i]);
    }

    // Puis on desalloue le tableau des lignes
    free(mat);
}

char** creer_tab_char_star(int dim) {
    VERIFIER_DIM(dim);

    return (char**) malloc(sizeof(char*) * dim);
}

char*** creer_mat_char_star(int lignes, int colonnes) {
    // On verifie que les 2 dimensions de la matrice sont valides
    VERIFIER_DIM(lignes);
    VERIFIER_DIM(colonnes);

    // Chaque ligne est un pointeur vers un tableau dynamique de chaines de caracteres
    char*** hauteur = (char***) malloc(sizeof(char**) * lignes);

    // On initialise chacune de ces lignes avec un tableau de taille colonnes
    for (int i = 0; i < lignes; i++) {
        char** ligne = creer_tab_char_star(colonnes);
        // On verifie que chaque ligne soit allouee correctement
        VERIFIER_MEM(ligne);

        hauteur[i] = ligne;
    }

    return hauteur;
}

void detruire_mat_char_star(char*** mat, int lignes) {
    for (int i = 0; i < lignes; i++) {
        // On desalloue chaque ligne
        free(mat[i]);
    }

    // Puis on desalloue le tableau des lignes
    free(mat);
}

bool creer_t_tab_int_dyn(t_tab_int_dyn* tab, int dim) {
    tab->elems = creer_tab_int(dim);
    tab->taille = dim;

    // Si la SDD allouee retournee n'est pas NULL, alors on sait que l'operation
    // s'est bien deroulee
    return tab->elems != NULL;
}

void detruite_t_tab_int_dyn(t_tab_int_dyn* tab) {
    // On desalloue la memoire du tableau
    free(tab->elems);
    // Le tableau est dorenavant vide
    tab->taille = 0;
}

bool creer_t_mat_int_dyn(t_mat_int_dyn* mat, int dim) {
    mat->elems = creer_mat_int(dim);
    mat->dim = dim;

    return mat->elems != NULL;
}

void detruire_t_mat_int_dyn(t_mat_int_dyn* mat) {
    // On desalloue la memoire de la matrice
    detruire_mat_int(mat->elems, mat->dim);
    // La matrice est dorenavant nulle
    mat->dim = 0;
}

bool creer_t_tab_char_star_dyn(t_tab_char_star_dyn* tab, int dim) {
    tab->elems = creer_tab_char_star(dim);
    tab->taille = dim;

    return tab->elems != NULL;
}

void detruite_t_tab_char_star_dyn(t_tab_char_star_dyn* tab) {
    // On desalloue la memoire du tableau
    free(tab->elems);
    // Le tableau est dorenavant vide
    tab->taille = 0;
}

bool creer_t_mat_char_star_dyn(t_mat_char_star_dyn* mat, int lignes, int colonnes) {
    mat->elems = creer_mat_char_star(lignes, colonnes);
    mat->lignes = lignes;
    mat->colonnes = colonnes;

    return mat->elems != NULL;
}

void detruire_t_mat_char_star_dyn(t_mat_char_star_dyn* mat) {
    // On desalloue la memoire de la matrice
    detruire_mat_char_star(mat->elems, mat->lignes);
    // La matrice est dorenavant nulle
    mat->lignes = 0;
    mat->colonnes = 0;
}
