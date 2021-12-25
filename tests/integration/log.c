#include <integration/log.h>

#include <log.h>
#include <assert.h>
#include <erreur.h>
#include <string.h>

// Dimension des SDD affichees dans les logs de test
#define DIM_SDD_TEST 5
// Taille maximum d'une ligne de logging dans le fichier de test
#define LIGNE_MAX 256

// Permet d'executer un test dans un nouveau fichier de logging
// automatiquement ouvert
// La fermeture sera geree par le test, qui appellera lire_lignes_logging
// lorsqu'il aura fini d'ecrire dedans.
#define AVEC_FICHIER_LOGGING(test_integration) \
    rediriger_logging_sur("logs/test.txt");\
    test_integration();


// Nom du module pour le logging
static char* module = "integration_log";


// Ferme le fichier de logging pour pouvoir lire ce qui a ete ecrit.
// Ensuite, ouvre en lecture le fichier de test, copie ses lignes dans le tableau
// de char* donne, puis ferme le ficher de test
static void lire_lignes_logging(int nb_lignes_attendues, char lignes[nb_lignes_attendues][LIGNE_MAX]) {
    fermer_fichier_logging();

    // Ouverture en lecture du fichier de logging
    FILE* sortie_logging = fopen("logs/test.txt", "r");

    // Erreur fatale si le logging de test n'a pas pu etre ouvert
    if (sortie_logging == NULL)
        erreur_fatale(2, "Ouverture logging de test");

    for (int ligne_i = 0; ligne_i < nb_lignes_attendues; ligne_i++) {
        char fin_de_ligne; // Utilise pour vider le \n du scan

        // Lecture de la ligne sans le \n, et verification de la lecture
        if (fscanf(sortie_logging, "%[^\n]%c", lignes[ligne_i], &fin_de_ligne) == EOF) {
            // Si on arrive pas a lire le fichier necessaire au test, alors
            // les tests doivent s'arreter
            fclose(sortie_logging);
            erreur_fatale(2, "Lecture logging de test");
        }
    }

    fclose(sortie_logging);
}


//
// sortie_logging_par_defaut
//

void sortie_logging_par_defaut_test() {
    sortie_logging_par_defaut();

    // On verifie que la sortie a bien ete mise a stdout, la valeur par defaut
    assert(sortie_logging_courante() == stdout);
}


//
// Tests n'ayant pas besoin d'un fichier de logging
//

void rediriger_logging_sur_creation_fichier_impossible() {
    // Le rep impossible n'existant pas, on s'attend a ce que l'operation
    // echoue
    assert(!rediriger_logging_sur("impossible/logs.txt"));

    // On doit donc garder la sortie par defaut
    assert(sortie_logging_courante() == stdout);
}

void fermer_fichier_logging_sortie_par_defaut() {
    fermer_fichier_logging(); // Aucun fichier ouvert
    assert(sortie_logging_courante() == stdout); // Toujours la sortie par defaut
}


//
// Tests utilisant le meme fichier de logging
//

void rediriger_logging_sur_ok() {
    // Le fichier existe ou peut etre cree, donc l'operation doit reussir
    assert(rediriger_logging_sur("logs/test.txt"));

    // La sortie est celle du fichier, et non plsu celle par defaut
    FILE* sortie = sortie_logging_courante();
    assert(sortie != stdout && sortie != NULL);
}

void fermer_fichier_logging_fichier_ouvert() {
    fermer_fichier_logging(); // Un fichier a ete ouvert par le test precedent
    assert(sortie_logging_courante() == stdout); // Il doit avoir ete ferme par ce test
}


//
// log_ligne
//

void log_ligne_message_simple() {
    // On ecrit les messages dans le logging
    log_ligne(module, "1 message");
    log_ligne(module, "2 messages");
    log_ligne(module, "3 messages");

    // On lit ce qu'il y a dans le logging
    char lignes[3][LIGNE_MAX];
    lire_lignes_logging(3, lignes);

    // On compare avec ce que l'on a voulu ecrire
    assert(strcmp("[integration_log] : 1 message", lignes[0]) == 0);
    assert(strcmp("[integration_log] : 2 messages", lignes[1]) == 0);
    assert(strcmp("[integration_log] : 3 messages", lignes[2]) == 0);
}

void log_ligne_message_avec_donnees() {
    // On ecrit les messages dans le logging
    char* message = "message(s)";
    for (int i = 0; i < 3; i++)
        log_ligne(module, "%d %s", i + 1, message);

    // On lit ce qu'il y a dans le logging
    char lignes[3][LIGNE_MAX];
    lire_lignes_logging(3, lignes);

    // On compare avec ce que l'on a voulu ecrire
    assert(strcmp("[integration_log] : 1 message(s)", lignes[0]) == 0);
    assert(strcmp("[integration_log] : 2 message(s)", lignes[1]) == 0);
    assert(strcmp("[integration_log] : 3 message(s)", lignes[2]) == 0);
}


//
// log_t_tab_int_dyn et log_t_mat_int_dyn
//

void log_t_tab_int_dyn_ok() {
    // Tableaux a afficher alloues statiquement
    int elems1[DIM_SDD_TEST] = { 5, 3, 4, 2, 1 };
    int elems2[DIM_SDD_TEST] = { 0, -1, -2, 3, 4 };
    t_tab_int_dyn tab1 = { elems1, DIM_SDD_TEST };
    t_tab_int_dyn tab2 = { elems2, DIM_SDD_TEST };

    // Ecriture des tableaux dans le logging
    log_t_tab_int_dyn(module, tab1);
    log_t_tab_int_dyn(module, tab2);

    // Verification des lignes ecrites dans la sortie du logging
    char lignes[2][LIGNE_MAX];
    lire_lignes_logging(2, lignes);
    assert(strcmp("[integration_log] :  | 5 | 3 | 4 | 2 | 1", lignes[0]) == 0);
    assert(strcmp("[integration_log] :  | 0 | -1 | -2 | 3 | 4", lignes[1]) == 0);
}

void log_t_mat_int_dyn_ok() {
    // Alloue statiquement les lignes de la matrice a afficher
    int lignes_mat[DIM_SDD_TEST][DIM_SDD_TEST] = {
        { 1, 2, 3, 4, 5 },
        { -1, -2, -3, -4, -5 },
        { 10, 0, 15, 25, 40 },
        { 33, -100, 2, 3, 4 },
        { 5, 4, 3, 2, 1 }
    };
    // Alloue statiquement le tableau de pointeurs vers chacune des lignes
    int* pointeurs_lignes[DIM_SDD_TEST];
    for (int ligne_i = 0; ligne_i < DIM_SDD_TEST; ligne_i++)
        pointeurs_lignes[ligne_i] = lignes_mat[ligne_i];
    
    // Utilise le tableau de pointeurs pour le convertir en int** et creer
    // la matrice
    t_mat_int_dyn mat = { pointeurs_lignes, DIM_SDD_TEST };

    log_t_mat_int_dyn(module, mat);

    // Verification de la sortie
    char lignes[DIM_SDD_TEST][LIGNE_MAX];
    lire_lignes_logging(DIM_SDD_TEST, lignes);
    assert(strcmp("[integration_log] :  | 1 | 2 | 3 | 4 | 5", lignes[0]) == 0);
    assert(strcmp("[integration_log] :  | -1 | -2 | -3 | -4 | -5", lignes[1]) == 0);
    assert(strcmp("[integration_log] :  | 10 | 0 | 15 | 25 | 40", lignes[2]) == 0);
    assert(strcmp("[integration_log] :  | 33 | -100 | 2 | 3 | 4", lignes[3]) == 0);
    assert(strcmp("[integration_log] :  | 5 | 4 | 3 | 2 | 1", lignes[4]) == 0);
}

void log_t_liste_simple_int_ok() {
    // Alloue dynamique une liste et la remplie de sorte qu'elle contienne
    // [5, 4, 3, 2, 1]
    t_liste_simple_int liste;
    creer_t_liste_simple_int(&liste);
    for (int i = 1; i <= 5; i++)
        inserer_debut_t_liste_simple_int(&liste, i);

    log_t_liste_simple_int(module, liste); // On ecrit la liste dans la sortie
    detruire_t_liste_simple_int(&liste); // On detruit la liste une fois ecrite

    // Verification de la sortie
    char lignes[1][LIGNE_MAX];
    lire_lignes_logging(1, lignes); // Une liste ecrite = 1 ligne
    assert(strcmp("[integration_log] :  | 5 | 4 | 3 | 2 | 1", lignes[0]) == 0);
}


// Script des tests du module
void tests_integration_log() {
    sortie_logging_par_defaut_test();

    // Ces tests n'ont pas besoin de fichier de logging pour s'executer
    rediriger_logging_sur_creation_fichier_impossible();
    fermer_fichier_logging_sortie_par_defaut();

    // Ces tests vont utiliser le meme fichier de logging
    rediriger_logging_sur_ok();
    fermer_fichier_logging_fichier_ouvert();

    // Ces tests vont chacun utiliser un nouveau fichier de logging
    AVEC_FICHIER_LOGGING(log_ligne_message_simple);
    AVEC_FICHIER_LOGGING(log_ligne_message_avec_donnees);
    AVEC_FICHIER_LOGGING(log_t_tab_int_dyn_ok);
    AVEC_FICHIER_LOGGING(log_t_mat_int_dyn_ok);
    AVEC_FICHIER_LOGGING(log_t_liste_simple_int_ok);
}
