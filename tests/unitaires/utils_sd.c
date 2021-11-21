#include <unitaires/utils_sd.h>

#include <assert.h>
#include <stdlib.h>

// Taille valide utilisee pour les tests
#define TAILLE_VALIDE 4
// Taille negative donc invalide utilisee pour les tests
#define TAILLE_INVALIDE -1


//
// creer_tab_int
//

// Avec une dimension strictement positive
void creer_tab_int_dim_ok() {
    int* tab = creer_tab_int(TAILLE_VALIDE);

    assert(tab != NULL); // Verifie que la memoire est allouee
    // Verifie que chaque elem de cette memoire est a 0 (calloc)
    for (int i = 0; i < TAILLE_VALIDE; i++)
        assert(tab[i] == 0);

    free(tab); // Nettoie les ressources utilisees pour le tests
}

// Avec une dimension nulle
void creer_tab_int_dim_nulle() {
    assert(creer_tab_int(0) == NULL);
}

// Avec une dimension negative
void creer_tab_int_dim_negative() {
    assert(creer_tab_int(TAILLE_INVALIDE) == NULL);
}


//
// creer_mat_int
//

// Avec une dimension strictement positive
void creer_mat_int_dim_ok() {
    int* mat = creer_mat_int(TAILLE_VALIDE);

    assert(mat != NULL); // Verifie que la memoire est allouee pour les lignes
    for (int i = 0; i < TAILLE_VALIDE; i++) {
        int* tab = mat[i];

        // Verifie que la memoire est allouee pour les colonnes de cette ligne
        assert(tab != NULL);
        // Verifie que chaque elem de cette memoire est a 0 (calloc)
        for (int j = 0; j < TAILLE_VALIDE; j++)
            assert(tab[j] == 0);
    }

    detruire_mat_int(mat, TAILLE_VALIDE); // Nettoie les ressources utilisees pour le tests
}

// Avec une dimension negative
void creer_mat_int_dim_negative() {
    assert(creer_mat_int(TAILLE_INVALIDE) == NULL);
}


//
// creer_tab_char_star
//

// Avec une dimension strictement positive
void creer_tab_char_star_dim_ok() {
    char** tab = creer_tab_char_star(TAILLE_VALIDE);

    assert(tab != NULL); // Verifie que la memoire est allouee

    free(tab); // Nettoie les ressources utilisees pour le tests
}

// Avec une longueur negative
void creer_tab_char_star_dim_negative() {
    assert(creer_tab_char_star(TAILLE_INVALIDE) == NULL);
}


//
// creer_mat_char_star
//

// Avec une dimension strictement positive
void creer_mat_char_star_dim_ok() {
    int* mat = creer_mat_char_star(TAILLE_VALIDE, 10);

    assert(mat != NULL); // Verifie que la memoire est allouee pour les lignes
    for (int i = 0; i < TAILLE_VALIDE; i++) {
        int* tab = mat[i];

        // Verifie que la memoire est allouee pour les colonnes de cette ligne
        assert(tab != NULL);
    }

    detruire_mat_char_star(mat, TAILLE_VALIDE); // Nettoie les ressources utilisees pour le test
}

// Avec une hauteur negative
void creer_mat_char_star_lignes_negative() {
    assert(creer_mat_char_star(TAILLE_INVALIDE, TAILLE_VALIDE) == NULL);
}

// Avec une largeur negative
void creer_mat_char_star_colonnes_negative() {
    assert(creer_mat_char_star(TAILLE_VALIDE, TAILLE_INVALIDE) == NULL);
}


//
// creer_t_tab_int_dyn
//

// Avec une longueur strictement positive
void creer_t_tab_int_dyn_dim_ok() {
    t_tab_int_dyn tab;

    // On s'assure que l'initialisation s'est effectuee correctement
    assert(creer_t_tab_int_dyn(&tab, TAILLE_VALIDE));

    // On verifie que la memoire a ete allouee
    assert(tab.elems != NULL);
    // Et que la taille a bien ete prise en compte
    assert(tab.taille == TAILLE_VALIDE);

    // On verifie que chaque element est initilise a 0 (calloc)
    for (int i = 0; i < TAILLE_VALIDE; i++)
        assert(tab.elems[i] == 0);

    detruire_t_tab_int_dyn(&tab); // Nettoie les ressources utilisees pour le test
}

// Avec une longueur negative
void creer_t_tab_int_dyn_dim_negative() {
    t_tab_int_dyn tab;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_tab_int_dyn(&tab, TAILLE_INVALIDE));
}


//
// detruire_t_tab_int_dyn
//

void detruire_t_tab_int_dyn() {
    t_tab_int_dyn tab;

    // Si l'initialisation du tableau echoue, alors le reste du test ne
    // fonctionnera pas
    assert(creer_t_tab_int_dyn(&tab, TAILLE_VALIDE));

    detruire_t_tab_int_dyn(&tab);

    // On verifie que la memoire a ete desallouee
    assert(tab.elems == NULL);
    // On verifie que la taille a ete actualisee
    assert(tab.taille == 0);
}


//
// creer_t_mat_int_dyn
//

// Avec une dimension strictement positive
void creer_t_mat_int_dyn_dim_ok() {
    t_mat_int_dyn mat;

    // On s'assure que l'initialisation s'est effectuee correctement
    assert(creer_t_mat_int_dyn(&mat, TAILLE_VALIDE));

    // On verifie que la memoire a ete allouee
    assert(mat.elems != NULL);
    // Et que la dimension a bien ete prise en compte
    assert(mat.dim == TAILLE_VALIDE);

    // On verifie que chaque ligne est allouee en memoire
    for (int i = 0; i < TAILLE_VALIDE; i++) {
        int* ligne = mat.elems[i];

        assert(ligne != NULL);

        // On verifie que chaque col de la ligne est initialisee a 0
        for (int j = 0; j < TAILLE_VALIDE; j++)
            assert(ligne[j] == 0);
    }

    detruire_t_mat_int_dyn(&mat); // Nettoie les ressources utilisees pour le test
}

// Avec une dimension negative
void creer_t_mat_int_dyn_dim_negative() {
    t_mat_int_dyn mat;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_mat_int_dyn(&mat, TAILLE_INVALIDE));
}


//
// detruire_t_mat_int_dyn
//

void detruire_t_mat_int_dyn() {
    t_mat_int_dyn mat;

    // Si l'initialisation de la matrice echoue, alors le reste du test ne
    // fonctionnera pas
    assert(creer_t_mat_int_dyn(&mat, TAILLE_VALIDE));

    detruire_t_mat_int_dyn(&mat);

    // On verifie que la memoire a ete desallouee
    assert(mat.elems == NULL);
    // On verifie que la dimension de la matrice carree a ete actualisee
    assert(mat.dim == 0);
}


//
// creer_t_tab_char_star_dyn
//

// Avec une longueur strictement positive
void creer_t_tab_char_star_dyn_dim_ok() {
    t_tab_char_star_dyn tab;

    // On s'assure que l'initialisation s'est effectuee correctement
    assert(creer_t_tab_char_star_dyn(&tab, TAILLE_VALIDE));

    // On verifie que la memoire a ete allouee
    assert(tab.elems != NULL);
    // Et que la taille a bien ete prise en compte
    assert(tab.taille == TAILLE_VALIDE);

    detruire_t_tab_char_star_dyn(&tab); // Nettoie les ressources utilisees pour le test
}

// Avec une longueur negative
void creer_t_tab_char_star_dyn_dim_negative() {
    t_tab_char_star_dyn tab;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_tab_char_star_dyn(&tab, TAILLE_INVALIDE));
}


//
// detruire_t_tab_char_star_dyn
//

void detruire_t_tab_char_star_dyn() {
    t_tab_char_star_dyn tab;

    // Si l'initialisation du tableau echoue, alors le reste du test ne
    // fonctionnera pas
    assert(creer_t_tab_char_star_dyn(&tab, TAILLE_VALIDE));

    detruire_t_tab_char_star_dyn(&tab);

    // On verifie que la memoire a ete desallouee
    assert(tab.elems == NULL);
    // On verifie que la taille a ete actualisee
    assert(tab.taille == 0);
}


//
// creer_t_mat_char_star_dyn
//

// Avec des dimensions strictement positive
void creer_t_mat_char_star_dyn_dim_ok() {
    t_mat_char_star_dyn mat;

    // On s'assure que l'initialisation s'est effectuee correctement
    assert(creer_t_mat_char_star_dyn(&mat, TAILLE_VALIDE, 2));

    // On verifie que la memoire a ete allouee
    assert(mat.elems != NULL);
    // Et que les dimensions ont bien ete prises en compte
    assert(mat.lignes == TAILLE_VALIDE);
    assert(mat.colonnes == 2);

    // On verifie que chaque ligne est allouee en memoire
    for (int i = 0; i < TAILLE_VALIDE; i++) {
        char* ligne = mat.elems[i];

        assert(ligne != NULL);
    }

    detruire_t_mat_char_star_dyn(&mat); // Nettoie les ressources utilisees pour le test
}

// Avec une hauteur negative
void creer_t_mat_char_star_dyn_lignes_negative() {
    t_mat_char_star_dyn mat;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_mat_char_star_dyn(&mat, TAILLE_INVALIDE, TAILLE_VALIDE));
}

// Avec une largeur negative
void creer_t_mat_char_star_dyn_colonnes_negative() {
    t_mat_char_star_dyn mat;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_mat_char_star_dyn(&mat, TAILLE_VALIDE, TAILLE_INVALIDE));
}


//
// detruire_t_mat_char_star_dyn
//

void detruire_t_mat_char_star_dyn() {
    t_mat_char_star_dyn mat;

    // Si l'initialisation de la matrice echoue, alors le reste du test ne
    // fonctionnera pas
    assert(creer_t_mat_char_star_dyn(&mat, TAILLE_VALIDE, TAILLE_VALIDE));

    detruire_t_mat_char_star_dyn(&mat);

    // On verifie que la memoire a ete desallouee
    assert(mat.elems == NULL);
    // On verifie que les dimensions ont ete actualisees
    assert(mat.lignes == 0);
    assert(mat.colonnes == 0);
}


// Scripts des tests appelle par main_utilitaires.c
void tests_unitaires_utils_sd() {
    creer_tab_int_dim_ok();
    creer_tab_int_dim_negative();

    creer_mat_int_dim_ok();
    creer_tab_int_dim_negative();

    creer_tab_char_star_dim_ok();
    creer_tab_char_star_dim_negative();

    creer_mat_char_star_dim_ok();
    creer_mat_char_star_lignes_negatif();
    creer_mat_char_star_colonnes_negatif();

    creer_t_tab_int_dyn_dim_ok();
    creer_t_tab_int_dyn_dim_negative();

    creer_t_mat_int_dyn_dim_ok();
    creer_t_mat_int_dyn_dim_negative();

    creer_t_tab_char_star_dim_ok();
    creer_t_tab_char_star_dim_negative();

    creer_t_mat_char_star_dim_ok();
    creer_t_mat_char_star_lignes_negatif();
    creer_t_mat_char_star_colonnes_negatif();
}
