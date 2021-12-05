#include <unitaires/utils_scrutins.h>

#include <utils_scrutins.h>
#include <assert.h>

// Nb de candidats dans le tableau donne aux tests de departager_candidats
#define NB_CANDIDATS_DEPARTAGER_TESTS 4


//
// departager_candidats
//

// Candidats utilises pour les tests dont on connait l'age
static t_candidat test_m = { "M", 4 };
static t_candidat test_s = { "S", 1 };
static t_candidat test_l = { "L", 0 };
static t_candidat test_h = { "H", 5 };
// Candidats utilises pour les tests dont on ne connait pas l'age
static t_candidat test_a = { "A", 2 };
static t_candidat test_b = { "B", 3 };

// Utilitaire pour tester si, avec le tableau de candidats donne, la
// fonction departager_candidats retourne l'ID attendu
static void tester_departager_candidats(t_candidat candidats[NB_CANDIDATS_DEPARTAGER_TESTS], int attendu) {
    t_candidats tab = { candidats, NB_CANDIDATS_DEPARTAGER_TESTS };

    assert(departager_candidats(tab) == attendu);
}

// On connait l'age de tous les candidats, le 1er est plus le age
void departager_candidats_1er_plus_age() {
    t_candidat candidats[] = { test_m, test_s, test_h, test_l };
    tester_departager_candidats(candidats, test_m.id);
}

// On connait l'age de c1 et c2, c1 est plus age
void departager_candidats_3eme_plus_age() {
    t_candidat candidats[] = { test_s, test_h, test_m, test_l };
    tester_departager_candidats(candidats, test_m.id);
}

// Dans les tests qui suivent, au moins 1 des candidats n'a pas d'age
// enregistre, donc on s'attend a ce que le 1er ID (celui de A) soit
// retourne

void departager_candidats_1_age_inconnu() {
    t_candidat candidats[] = { test_a, test_m, test_h, test_l };
    tester_departager_candidats(candidats, test_a.id);;
}

void departager_candidats_2_ages_inconnu() {
    t_candidat candidats[] = { test_l, test_m, test_b, test_a };
    tester_departager_candidats(candidats, test_l.id);
}


// Script principal executant tous les tests de ce module
void tests_unitaires_utils_scrutin() {
    departager_candidats_1er_plus_age();
    departager_candidats_3eme_plus_age();
    departager_candidats_1_age_inconnu();
    departager_candidats_2_ages_inconnu();
}
