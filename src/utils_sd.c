#include <utils_sd.h>

#include <stdlib.h>
#include <erreur.h>

// Utilisee sur les dimensions des SDD pour verifier leur validite
#define VERIFIER_DIM(dim) if (dim < 1) return NULL


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
    verifier_alloc(lignes, "Allocation matrice entiers");

    // On initialise chacune de ces lignes avec un tableau de taille dim
    for (int i = 0; i < dim; i++) {
        int* ligne =  creer_tab_int(dim);
        // On verifie que chaque ligne soit allouee correctement
        verifier_alloc(ligne, "Allocation ligne entiers");

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
    verifier_alloc(hauteur, "Allocation matrice char*");

    // On initialise chacune de ces lignes avec un tableau de taille colonnes
    for (int i = 0; i < lignes; i++) {
        char** ligne = creer_tab_char_star(colonnes);
        // On verifie que chaque ligne soit allouee correctement
        verifier_alloc(ligne, "Allocation ligne char*");

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

t_candidat* creer_tab_candidats(int dim) {
    VERIFIER_DIM(dim);

    return (t_candidat*) malloc(sizeof(t_candidat) * dim);
}

bool creer_t_tab_int_dyn(t_tab_int_dyn* tab, int dim) {
    tab->elems = creer_tab_int(dim);
    tab->taille = dim;

    // Si la SDD allouee retournee n'est pas NULL, alors on sait que l'operation
    // s'est bien deroulee
    return tab->elems != NULL;
}

void detruire_t_tab_int_dyn(t_tab_int_dyn* tab) {
    // On desalloue la memoire du tableau
    free(tab->elems);

    // Il n'y a plus de memoire pour cette structure de donnees
    tab->elems = NULL;
    // Le tableau est dorenavant vide
    tab->taille = 0;
}

void mettre_t_tab_int_dyn_erreur(t_tab_int_dyn* tab) {
    tab->elems = NULL;
    tab->taille = -1;
}

bool est_t_tab_int_dyn_erreur(t_tab_int_dyn tab) {
    return tab.elems == NULL && tab.taille == -1;
}

bool creer_t_mat_int_dyn(t_mat_int_dyn* mat, int dim) {
    mat->elems = creer_mat_int(dim);
    mat->dim = dim;

    return mat->elems != NULL;
}

void detruire_t_mat_int_dyn(t_mat_int_dyn* mat) {
    // On desalloue la memoire de la matrice
    detruire_mat_int(mat->elems, mat->dim);

    // Il n'y a plus de memoire pour cette structure de donnees
    mat->elems = NULL;
    // La matrice est dorenavant nulle
    mat->dim = 0;
}

void mettre_t_mat_int_dyn_erreur(t_mat_int_dyn* mat) {
    mat->elems = NULL;
    mat->dim = -1;
}

bool est_t_mat_int_dyn_erreur(t_mat_int_dyn mat) {
    return mat.elems == NULL && mat.dim == -1;
}

bool creer_t_tab_char_star_dyn(t_tab_char_star_dyn* tab, int dim) {
    tab->elems = creer_tab_char_star(dim);
    tab->taille = dim;

    return tab->elems != NULL;
}

void detruire_t_tab_char_star_dyn(t_tab_char_star_dyn* tab) {
    // On desalloue la memoire du tableau
    free(tab->elems);

    tab->elems = NULL;
    // Le tableau est dorenavant vide
    tab->taille = 0;
}

void mettre_t_tab_char_star_dyn_erreur(t_tab_char_star_dyn* tab) {
    tab->elems = NULL;
    tab->taille = -1;
}

bool est_t_tab_char_star_dyn_erreur(t_tab_char_star_dyn tab) {
    return tab.elems == NULL && tab.taille == -1;
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

    // Il n'y a plus de memoire pour cette structure de donnees
    mat->elems = NULL;
    // La matrice est dorenavant nulle
    mat->lignes = 0;
    mat->colonnes = 0;
}

void mettre_t_mat_char_star_dyn_erreur(t_mat_char_star_dyn* mat) {
    mat->elems = NULL;
    mat->lignes = -1;
    mat->colonnes = -1;
}

bool est_t_mat_char_star_dyn_erreur(t_mat_char_star_dyn mat) {
    return mat.elems == NULL && mat.lignes == -1 && mat.colonnes == -1;
}

bool creer_t_candidats_dyn(t_candidats* tab, int dim) {
    // On procede comme pour la creation des autres tableaux dynamiques
    
    tab->elems = creer_tab_candidats(dim);
    tab->nb = dim;

    return tab->elems != NULL;
}

void detruire_t_candidats_dyn(t_candidats* tab) {
    // On procede comme pour la destruction des autres tableaux dynamiques
    
    free(tab->elems);

    tab->elems = NULL;
    tab->nb = 0;
}

void mettre_t_candidats_erreur(t_candidats* tab) {
    tab->elems = NULL;
    tab->nb = -1;
}

bool est_t_candidats_erreur(t_candidats tab) {
    return tab.elems == NULL && tab.nb == -1;
}

void creer_t_liste_simple_int(t_liste_simple_int* liste) {
    liste->elems = NULL; // Aucun 1er element, liste initialisee vide
    liste->taille = 0;
}

// Permet de desallouer recursivement toutes les cellules contenues dans une liste
static void detruire_cellules_liste_simple_int(t_cellule_simple_int* premier_elem) {
    // Terminaison ; On s'arrete apres la derniere cellule de la liste
    if (premier_elem == NULL)
        return;

    // On desalloue le reste de la liste
    detruire_cellules_liste_simple_int(premier_elem->suiv);

    free(premier_elem); // Puis on desalloue la cellule courante
}

void detruire_t_liste_simple_int(t_liste_simple_int* liste) {
    // On desalloue la memoire des cellues
    detruire_cellules_liste_simple_int(liste->elems);

    // On remet la liste a l'etat initial (vide)
    creer_t_liste_simple_int(liste);
}

void inserer_debut_t_liste_simple_int(t_liste_simple_int* liste, int val) {
    // On creer la nouvelle cellule
    t_cellule_simple_int* nouv = (t_cellule_simple_int*) malloc(sizeof(t_cellule_simple_int));
    verifier_alloc(nouv, "Allocation nouvelle cellule");
    nouv->suiv = liste->elems; // Cette cellule precede la liste courante
    nouv->val = val;

    liste->taille++; // Une cellule vient d'etre ajoutee
    liste->elems = nouv; // Le debut de la liste a change
}

// Desalloue la cellule en memoire et fait le lien entre prec et suppr->suiv.
// Si prec vaut NULL, alors c'est qu'on est au debut de la liste.
// Cette fonction s'occupe aussi de mettre a jour les metadonnees de la liste.
static void supprimer_cellule_liste_simple_int(t_liste_simple_int* liste, t_cellule_simple_int* prec, t_cellule_simple_int* suppr) {    
    // Un element va etre supprime de la liste
    liste->taille--;

    t_cellule_simple_int* suiv = suppr->suiv; // Cell qui va etre connectee a prec
    // Si on n'est pas au debut de la liste, on raccorde les cells prec et suiv
    if (prec != NULL)
        prec->suiv = suiv;
    else // Si on est au debut de la liste, la 1ere cell doit etre mise a jour
        liste->elems = suiv;

    free(suppr); // On desalloue la memoire de la cell courante
}

bool supprimer_valeur_t_liste_simple_int(t_liste_simple_int* liste, int val) {
    t_cellule_simple_int* premiere_cell = liste->elems;

    // Cell precendent la cell courante, vaudra NULL si cell courante est la 1ere
    t_cellule_simple_int* precedente = NULL;
    // Pour chaque cellule de la liste
    for (t_cellule_simple_int* cell = premiere_cell; cell != NULL; cell = cell->suiv) {
        // On cherche une cellule contenant la valeur passee entre parametres
        if (cell->val == val) {
            supprimer_cellule_liste_simple_int(liste, precedente, cell);
            return true; // On a effectivement supprime un element de la liste
        }

        // La cellule precedente de la prochaine iteration est la cellule courante
        precedente = cell;
    }

    return false; // On a parcouru toute la liste sans supprimer d'element
}

int retirer_premier_t_liste_simple_int(t_liste_simple_int* liste) {
    t_cellule_simple_int* premiere_cell = liste->elems;

    int valeur_retiree = premiere_cell->val; // On sauvegarde la valeur qu'on va suppr
    supprimer_cellule_liste_simple_int(liste, NULL, premiere_cell); // Suppr 1ere valeur de la liste

    return valeur_retiree; // On retourne la 1ere premiere valeur, qui a ete suppr
}
