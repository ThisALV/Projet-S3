#include <unitaires/utils_scrutins.h>

#include <utils_scrutins.h>
#include <assert.h>


//
// departager_candidats
//

// Candidats utilises pour les tests dont on connait l'age
static t_candidat test_m = { "M", 0 };
static t_candidat test_s = { "S", 1 };
// Candidats utilises pour les tests dont on ne connait pas l'age
static t_candidat test_a = { "A", 2 };
static t_candidat test_b = { "B", 3 };

// On connait l'age de c1 et c2, c1 est plus age
void departager_candidats_c1_plus_age() {
    // On s'attend a ce que le candidat plus age c1 soit retourne
    assert(departager_candidats(test_m, test_s) == test_m.id);
}

// On connait l'age de c1 et c2, c1 est plus age
void departager_candidats_c2_plus_age() {
    // On s'attend a ce que le candidat plus age c2 soit retourne
    assert(departager_candidats(test_s, test_m) == test_m.id);
}

// Dans les tests qui suivent, au moins 1 des 2 candidats n'a pas d'age
// enregistre, donc on s'attend a ce que c1 soit retourne

void departager_candidats_c1_age_inconnu() {
    assert(departager_candidats(test_m, test_a) == test_m.id);
}

void departager_candidats_c2_age_inconnu() {
    assert(departager_candidats(test_a, test_m) == test_a.id);
}

void departager_candidats_ages_inconnus() {
    assert(departager_candidats(test_b, test_a) == test_b.id);
}


// Script principal executant tous les tests de ce module
void tests_unitaires_utils_scrutin() {
    departager_candidats_c1_plus_age();
    departager_candidats_c2_plus_age();
    departager_candidats_c1_age_inconnu();
    departager_candidats_c2_age_inconnu();
    departager_candidats_ages_inconnus();
}
