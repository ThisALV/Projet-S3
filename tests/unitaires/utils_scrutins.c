#include <unitaires/utils_scrutins.h>

#include <utils_scrutins.h>
#include <assert.h>

// Nb de candidats dans le tableau donne aux tests de departager_candidats
#define NB_CANDIDATS_DEPARTAGER_TESTS 4

// Nb de candidats dans la BDD utilisee par les tests de ce module
#define NB_CANDIDATS_TESTS 4
// Nb d'electeurs pour connaitre le nb de ballots utilises dans les tests
// de ce module
#define NB_BALLOTS_TESTS 5


// Tableau pointe par la BDD de candidats
static t_candidat candidats_tests[NB_CANDIDATS_TESTS] = {
    { "L", 0 }, { "M", 1 }, { "S", 2 }, { "H", 3 }
};

// Retourne une BDD de candidats utilisables pour les tests de ce module
static t_candidats generer_candidats_tests() {
    t_candidats candidats = { candidats_tests, NB_CANDIDATS_TESTS };
    return candidats;
}


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


//
// compter_voix_ballots
//

// Pour un tableau de tetes de listes donne pour chaque ballot, verifie
// si on compte bien le nb de voix attendu avec chaque candidat
static void tester_compter_voix_ballots(t_tab_int_dyn tetes_de_listes[NB_BALLOTS_TESTS], int nb_voix_attendu[NB_CANDIDATS_TESTS]) {
    // On initialise le tableau qui comptera les voix des candidats
    t_tab_int_dyn nb_voix_obtenu;
    creer_t_tab_int_dyn(&nb_voix_obtenu, NB_CANDIDATS_TESTS);

    compter_voix_ballots(&nb_voix_obtenu, generer_candidats_tests(), tetes_de_listes, NB_BALLOTS_TESTS);

    // On a cree nous-meme le tableau, on sait qu'il a la bonne taille
    // On va verifier qu'il y a le bon nb de voix pour chaque candidat
    for (int candidat_i = 0; candidat_i < NB_CANDIDATS_TESTS; candidat_i++)
        assert(nb_voix_obtenu.elems[candidat_i] == nb_voix_attendu[candidat_i]);

    // On a verifie le tableau, on n'en a plus besoin
    detruire_t_tab_int_dyn(&nb_voix_obtenu);
}

// Un seul gagnant dans chaque ballot de votes
void compter_voix_ballots_un_gagnant() {
    int gagnants1[] = { 2 };
    int gagnants2[] = { 0 };
    int gagnants3[] = { 0 };
    int gagnants4[] = { 1 };
    int gagnants5[] = { 2 };
    // Donc :
    int nb_voix_attendu[] = { 2, 1, 2, 0 };

    // On forme le tableau des tetes de listes a partir de nos gagnants
    // statiques pour chaque ballot
    t_tab_int_dyn tetes_de_listes[NB_BALLOTS_TESTS] = {
        { gagnants1, 1 },
        { gagnants2, 1 },
        { gagnants3, 1 },
        { gagnants4, 1 },
        { gagnants5, 1 }
    };

    tester_compter_voix_ballots(tetes_de_listes, nb_voix_attendu);
}

// Certains ballots de votes ont plusieurs gagnants a egalite
void compter_voix_ballots_egalites() {
    int gagnants1[] = { 3, 2 };     // Gagnant : 2
    int gagnants2[] = { 0 };        // Gagnant : 0
    int gagnants3[] = { 0, 3 };     // Gagnant : 3
    int gagnants4[] = { 2, 0, 3 };  // Gagnant : 2
    int gagnants5[] = { 2 };        // Gagnant : 2
    // Donc :
    int nb_voix_attendu[] = { 1, 0, 3, 1 };

    // On forme le tableau des tetes de listes a partir de nos gagnants
    // statiques pour chaque ballot
    t_tab_int_dyn tetes_de_listes[NB_BALLOTS_TESTS] = {
        { gagnants1, 2 },
        { gagnants2, 1 },
        { gagnants3, 2 },
        { gagnants4, 3 },
        { gagnants5, 1 }
    };

    tester_compter_voix_ballots(tetes_de_listes, nb_voix_attendu);
}


//
// compare_voix_ballots
//

// Verifie si, pour un nb de voix pour chaque candidat donne, on a bien
// l'ID gagnant attendu
static void tester_comparer_voix_ballots(int nb_voix[NB_CANDIDATS_TESTS], int id_gagnant_attendu) {
    t_tab_int_dyn nb_voix_candidats = { nb_voix, NB_CANDIDATS_TESTS };

    assert(comparer_voix_ballots(nb_voix_candidats, generer_candidats_tests()) == id_gagnant_attendu);
}

// Tous les candidats ont eu un nb de voix different
void comparer_voix_ballots_normal() {
    int nb_voix[] = { 0, 1, 3, 2 };
    tester_comparer_voix_ballots(nb_voix, 2);
}

// Certains candidats ont eu le meme nb de voix
void comparer_voix_ballots_egalite() {
    int nb_voix[] = { 1, 1, 3, 3 };
    // Le 3eme candidat est S qui est plus age que le 4eme candidat H,
    // donc c'est le 3eme candidat qui devrait etre retourne
    tester_comparer_voix_ballots(nb_voix, 2);
}


//
// vainqueur_uninominale
//

void vainqueur_uninominale_avec_egalites() {
    // Les IDs des gagnants pour chaque ballot
    int gagnants1[] = { 3, 2 };
    int gagnants2[] = { 0 };
    int gagnants3[] = { 0, 3 };
    int gagnants4[] = { 3 };
    int gagnants5[] = { 2 };
    // Donc on aura un comptage de voix == { 1, 0, 2, 2 }
    // Le 3eme candidat etant plus age que le 4eme, alors on s'attend a
    // ce que le gagnant ait l'ID 2

    t_tab_int_dyn tetes_de_listes[NB_BALLOTS_TESTS] = {
        { gagnants1, 2 },
        { gagnants2, 1 },
        { gagnants3, 2 },
        { gagnants4, 3 },
        { gagnants5, 1 }
    };

    // Avec le BDD de test et les tetes de listes formees ci-dessus, on s'attend
    // a ce que le 3eme candidat (ID 2) gagne
    assert(vainqueur_uninominale(generer_candidats_tests(), tetes_de_listes, NB_BALLOTS_TESTS) == 2);
}


// Script principal executant tous les tests de ce module
void tests_unitaires_utils_scrutin() {
    departager_candidats_1er_plus_age();
    departager_candidats_3eme_plus_age();
    departager_candidats_1_age_inconnu();
    departager_candidats_2_ages_inconnu();

    compter_voix_ballots_un_gagnant();
    compter_voix_ballots_egalites();

    comparer_voix_ballots_normal();
    comparer_voix_ballots_egalite();

    vainqueur_uninominale_avec_egalites();
}
