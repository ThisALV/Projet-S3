#include <unitaires/condorcet_minimax.h>

#include <assert.h>
#include <condorcet_minimax.h>
#include <utils_sd.h>

// Nombre de candidats representes par les matrices de duels utilisees
// pour ces tests
#define NB_CANDIDATS_TESTS 4


// Tableau pointe par la BDD des candidats utilisee pour les tests
static t_candidat candidats_tests[NB_CANDIDATS_TESTS] = {
    { "H", 0 }, { "S", 1 }, { "B", 2 }, { "M", 3 }
};


// Initialise dynamiquement la SDD matrice donnee avec le nb de candidats
// et en copiant le contenu de la matrice `duels`
static void creer_mat_duels_compatible(int duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS], t_mat_int_dyn* mat) {
    creer_t_mat_int_dyn(mat, NB_CANDIDATS_TESTS);

    for (int l = 0; l < NB_CANDIDATS_TESTS; l++)
        for (int c = 0; c < NB_CANDIDATS_TESTS; c++)
            mat->elems[l][c] = duels[l][c];
}

// Fourni a la methode condorcet + minimax une matrice de duels et verifie
// que l'ID du vainqueur est bien celui attendu

static void tester_condorcet_minimax(int duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS], int vainqueur_attendu) {
    // On convertit la matrice de duels statique simple a ecrire dans le code
    // en une matrice dynamique utilisable dans nos fonctions
    t_mat_int_dyn mat_duels;
    creer_mat_duels_compatible(duels, &mat_duels);
    // Initialisation de la BDD des candidats en utilisant les candidats
    // de test, construite statiquement
    t_candidats candidats = { candidats_tests, NB_CANDIDATS_TESTS };

    assert(condorcet_minimax(mat_duels, candidats) == vainqueur_attendu);

    detruire_t_mat_int_dyn(&mat_duels);
}


//
// condorcet_minimax
//

// Il y a un vainqueur de condorcet donc on le retourne
void condorcet_minimax_vainqueur_condorcet() {
    int duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS] = {
        {  0, 50, 24, 50 },
        { 50,  0, 62, 51 }, // Toujours >= 50 --> vainqueur de condorcet
        { 76, 38,  0, 99 },
        { 50, 49,  1,  0 }
    };

    tester_condorcet_minimax(duels, 1);
}

// Il n'y a pas de vainqueur de condorcet donc on utilise les minimax pour
// designer un vainqueur
void condorcet_minimax_vainqueur_minimax() {
    int duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS] = {
        {  0, 70, 24, 50 }, // Pire score : 24
        { 30,  0, 62, 51 }, // Pire score : 30
        { 76, 38,  0, 99 }, // Pire score : 38 --> vainqueur minimax
        { 50, 49,  1,  0 }  // Pire score : 1
    };

    tester_condorcet_minimax(duels, 2);
}

// Il n'y a pas de vainqueur de condorcet, et on a besoin d'utiliser les
// infos des candidats pour en departager deux qui ont le meme pire score
void condorcet_minimax_vainqueur_minimax_egalite() {
    int duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS] = {
        {  0, 70, 24, 50 }, // Pire score : 24
        { 30,  0, 72, 70 }, // Pire score : 30 -- Age : 66 ans (S)
        { 76, 28,  0, 55 }, // Pire score : 38
        { 50, 30, 45,  0 }  // Pire score : 30 -- Age : 70 ans (M)
    };

    tester_condorcet_minimax(duels, 3);
}


// Script principal executant tous les tests de ce module
void tests_unitaires_condorcet_minimax() {
    condorcet_minimax_vainqueur_condorcet();
    condorcet_minimax_vainqueur_minimax();
    condorcet_minimax_vainqueur_minimax_egalite();
}
