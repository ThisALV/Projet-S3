#include <unitaires/lecture_csv.h>

#include <lecture_csv.h>
#include <utils_sd.h>
#include <assert.h>
#include <string.h>

// Dimension pour la matrice des duels pour les tests de
// convertir_mat_duels
#define DIM_DUELS_TEST 3
// Nombre de candidats pour les tests de obtenir_candidats_ballots et
// obtenir_candidats_duels
#define NB_CANDIDATS_TEST 3


// Fonction privee utilitaire qui verifie que la matrice de duels donnee
// est passee en mode erreur
void verifier_mat_erreur(t_mat_int_dyn duels) {
    assert(duels.elems == NULL);
    assert(duels.dim == -1);
}

// Fonction privee utilitaire qui verifie que le tableau des candidats donne
// est passe en mode erreur
void verifier_tab_erreur(t_candidats candidats) {
    assert(candidats.elems == NULL);
    assert(candidats.nb == -1);
}

// Fonction privee utilitaire qui transforme une matrice constante de la
// forme char*[N][]M en matrice de la t_mat_char_star_dyn
void convertir_mat_compatible(int lignes, int colonnes, char* src[lignes][colonnes], t_mat_char_star_dyn* dest) {
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
// convert_mat_duels
//

// Avec une mat de mots CSV qui a plus de lignes que de colonnes (sans
// compter l'en-tete)
void convertir_mat_duels_non_carree() {
    t_mat_char_star_dyn mots;
    t_mat_int_dyn duels;
    // Sans compter l'en-tete : 3 lignes et 2 cols -> non carree
    mots.lignes = 4;
    mots.colonnes = 2;

    convertir_mat_duels(mots, &duels);

    // On verifie que la mat des duels est en mode erreur
    verifier_mat_erreur(duels);
}

// Avec une mat de mots CSV depourvue de colonnes, donc ne contentant aucun
// duel
void convertir_mat_duels_vide() {
    t_mat_char_star_dyn mots;
    t_mat_int_dyn duels;
    // Sans compter l'en-tete : 3 lignes et 0 cols -> aucun duel
    mots.colonnes = 0;
    mots.lignes = 3;

    convertir_mat_duels(mots, &duels);

    // On verifie que la mat des duels est en mode erreur
    verifier_mat_erreur(duels);
}

// Avec une mat de mots CSV valide, contenant des score de duels entre les
// candidats presentes dans l-en-tete
void convertir_mat_duels_ok() {
    // Matrice de mots apres la lecture d'un fichier CSV
    char* contenu_csv[DIM_DUELS_TEST + 1][DIM_DUELS_TEST] = {
        { "A",  "B",  "C"  }, // En-tete
        { "0",  "55", "25" },
        { "45", "0",  "57" },
        { "75", "43", "0"  }
    };

    // Matrice des duels qu'on s'attend a obtenir apres la conversion
    int duels_attendu[DIM_DUELS_TEST][DIM_DUELS_TEST] = {
        { 0,  55, 25 },
        { 45, 0,  57 },
        { 75, 43, 0  }
    };

    t_mat_char_star_dyn mots;
    t_mat_int_dyn duels;
    // +1 ligne pour l'en-tete
    convertir_mat_compatible(DIM_DUELS_TEST + 1, DIM_DUELS_TEST,
        contenu_csv, &mots);

    convertir_mat_duels(mots, &duels);

    // On verifie la dimension de la matrice des duels obtenue
    assert(duels.dim == DIM_DUELS_TEST);
    // On verifie son conteu cellule par cellule
    for (int i = 0; i < DIM_DUELS_TEST; i++)
        for (int j = 0; j < DIM_DUELS_TEST; j++)
            assert(duels_attendu[i][j] == duels.elems[i][j]);

    // Liberation des ressources du test
    detruire_t_mat_int_dyn(&duels);
    detruire_t_mat_char_star_dyn(&mots);
}


//
// obtenir_candidats_duels
//

// Avec une matrice CSV vide, aucune en-tete pour lire les candidats
void obtenir_candidats_duels_csv_vide() {
    t_mat_char_star_dyn mots;
    t_candidats candidats;
    // On simule un fichier CSV completement vide
    mots.colonnes = 0;
    mots.lignes = 0;

    obtenir_candidats_duels(mots, &candidats);

    // On verifie que le tableau des candidats est en mode erreur
    verifier_tab_erreur(candidats);
}

// Avec un nombre de colonnes insuffisant, càd 0 colonne donc 0 candidat
void obtenir_candidats_duels_nb_colonnes_invalide() {
    t_mat_char_star_dyn mots;
    t_candidats candidats;
    // On simule un fichier CSV avec 3 \n sans rien d'autre
    mots.colonnes = 0;
    mots.lignes = 3;

    obtenir_candidats_duels(mots, &candidats);

    // On verifie que le tableau des candidats est en mode erreur
    verifier_tab_erreur(candidats);
}

// Avec une matrice CSV valide, càd au moins 5 colonnes et plusieurs lignes
void obtenir_candidats_duels_ok() {
    // Matrice de mots apres la lecture d'un fichier CSV
    char* contenu_csv[NB_CANDIDATS_TEST + 1][NB_CANDIDATS_TEST] = {
        { "A",  "B",  "C"  }, // En-tete
        { "0",  "55", "25" },
        { "45", "0",  "57" },
        { "75", "43", "0"  }
    };

    // Candidats attendus dans l'ordre dans l'en-tete CSV
    t_candidat candidats_attendus[NB_CANDIDATS_TEST] = {
        { "A", 0 },
        { "B", 1 },
        { "C", 2 }
    };

    t_mat_char_star_dyn mots;
    t_candidats candidats;
    // On obtient notre matrice de valeurs CSV exploitable par la fonciton
    convertir_mat_compatible(NB_CANDIDATS_TEST + 1, NB_CANDIDATS_TEST,
        contenu_csv, &mots);

    obtenir_candidats_duels(mots, &candidats);

    // On verifie le nombre de candidats (colonnes) lus
    assert(candidats.nb == NB_CANDIDATS_TEST);
    // On verifie que chaque candidat est, dans l'ordre, celui qu'on attend
    for (int i = 0; i < NB_CANDIDATS_TEST; i++) {
        t_candidat* attendu = candidats_attendus + i;
        t_candidat* obtenu = candidats.elems + i;

        assert(strcmp(attendu->nom, obtenu->nom) == 0);
        assert(attendu->id == obtenu->id);
    }

    // On libere les ressources allouees pour le test
    detruire_t_candidats_dyn(&candidats);
    detruire_t_mat_char_star_dyn(&mots);
}


//
// obtenir_candidats_ballots
//

// Avec une matrice CSV vide, aucune en-tete pour lire les candidats
void obtenir_candidats_ballots_csv_vide() {
    t_mat_char_star_dyn mots;
    t_candidats candidats;
    // On simule un fichier CSV completement vide
    mots.colonnes = 0;
    mots.lignes = 0;

    obtenir_candidats_ballots(mots, &candidats);

    // On verifie que le tableau des candidats est en mode erreur
    verifier_tab_erreur(candidats);
}

// Avec un nombre de colonnes insuffisant, càd 0 colonne donc 0 candidat
void obtenir_candidats_ballots_nb_colonnes_invalide() {
    t_mat_char_star_dyn mots;
    t_candidats candidats;
    // On simule un fichier CSV avec 4 colonnes par ligne, donc que des
    // colonnes prefixes et aucune colonne pour les candidats
    mots.colonnes = 4;
    mots.lignes = 3;

    obtenir_candidats_ballots(mots, &candidats);

    // On verifie que le tableau des candidats est en mode erreur
    verifier_tab_erreur(candidats);
}

// Avec une matrice CSV valide, càd au moins 5 colonnes et plusieurs lignes
void obtenir_candidats_ballots_ok() {
    // Matrice de mots apres la lecture d'un fichier CSV, avec 4 colonnes
    // prefixes puis NB_CANDIDATS_TEST colonnes de candidat
    char* contenu_csv[5][4 + NB_CANDIDATS_TEST] = {
        { "", "", "", "abcd", "A", "B", "C" }, // En-tete
        { "", "", "", "efgh", "1", "3", "2" },
        { "", "", "", "ijkl", "3", "2", "1" },
        { "", "", "", "mnop", "1", "2", "3" },
        { "", "", "", "qrst", "2", "1", "3" },
    };
    

    // Candidats attendus dans l'ordre dans l'en-tete CSV
    t_candidat candidats_attendus[NB_CANDIDATS_TEST] = {
        { "A", 0 },
        { "B", 1 },
        { "C", 2 }
    };

    t_mat_char_star_dyn mots;
    t_candidats candidats;
    // On obtient une matrice CSV interpretable pour notre fonction
    convertir_mat_compatible(5, 4 + NB_CANDIDATS_TEST, contenu_csv, &mots);

    obtenir_candidats_ballots(mots, &candidats);

    // On verifie le nombre de candidats (colonnes) lus
    assert(candidats.nb == NB_CANDIDATS_TEST);
    // On verifie que chaque candidat est, dans l'ordre, celui qu'on attend
    for (int i = 0; i < NB_CANDIDATS_TEST; i++) {
        t_candidat* attendu = candidats_attendus + i;
        t_candidat* obtenu = candidats.elems + i;

        assert(strcmp(attendu->nom, obtenu->nom) == 0);
        assert(attendu->id == obtenu->id);
    }

    // On libere les ressources allouees pour le test
    detruire_t_candidats_dyn(&candidats);
    detruire_t_mat_char_star_dyn(&mots);
}


//
// creer_mat_duels_absolue
//

// Fonction privee utilitaire pour tester creer_mat_duels_absolue, en
// lui fournissant une matrice de duels attendu et un fichier CSV lu en
// memoire
static void tester_creer_mat_duels_absolue(
    char* contenu_csv[5][4 + NB_CANDIDATS_TEST],
    int duels_attendu[NB_CANDIDATS_TEST][NB_CANDIDATS_TEST])
{
    t_mat_char_star_dyn mots;
    convertir_mat_compatible(5, 4 + NB_CANDIDATS_TEST, contenu_csv, &mots);

    // La fonction ne s'occupe pas de l'initialisation de la matrice
    t_mat_int_dyn duels;
    creer_t_mat_int_dyn(&duels, NB_CANDIDATS_TEST);

    int nb_electeurs;
    creer_mat_duels_absolue(mots, &duels, &nb_electeurs);

    // On verifie qu'on a bien obtenu la matrice intermediaire attendu
    for (int i = 0; i < NB_CANDIDATS_TEST; i++)
        for (int j = 0; j < NB_CANDIDATS_TEST; j++)
            assert(duels_attendu[i][j] == duels.elems[i][j]);

    // Liberation des ressources du test
    detruire_t_mat_char_star_dyn(&mots);
    detruire_t_mat_int_dyn(&duels);
}

// Avec des valeurs CSV qui ne sont pas interpretables comme des entiers
// > 0 dans certains scores dans les ballots de vote
void creer_mat_duels_absolue_erreurs_ballots() {
    // On s'attend a obtenir cette matrice de duels intermediaire
    int duels_attendu[NB_CANDIDATS_TEST][NB_CANDIDATS_TEST] = {
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 2, 2, 0 }
    };

    // On simule avoir lu ce fichier CSV
    char* contenu_csv[5][4 + NB_CANDIDATS_TEST] = {
        { "", "", "", "abcd", "A", "B", "C"  }, // En-tete
        { "", "", "", "efgh", "1", "",  "2"  }, // Valeur CSV vide : atoi erreur
        { "", "", "", "ijkl", "3", "2", "-1" }, // Score negatif : erreur
        { "", "", "", "mnop", "1", "2", "3"  },
        { "", "", "", "qrst", "a", "1", "3" }, // Pas un entier : atoi erreur
    };
    
    tester_creer_mat_duels_absolue(contenu_csv, duels_attendu);
}

// Avec une des ballots CSV ne comportant aucun score illisible
void creer_mat_duels_absolue_csv_bon() {
    // On s'attend a obtenir cette matrice de duels intermediaire
    int duels_attendu[NB_CANDIDATS_TEST][NB_CANDIDATS_TEST] = {
        { 0, 0, 0 },
        { 2, 0, 0 },
        { 3, 2, 0 }
    };

    // On simule avoir lu ce fichier CSV
    char* contenu_csv[5][4 + NB_CANDIDATS_TEST] = {
        { "", "", "", "abcd", "A", "B", "C" },
        { "", "", "", "efgh", "1", "3", "2" },
        { "", "", "", "ijkl", "3", "2", "1" },
        { "", "", "", "mnop", "1", "2", "3" },
        { "", "", "", "qrst", "2", "1", "3" },
    };
    
    tester_creer_mat_duels_absolue(contenu_csv, duels_attendu);
}


//
// completer_mat_duels
//

void completer_mat_duels_test() {
    // C'est la matrice de duels intermediaire fournie a la fonction
    int duels_absolus[NB_CANDIDATS_TEST][NB_CANDIDATS_TEST] = {
        { 0, 0, 0 },
        { 2, 0, 0 },
        { 3, 2, 0 }
    };

    // C'est la matrice de duels avec les pourcentages attendus
    int duels_attendus[NB_CANDIDATS_TEST][NB_CANDIDATS_TEST] = {
        {  0, 50, 25 }, // Tout est passe en % pour 4 electeurs
        { 50,  0, 50 },
        { 75, 50,  0 }
    };

    t_mat_int_dyn duels; // La fonction n'initialise pas la matrice
    creer_t_mat_int_dyn(&duels, NB_CANDIDATS_TEST);
    // Maintenant on initialise son contenu d'entree...
    for (int i = 0; i < NB_CANDIDATS_TEST; i++)
        for (int j = 0; j < NB_CANDIDATS_TEST; j++)
            duels.elems[i][j] = duels_absolus[i][j];

    completer_mat_duels(&duels, 4); // 4 electeurs

    // ...puis on verifie son contenu de sortie
    for (int i = 0; i < NB_CANDIDATS_TEST; i++)
        for (int j = 0; j < NB_CANDIDATS_TEST; j++)
            assert(duels.elems[i][j] == duels_attendus[i][j]);

    detruire_t_mat_int_dyn(&duels);
}


//
// creer_mat_duels
//

// Fonction privee pour tester si, avec le nb de lignes et de colonnes CSV
// donnees, les dimensions sont invalides et donc si la matrice de sortie
// est bien en mode erreur
static void tester_creer_mat_duels_dims_invalides(int colonnes, int lignes) {
    t_mat_char_star_dyn mots;
    // On simule une matrice vide
    mots.colonnes = colonnes;
    mots.lignes = lignes;

    t_mat_int_dyn duels;
    creer_mat_duels(mots, &duels);

    verifier_mat_erreur(duels);
}

// Avec un fichier CSV completement vide
void creer_mat_duels_csv_vide() {
    tester_creer_mat_duels_dims_invalides(0, 0);
}

// Avec un fichier CSV ne contennant qu'une en-tete
void creer_mat_duels_aucun_electeur() {
    tester_creer_mat_duels_dims_invalides(4 + NB_CANDIDATS_TEST, 1);
}

// Avec un nombre de colonnes insuffisant, donc 0 candidat
void creer_mat_duels_nb_colonnes_invalide() {
    tester_creer_mat_duels_dims_invalides(4, 11);
}

// Avec une matrice CSV contenant des ballots CSV valides, avec certains
// scores illisbles et des egalites
void creer_mat_duels_ok() {
    // On s'attend a se resultat pour la matrice de duels
    int duels_attendu[NB_CANDIDATS_TEST][NB_CANDIDATS_TEST] = {
        {  0, 75, 75 },
        { 25,  0, 50 },
        { 25, 50,  0 }
    };

    // On simule avoir lu ce fichier CSV
    char* contenu_csv[5][4 + NB_CANDIDATS_TEST] = {
        { "", "", "", "abcd", "A", "B", "C" }, // En-tete
        { "", "", "", "efgh", "1", "",  "2" }, // Valeur CSV vide : atoi erreur
        { "", "", "", "ijkl", "1", "2", "1" },
        { "", "", "", "mnop", "3", "2", "3" },
        { "", "", "", "qrst", "a", "1", "3" }, // Pas un entier : atoi erreur
    };
    t_mat_char_star_dyn mots;
    convertir_mat_compatible(5, 4 + NB_CANDIDATS_TEST, contenu_csv, &mots);

    t_mat_int_dyn duels;
    creer_mat_duels(mots, &duels);

    // On verifie que notre matrice carree contient tous les candidats
    assert(duels.dim == NB_CANDIDATS_TEST);
    // Puis on verifie les resultats en % des duels
    for (int i = 0; i < NB_CANDIDATS_TEST; i++)
        for (int j = 0; j < NB_CANDIDATS_TEST; j++)
            assert(duels.elems[i][j] == duels_attendu[i][j]);

    detruire_t_mat_char_star_dyn(&mots);
    detruire_t_mat_int_dyn(&duels);
}


// Script des tests appele depuis main_unitaires.c
void tests_unitaires_lecture_csv() {
    convertir_mat_duels_non_carree();
    convertir_mat_duels_vide();
    convertir_mat_duels_ok();

    obtenir_candidats_duels_csv_vide();
    obtenir_candidats_duels_nb_colonnes_invalide();
    obtenir_candidats_duels_ok();

    obtenir_candidats_ballots_csv_vide();
    obtenir_candidats_ballots_nb_colonnes_invalide();
    obtenir_candidats_ballots_ok();

    creer_mat_duels_absolue_erreurs_ballots();
    creer_mat_duels_absolue_csv_bon();

    completer_mat_duels_test();

    creer_mat_duels_csv_vide();
    creer_mat_duels_aucun_electeur();
    creer_mat_duels_nb_colonnes_invalide();
    creer_mat_duels_ok();
}
