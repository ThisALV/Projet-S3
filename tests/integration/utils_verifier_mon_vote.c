#include <integration/utils_verifier_mon_vote.h>

#include <utils_verifier_mon_vote.h>
#include <assert.h>
#include <erreur.h>
#include <string.h>
#include <lecture_csv.h>

#define LIGNE_MAX 200
#define NB_LIGNES_CSV_TEST 3
#define NB_ELECTEURS_TEST (NB_LIGNES_CSV_TEST - 1)


// Lit dans le fichier ouvert en lecture donne et verifie le nb et le contenu des lignes
static void verifier_contenu_fichier(FILE* fichier,
    int nb_lignes_attendues, char* lignes_attendues[nb_lignes_attendues])
{
    // On lit chaque ligne du fichier jusqu'a la fin
    int nb_lignes_lues = 0; // en gardant une trace de nb de lignes deja lues
    char ligne_courante[LIGNE_MAX];
    char fin_de_ligne; // Utilise pour vider le \n du scan
    while (fscanf(fichier, "%[^\n]%c", ligne_courante, &fin_de_ligne) != EOF) {
        // On verifie qu'on a pas deja lu toutes les lignes attendus, sinon cette ligne
        // est en trop
        assert(nb_lignes_lues < nb_lignes_attendues);

        // Index de la ligne courante, et on compabilise une nouvelle ligne lue
        int ligne_courante_i = nb_lignes_lues++;
        // On s'assure que la ligne lue est celle attendue par le test
        assert(strcmp(ligne_courante, lignes_attendues[ligne_courante_i]) == 0);
    }

    // On verifie que le fichier n'a pas moins de lignes que le nb attendu
    assert(nb_lignes_lues == nb_lignes_attendues);
}

// Fonction privee utilitaire qui transforme une matrice constante de la
// forme char*[N][]M en matrice de la t_mat_char_star_dyn
static void convertir_mat_compatible(int lignes, int colonnes, char* src[lignes][colonnes], t_mat_char_star_dyn* dest) {
    // On creee une matrice aux dimensions similaires en s'assurant qu'elle
    // a bien ete initialisee avant de continuer le test
    assert(creer_t_mat_char_star_dyn(dest, lignes, colonnes));

    // Puis on copie les char* de la matrice source vers la matric destination,
    // case par case, on allouant dynamiquement une nouvelle chaine puisque
    // c'est requis par les fonctions que nous allons tester
    for (int l = 0; l < lignes; l++)
        for (int c = 0; c < colonnes; c++)
            dest->elems[l][c] = allouer_copie_char_star(src[l][c], "Copie mat compatible");
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
    // On verifie que ces lignes sont bien celles ecrtires dans le CSV
    verifier_contenu_fichier(fichier_csv, NB_LIGNES_CSV_TEST, lignes_attendues);

    // Liberation des ressources utilisees par le test
    fclose(fichier_csv);
    detruire_t_mat_char_star_dyn(&mots);
}


//
// chiffrer_ballots_votes
//

void chiffrer_ballots_votes_nb_colonnes_invalide() {
    // Cette matrice n'a pas assez de colonnes pour contenir les noms des electeurs
    char* mots_elems[NB_LIGNES_CSV_TEST][3] = {
        { "", "", "" }, // On se fiche du contenu des colonnes, c'est leur nb qui
        { "", "", "" }, // va declencher l'erreur
        { "", "", "" }
    };
    t_mat_char_star_dyn mots;
    convertir_mat_compatible(NB_LIGNES_CSV_TEST, 3, mots_elems, &mots);

    // On s'assure que l'operation echoue, on lui donnant pour sortie stdout
    // et aucune cle privee puisque de toutes manieres rien ne va etre
    assert(!chiffrer_ballots_votes(mots, NULL, stdout));
}

void chiffrer_ballots_votes_ok() {
    // Cette matrice contient assez de colonnes pour avoir des noms a chiffrer
    char* mots_elems[NB_LIGNES_CSV_TEST][4] = {
        { "", "", "", "" }, // En-tete, le contenu n'a aucun importance
        { "", "", "", "abcd" }, // Seul le contenu de la colonne du nom nous interesse
        { "", "", "", "efgh" }
    };
    t_mat_char_star_dyn mots;
    convertir_mat_compatible(NB_LIGNES_CSV_TEST, 4, mots_elems, &mots);

    // On creer/tronque un fichier pour y stocker les noms et les cles, on
    // verifiera plus tard ce que la fonction a ecrit dedans
    FILE* fichier_cles = fopen("votes/cles_test.csv", "w+");
    // Chaque cle de 80 caracteres, donc 2 cles puisque 2 ballots => 2 electeurs
    char* cles_prives[NB_ELECTEURS_TEST] = {
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
    };

    // On s'assure que l'operation a reussi
    assert(chiffrer_ballots_votes(mots, cles_prives, fichier_cles));

    // On verifie que notre fichier de sortie contient bien les 2 associations
    // nom_electeur:cle_privee
    rewind(fichier_cles); // On revient au debut du fichier pour le lire
    char* lignes_attendues[NB_ELECTEURS_TEST] = {
        "abcd:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "efgh:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
    };
    // Verification du contenu
    verifier_contenu_fichier(fichier_cles, NB_ELECTEURS_TEST, lignes_attendues);

    fclose(fichier_cles); // On a fini de verifier l'utilisation du fichier

    //
    // Enfin, on verifie le contenu de la matrice apres chiffrement
    //

    // Les dimensions n'ont pas du etre changees
    assert(mots.lignes == NB_LIGNES_CSV_TEST);
    assert(mots.colonnes == 4);

    char* mots_attendus[NB_LIGNES_CSV_TEST][4] = {
        { "", "", "", "" }, // En-tete, non modifiee
        { "", "", "", "bc8f0aa116ac590450cdd321ea26bb4c8cbc4c23d5419f09311635a676e5b33e" },
        { "", "", "", "fa567d0e1780a4abe479325ed65bd92819deb26a586c8011caad9d27241cf9aa" }
    };
    // On teste ligne par ligne, colonne par colonne, si le mot CSV est correct
    for (int ligne_i = 0; ligne_i < NB_LIGNES_CSV_TEST; ligne_i++) {
        for (int colonne_i = 0; colonne_i < 4; colonne_i++) {
            char* mot_obtenu = mots.elems[ligne_i][colonne_i];
            char* mot_attendu = mots_attendus[ligne_i][colonne_i];

            assert(strcmp(mot_obtenu, mot_attendu) == 0);
        }
    }

    detruire_t_mat_char_star_dyn(&mots);
}


// Script principale pour les tests

void tests_integration_utils_verifier_mon_vote() {
    ecriture_fichier_votes_test();

    chiffrer_ballots_votes_nb_colonnes_invalide();
    chiffrer_ballots_votes_ok();
}
