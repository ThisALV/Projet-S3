#include <integration/utils_verifier_mon_vote.h>

#include <utils_verifier_mon_vote.h>
#include <assert.h>
#include <erreur.h>
#include <string.h>

#define TAILLE_MAX_LIGNE_CSV 50
#define NB_LIGNES_CSV_TEST 3


// Fonction privee utilitaire qui transforme une matrice constante de la
// forme char*[N][]M en matrice de la t_mat_char_star_dyn
static void convertir_mat_compatible(int lignes, int colonnes, char* src[lignes][colonnes], t_mat_char_star_dyn* dest) {
    // On creee une matrice aux dimensions similaires en s'assurant qu'elle
    // a bien ete initialisee avant de continuer le test
    assert(creer_t_mat_char_star_dyn(dest, lignes, colonnes));

    // Puis on copie les char* de la matrice source vers la matric destination,
    // case par case
    for (int l = 0; l < lignes; l++)
        for (int c = 0; c < colonnes; c++)
            dest->elems[l][c] = src[l][c];
}


//
// ecrire_fichier_votes
//

void ecriture_fichier_votes_test() {
    // Matrice des mots qui seront ecrits dans le fichier CSV
    char* mots_elems[3][4] = {
        { "abcd", "efgh", "ijkl", "mnop" },
        { "0123", "aaa",  "bbb",  "ccc"  },
        { "4567", "ddd",  "eee",  "fff"  }
    };
    // Matrice allouee dynamiquement avec le type prevu a cet effet qu'on
    // passe en parametre a la fonction
    t_mat_char_star_dyn mots;
    convertir_mat_compatible(3, 4, mots_elems, &mots);

    // Ouverture du fichier en ecriture/lecture/creation
    FILE* fichier_csv = fopen("votes/test_ecriture.csv", "w+");
    // On s'assure que le fichier de test est bien ouvert
    if (fichier_csv == NULL)
        erreur_fatale(2, "Ouverture fichier CSV de test");

    // Appel a la fonction donc ecriture dans le fichier CSV, mots separes par
    // une virgule
    ecrire_fichier_votes(fichier_csv, ',', mots);

    // Retour au debut du fichier pour lire ce qui a ete ecrit dessus
    rewind(fichier_csv);

    // 3 ligne a lire attendues dans le fichier,
    char* lignes_attendues[NB_LIGNES_CSV_TEST] = {
        "abcd,efgh,ijkl,mnop",
        "0123,aaa,bbb,ccc",
        "4567,ddd,eee,fff"
    };
    
    // On lit chaque ligne du fichier jusqu'a la fin
    int nb_lignes_lues = 0; // en gardant une trace de nb de lignes deja lues
    char ligne_courante[TAILLE_MAX_LIGNE_CSV];
    char fin_de_ligne; // Utilise pour vider le \n du scan
    while (fscanf(fichier_csv, "%[^\n]%c", ligne_courante, &fin_de_ligne) != EOF) {
        // On verifie qu'on a pas deja lu toutes les lignes attendus, sinon cette ligne
        // est en trop
        assert(nb_lignes_lues < NB_LIGNES_CSV_TEST);

        // Index de la ligne courante, et on compabilise une nouvelle ligne lue
        int ligne_courante_i = nb_lignes_lues++;
        // On s'assure que la ligne lue est celle attendue par le test
        assert(strcmp(ligne_courante, lignes_attendues[ligne_courante_i]) == 0);
    }

    // On verifie que le fichier n'a pas moins de lignes que le nb attendu
    assert(nb_lignes_lues == NB_LIGNES_CSV_TEST);

    // Liberation des ressources utilisees par le test
    fclose(fichier_csv);
    detruire_t_mat_char_star_dyn(&mots);
}


// Script principale pour les tests

void tests_integration_utils_verifier_mon_vote() {
    ecriture_fichier_votes_test();
}
