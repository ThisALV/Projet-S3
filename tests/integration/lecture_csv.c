#include <lecture_csv.h>

#include <assert.h>
#include <string.h>

// Nb de lignes attendu pour la matrice de test
#define NB_LIGNES_TEST 4
// Nb de colonnes attendu pour la matrice de test
#define NB_COLONNES_TEST 6


//
// lire_fichier_votes
//

void lire_fichier_votes_test() {
    // Mots que l'on s'attend a lire dans le fichier CSV
    char* contenu_attendu[NB_LIGNES_TEST][NB_COLONNES_TEST] = {
        { "A", "B", "C", "D", "E", "Fin" },
        { "0", "1", "2", "3", "4", "AAA" },
        { "5", "6", "7", "8", "9", "BBB" },
        { "10", "11", "12", "13", "14", "CCC" }
    };

    // On ouvre en lecture le fichier contenant nos valeurs CSV de test
    FILE* test_csv = fopen("votes/test.csv", "r");
    // Il faut s'assurer qu'il est bien ouvert, sinon le reste du test est faux
    assert(test_csv != NULL);

    t_mat_char_star_dyn mots;
    // On lit le fichier de test avec le separateur virgule
    lire_fichier_votes(test_csv, ",", &mots);

    // On verifie le nb de lignes et de colonnes lues dans le CSV
    assert(mots.lignes == NB_LIGNES_TEST);
    assert(mots.colonnes == NB_COLONNES_TEST);
    // On verifie qu'une matrice a bien ete allouee
    assert(mots.elems != NULL);

    for (int i = 0; i < NB_LIGNES_TEST; i++) {
        for (int j = 0; j < NB_COLONNES_TEST; j++) {
            // On teste le contenu de la matrice case par case
            assert(strcmp(mots.elems[i][j], contenu_attendu[i][j]) == 0);
        }
    }

    fclose(test_csv); // On ferme les ressources utilisees par le test
}


// Script des tests appele depuis main_integration.c
void tests_integration_lecture_csv() {
    lire_fichier_votes_test();
}
