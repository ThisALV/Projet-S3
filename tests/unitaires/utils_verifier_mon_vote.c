#include <unitaires/utils_verifier_mon_vote.h>

#include <utils_verifier_mon_vote.h>
#include <assert.h>
#include <string.h>

#define NB_LIGNES_CSV_TEST 3
#define NB_ELECTEURS_TEST (NB_LIGNES_CSV_TEST - 1)


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
// hash_electeur
//

// Teste si pour le nom d'electeur et la cle privee donnee, on obtient bien le
// hash d'electeur passe en parametre
static void tester_hash_electeur(char* nom_electeur, char* cle_privee, char* hash_attendu) {
    char hash_obtenu[TAILLE_HASH_ELECTEUR + 1];
    hash_electeur(nom_electeur, cle_privee, hash_obtenu);

    assert(strcmp(hash_obtenu, hash_attendu) == 0);
}
 
void hash_electeur_nom_vide() {
    char* nom_electeur = "";
    char* cle_privee =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char* hash_attendu =
        "0f45e858fbc4176cdf4e411f88281edefc390ae5afe7df0f44cd9297f0a64580";

    tester_hash_electeur(nom_electeur, cle_privee, hash_attendu);
}

void hash_electeur_nom_electeur() {
    char* nom_electeur = "MACHIN Truc";
    char* cle_privee =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char* hash_attendu =
        "86244289c722529343bb01182622f1ae45ef1bf039831f070a8aba2de7411fc3";

    tester_hash_electeur(nom_electeur, cle_privee, hash_attendu);
}


//
// chercher_electeur
//

// Matrice de mots CSV predefinie utilise dans les tests de chercher_electeur
// ou la matrice est bien formee.
// 3 candidats : ab, cd et ef. 2 electeurs.
static char* mots_elems[NB_LIGNES_CSV_TEST][7] = {
    { "", "", "", "", "ab", "cd", "ef" },
    // Hash quelconque
    { "", "", "", "0000000000000000000000000000000000000000000000000000000000000000", "2", "0", "1" },
    // Hash de MACHIN Truc avec la cle "aaa..." (64 fois 'a'),
    // les scores invalides doivent aussi etre affiches a l'utilisateur
    { "", "", "", "86244289c722529343bb01182622f1ae45ef1bf039831f070a8aba2de7411fc3", "-5", "0", "2" }
};

// A partir d'une matrice de mots CSV predefinie, essaie de trouver un electeur
// avec le nom et la cle privee donnes dans les ballots de vote.
// Ensuite, compare le resultat de l'operation avec le resultat attendu,
// en n'oubliant pas de liberer les ressources.
// Si scores_attendus est NULL, on s'attend a ne pas trouver le candidat, sinon
// on compare ce tableau avec celui en sortie de la fonction pour verifier le resultat.
static void tester_chercher_electeur_nb_colonnes_valide(char* nom, char* cle, t_tab_char_star_dyn* scores_attendus) {
    t_mat_char_star_dyn mots;
    convertir_mat_compatible(NB_LIGNES_CSV_TEST, 7, mots_elems, &mots);

    // Parametres de sortie
    t_tab_char_star_dyn noms_candidats;
    t_tab_char_star_dyn scores_candidats;

    // Le fichier CSV est bien forme puisque le nb de colonnes est suffisant,
    // l'operation devrai se derouler normalement
    assert(chercher_electeur(mots, nom, cle, &noms_candidats, &scores_candidats));

    // On a 3 candidats : ab, cd et ef
    assert(noms_candidats.taille == 3);
    assert(strcmp(noms_candidats.elems[0], "ab") == 0);
    assert(strcmp(noms_candidats.elems[1], "cd") == 0);
    assert(strcmp(noms_candidats.elems[2], "ef") == 0);

    // Est-ce qu'on s'attend a avoir trouve le hash de l'electeur ?
    if (scores_attendus == NULL) { // Non, mode erreur pour le tableau en sortie
        assert(est_t_tab_char_star_dyn_erreur(scores_candidats));
    } else { // Oui, on verifie les resultats obtenus
        // Autant de score que ce que l'on s'attendait a avoir (le nb de candidats)
        assert(scores_candidats.taille == scores_attendus->taille);

        // On verifie chaque score brut du CSV
        for (int score_i = 0; score_i < scores_candidats.taille; score_i++) {
            char* score_attendu = scores_attendus->elems[score_i];
            char* score_obtenu = scores_candidats.elems[score_i];

            assert(strcmp(score_obtenu, score_attendu) == 0);
        }
    }

    // Destruction des ressources alloues dynamiquement par la fonction
    detruire_t_tab_char_star_dyn(&noms_candidats);
    detruire_t_tab_char_star_dyn(&scores_candidats);
}

void chercher_electeur_nb_colonnes_invalide() {
    // Matrice de mots qui ne contient pas assez de colonnes, le contenu
    // n'importe pas ce sont les dimension indiquees qui declenchent
    // l'erreur (il faudra au moins 5 cols donc 1 de plus)
    t_mat_char_star_dyn mots = { NULL, NB_LIGNES_CSV_TEST, 4 };

    // On s'assure que la matrice de mots etait mal formee
    assert(!chercher_electeur(mots, NULL, NULL, NULL, NULL));
}

void chercher_electeur_electeur_non_trouve() {
    // Nom d'electeur qui a participe avec une cle invalide
    char* nom_electeur = "MACHIN Truc";
    char* cle = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

    // On s'attend donc a ne pas le trouver, donc scores_attendus est NULL
    tester_chercher_electeur_nb_colonnes_valide(nom_electeur, cle, NULL);
}

void chercher_electeur_electeur_trouve() {
    // Nom d'electeur qui a participe avec une cle valide
    char* nom_electeur = "MACHIN Truc";
    char* cle = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    
    // On s'attend donc a trouver les scores suivants selon la matrice predefinie
    char* scores_attendus_elems[3] = { "-5", "0", "2" };
    // 3 candidats, 1 score pour chaque attendu
    t_tab_char_star_dyn scores_attendus = { scores_attendus_elems, 3 };
    tester_chercher_electeur_nb_colonnes_valide(nom_electeur, cle, &scores_attendus);
}


// Script principale pour les tests

void tests_unitaires_utils_verifier_mon_vote() {
    hash_electeur_nom_vide();
    hash_electeur_nom_electeur();

    chercher_electeur_nb_colonnes_invalide();
    chercher_electeur_electeur_non_trouve();
    chercher_electeur_electeur_trouve();
}
