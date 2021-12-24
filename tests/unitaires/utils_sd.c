#include <unitaires/utils_sd.h>

#include <assert.h>
#include <stdlib.h>

// Taille valide utilisee pour les tests
#define TAILLE_VALIDE 4
// Taille negative donc invalide utilisee pour les tests
#define TAILLE_INVALIDE -1


// Verifier qu'une liste donnee contient les elements attendus dans le bon ordre
static void verifier_elements_liste(t_liste_simple_int liste, t_tab_int_dyn attendus) {
    assert(liste.taille == attendus.taille); // On verifie le nb d'elements

    // On verifie les elements eux-memes, cellule d'element par cellule d'element
    t_cellule_simple_int* cell_courante = liste.elems;
    for (int i = 0; i < liste.taille; i++) { // Tant que toute la liste n'a pas ete parcourue
        // On verifie que l'element a cette position contient le meme element que le
        // tab des elements attendus au meme endroit
        assert(cell_courante->val == attendus.elems[i]);

        cell_courante = cell_courante->suiv; // On passe a la cellule suivante
    }
}

// Creer une liste non vide contenant les elements [5, 4, 3, 2, 1] pour effectuer
// des tests dessus
static void remplir_elements_liste(t_liste_simple_int* liste) {
    creer_t_liste_simple_int(liste); // Creation d'une liste vide

    // Remplissage en inserant au debut 1, 2, 3, 4, 5 a chaque fois
    for (int i = 1; i <= 5; i++)
        inserer_debut_t_liste_simple_int(liste, i);
}


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
    int** mat = creer_mat_int(TAILLE_VALIDE);

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
    char*** mat = creer_mat_char_star(TAILLE_VALIDE, 10);

    assert(mat != NULL); // Verifie que la memoire est allouee pour les lignes
    for (int i = 0; i < TAILLE_VALIDE; i++) {
        char** tab = mat[i];

        // Verifie que la memoire est allouee pour les colonnes de cette ligne
        assert(tab != NULL);
    }

    detruire_mat_char_star(mat, TAILLE_VALIDE); // Nettoie les ressources utilisees pour le test
}

// Avec une hauteur negative
void creer_mat_char_star_lignes_negatif() {
    assert(creer_mat_char_star(TAILLE_INVALIDE, TAILLE_VALIDE) == NULL);
}

// Avec une largeur negative
void creer_mat_char_star_colonnes_negatif() {
    assert(creer_mat_char_star(TAILLE_VALIDE, TAILLE_INVALIDE) == NULL);
}


//
// creer_tab_candidats
//

// Avec une dimension strictement positive
void creer_tab_candidats_dim_ok() {
    t_candidat* tab = creer_tab_candidats(TAILLE_VALIDE);

    assert(tab != NULL); // Verifie que la memoire est allouee

    free(tab); // Nettoie les ressources utilisees pour le tests
}

// Avec une longueur negative
void creer_tab_candidats_dim_negative() {
    assert(creer_tab_candidats(TAILLE_INVALIDE) == NULL);
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

void detruire_t_tab_int_dyn_test() {
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

void detruire_t_mat_int_dyn_test() {
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

void detruire_t_tab_char_star_dyn_test() {
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
        char** ligne = mat.elems[i];

        assert(ligne != NULL);
    }

    detruire_t_mat_char_star_dyn(&mat); // Nettoie les ressources utilisees pour le test
}

// Avec une hauteur negative
void creer_t_mat_char_star_dyn_lignes_negatif() {
    t_mat_char_star_dyn mat;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_mat_char_star_dyn(&mat, TAILLE_INVALIDE, TAILLE_VALIDE));
}

// Avec une largeur negative
void creer_t_mat_char_star_dyn_colonnes_negatif() {
    t_mat_char_star_dyn mat;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_mat_char_star_dyn(&mat, TAILLE_VALIDE, TAILLE_INVALIDE));
}


//
// detruire_t_mat_char_star_dyn
//

void detruire_t_mat_char_star_dyn_test() {
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


//
// creer_t_candidats_dyn
//

// Avec une longueur strictement positive
void creer_t_candidats_dyn_dim_ok() {
    t_candidats tab;

    // On s'assure que l'initialisation s'est effectuee correctement
    assert(creer_t_candidats_dyn(&tab, TAILLE_VALIDE));

    // On verifie que la memoire a ete allouee
    assert(tab.elems != NULL);
    // Et que la taille a bien ete prise en compte
    assert(tab.nb == TAILLE_VALIDE);

    detruire_t_candidats_dyn(&tab); // Nettoie les ressources utilisees pour le test
}

// Avec une longueur negative
void creer_t_candidats_dyn_dim_negative() {
    t_candidats tab;

    // On s'assure que l'initialisation a plantee
    assert(!creer_t_candidats_dyn(&tab, TAILLE_INVALIDE));
}


//
// detruire_t_candidats_dyn
//

void detruire_t_candidats_dyn_test() {
    t_candidats tab;

    // Si l'initialisation du tableau echoue, alors le reste du test ne
    // fonctionnera pas
    assert(creer_t_candidats_dyn(&tab, TAILLE_VALIDE));

    detruire_t_candidats_dyn(&tab);

    // On verifie que la memoire a ete desallouee
    assert(tab.elems == NULL);
    // On verifie que la taille a ete actualisee
    assert(tab.nb == 0);
}


//
// mettre_t_*_erreur
// Une sd en mode erreur possede un pointeur NULL et des dimensions -1
//

void mettre_t_tab_int_dyn_erreur_test() {
    t_tab_int_dyn tab;
    mettre_t_tab_int_dyn_erreur(&tab);
    
    assert(tab.elems == NULL);
    assert(tab.taille == -1);
}

void mettre_t_mat_int_dyn_erreur_test() {
    t_mat_int_dyn mat;
    mettre_t_mat_int_dyn_erreur(&mat);
    
    assert(mat.elems == NULL);
    assert(mat.dim == -1);
}

void mettre_t_tab_char_star_dyn_erreur_test() {
    t_tab_char_star_dyn tab;
    mettre_t_tab_char_star_dyn_erreur(&tab);
    
    assert(tab.elems == NULL);
    assert(tab.taille == -1);
}

void mettre_t_mat_char_star_dyn_erreur_test() {
    t_mat_char_star_dyn mat;
    mettre_t_mat_char_star_dyn_erreur(&mat);
    
    assert(mat.elems == NULL);
    assert(mat.lignes == -1);
    assert(mat.colonnes == -1);
}

void mettre_t_candidats_erreur_test() {
    t_candidats tab;
    mettre_t_candidats_erreur(&tab);

    assert(tab.elems == NULL);
    assert(tab.nb == -1);
}


//
// est_t_*_erreur
// Lorsqu'une sd a un pointeur vers NULL et toutes ses dimensions a -1,
// alors on considere qu'elle est en mode erreur
//

void est_t_tab_int_dyn_erreur_ok() {
    // On creer un tableau tout a fait valide
    t_tab_int_dyn tab;
    creer_t_tab_int_dyn(&tab, 5);

    assert(!est_t_tab_int_dyn_erreur(tab));
}

void est_t_tab_int_dyn_erreur_erreur() {
    // On creer un tableau aves les proprietes du mode erreur
    t_tab_int_dyn tab = { NULL, -1 };

    assert(est_t_tab_int_dyn_erreur(tab));
}


//
// creer_t_liste_simple_int
//

void creer_t_liste_simple_int_test() {
    t_liste_simple_int liste;
    creer_t_liste_simple_int(&liste);

    // Aucune cellule a la creation de la liste
    assert(liste.elems == NULL);
    assert(liste.taille == 0);
}


//
// detruire_t_liste_simple_int
//

// On essaie de detruire la liste passee en parametre, et on regarde si
// les metadonnes (taille et 1ere cellule) ont bien ete reinitialises
// pour representer une liste vide
static void tester_detruire_t_liste_simple_int(t_liste_simple_int a_detruire) {
    detruire_t_liste_simple_int(&a_detruire);

    // Aucune cellule dans la liste
    assert(a_detruire.elems == NULL);
    assert(a_detruire.taille == 0);
}

void detruire_t_liste_simple_int_non_vide() {
    t_liste_simple_int liste;
    remplir_elements_liste(&liste);

    // Les ressources du test seront liberees quand la liste sera detruite
    // a l'interieur de cette fonction utilitaire
    tester_detruire_t_liste_simple_int(liste);
}

void detruire_t_liste_simple_int_vide() {
    t_liste_simple_int liste_vide;
    creer_t_liste_simple_int(&liste_vide);

    tester_detruire_t_liste_simple_int(liste_vide);
}


//
// inserer_debut_t_liste_simple_int
//

void inserer_debut_t_liste_simple_int_test() {
    t_liste_simple_int liste;
    // Cette fonction appelle inserer_debut_t_liste_simple_int() que l'on souhaite
    // tester
    remplir_elements_liste(&liste);

    // On s'attend donc a finalement avoir ces elements dans cet ordre
    int elems_attendus[] = { 5, 4, 3, 2, 1 };
    t_tab_int_dyn tab_attendus = { elems_attendus, 5 };

    verifier_elements_liste(liste, tab_attendus);
    detruire_t_liste_simple_int(&liste); // On n'oublie pas la liberation des ressources du test
}


//
// supprimer_valeur_t_liste_simple_int
//

void supprimer_valeur_t_liste_simple_int_non_trouve() {
    t_liste_simple_int liste;
    remplir_elements_liste(&liste);

    // On s'attend a ne pas pouvoir trouver 6 dans la liste
    assert(!supprimer_valeur_t_liste_simple_int(&liste, 6));
    // Liste qui sera donc toujours la meme
    int elems_attendus[] = { 5, 4, 3, 2, 1 };
    t_tab_int_dyn tab_attendus = { elems_attendus, 5 }; // Tab des elems attendus dans la liste
    verifier_elements_liste(liste, tab_attendus);

    detruire_t_liste_simple_int(&liste);
}

void supprimer_valeur_t_liste_simple_int_trouve_debut() {
    t_liste_simple_int liste;
    remplir_elements_liste(&liste);

    // On s'attend a trouver 5 au debut de la liste
    assert(supprimer_valeur_t_liste_simple_int(&liste, 5));
    // Liste qui sera donc raccourcie a partir de 4
    int elems_attendus[] = { 4, 3, 2, 1 };
    t_tab_int_dyn tab_attendus = { elems_attendus, 4 };
    verifier_elements_liste(liste, tab_attendus);

    detruire_t_liste_simple_int(&liste);
}

void supprimer_valeur_t_liste_simple_int_trouve_milieu() {
    t_liste_simple_int liste;
    remplir_elements_liste(&liste);

    // On s'attend a trouver 3 au milieu de la liste
    assert(supprimer_valeur_t_liste_simple_int(&liste, 3));
    // Liste qui sera donc tronquee de la valeur 3
    int elems_attendus[] = { 5, 4, 2, 1 };
    t_tab_int_dyn tab_attendus = { elems_attendus, 4 };
    verifier_elements_liste(liste, tab_attendus);

    detruire_t_liste_simple_int(&liste);
}


//
// retirer_premier_t_liste_simple_int
//

void retirer_premier_t_liste_simple_int_plusieurs_elems() {
    t_liste_simple_int liste;
    remplir_elements_liste(&liste); // On a 5 elements dans la liste

    assert(retirer_premier_t_liste_simple_int(&liste) == 5); // On a retire 5
    // On n'a plus que 4 elements dans la liste
    int elems_attendus[] = { 4, 3, 2, 1 };
    t_tab_int_dyn tab_attendus = { elems_attendus, 4 };
    verifier_elements_liste(liste, tab_attendus);

    detruire_t_liste_simple_int(&liste);
}

void retirer_premier_t_liste_simple_int_unique_elem() {
    t_liste_simple_int liste;
    // Cette liste n'a qu'un seul element
    creer_t_liste_simple_int(&liste);
    inserer_debut_t_liste_simple_int(&liste, 1);

    assert(retirer_premier_t_liste_simple_int(&liste) == 1); // On a retire 1
    // Maintenant, la liste est vide
    int elems_attendus[0] = {};
    t_tab_int_dyn tab_attendus = { elems_attendus, 0 };
    verifier_elements_liste(liste, tab_attendus);

    detruire_t_liste_simple_int(&liste);
}


// Scripts des tests appelle par main_utilitaires.c
void tests_unitaires_utils_sd() {
    creer_tab_int_dim_ok();
    creer_tab_int_dim_negative();

    creer_mat_int_dim_ok();
    creer_mat_int_dim_negative();

    creer_tab_char_star_dim_ok();
    creer_tab_char_star_dim_negative();

    creer_mat_char_star_dim_ok();
    creer_mat_char_star_lignes_negatif();
    creer_mat_char_star_colonnes_negatif();

    creer_tab_candidats_dim_ok();
    creer_tab_candidats_dim_negative();

    creer_t_tab_int_dyn_dim_ok();
    creer_t_tab_int_dyn_dim_negative();

    detruire_t_tab_int_dyn_test();

    creer_t_mat_int_dyn_dim_ok();
    creer_t_mat_int_dyn_dim_negative();

    detruire_t_tab_char_star_dyn_test();

    creer_t_tab_char_star_dyn_dim_ok();
    creer_t_tab_char_star_dyn_dim_negative();

    detruire_t_tab_char_star_dyn_test();

    creer_t_mat_char_star_dyn_dim_ok();
    creer_t_mat_char_star_dyn_lignes_negatif();
    creer_t_mat_char_star_dyn_colonnes_negatif();

    detruire_t_mat_char_star_dyn_test();

    creer_t_candidats_dyn_dim_ok();
    creer_t_candidats_dyn_dim_negative();

    mettre_t_tab_int_dyn_erreur_test();
    mettre_t_mat_int_dyn_erreur_test();
    mettre_t_tab_char_star_dyn_erreur_test();
    mettre_t_mat_char_star_dyn_erreur_test();
    mettre_t_candidats_erreur_test();

    est_t_tab_int_dyn_erreur_ok();
    est_t_tab_int_dyn_erreur_erreur();

    detruire_t_candidats_dyn_test();

    creer_t_liste_simple_int_test();
    
    detruire_t_liste_simple_int_non_vide();
    detruire_t_liste_simple_int_vide();
    
    inserer_debut_t_liste_simple_int_test();
    
    supprimer_valeur_t_liste_simple_int_non_trouve();
    supprimer_valeur_t_liste_simple_int_trouve_debut();
    supprimer_valeur_t_liste_simple_int_trouve_milieu();
    
    retirer_premier_t_liste_simple_int_plusieurs_elems();
    retirer_premier_t_liste_simple_int_unique_elem();
}
