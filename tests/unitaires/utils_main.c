#include <unitaires/utils_main.h>

#include <utils_main.h>
#include <assert.h>
#include <string.h>


//
// traiter_parametres
//

// Verifie que les arguments donnes a interpreter au module produisent bien les
// options/valeurs d'options attendus
// Note : pour le logging, si l'option attendu vaut NULL, alors on verifiera que
// l'option obtenue vaut NULL aussi, sinon on verifiera l'egalite lexicale
// avec strcmp()
static void tester_traiter_parametres(
    int argc, char** argv, int format_attendu, char* entree_attendue,
    char* logging_attendu, int methode_attendue)
{
    t_parametres options_obtenues;
    traiter_parametres(argc, argv, &options_obtenues);

    assert(options_obtenues.format_entree == format_attendu);
    assert(strcmp(options_obtenues.option_entree, entree_attendue) == 0);
    assert(options_obtenues.option_methode == methode_attendue);

    if (logging_attendu == NULL)
        assert(options_obtenues.option_logging == NULL);
    else
        assert(strcmp(options_obtenues.option_logging, logging_attendu) == 0);
}

void traiter_parametres_ballots_aucune_option() {
    // -i pour une liste de ballots, stockee dans ballots.csv
    // toutes les autres options seront laissees par defaut
    char* argv[] = { "./scrutin", "-i", "ballots.csv" };

    tester_traiter_parametres(3, argv,
        ENTREE_LISTE_BALLOTS, "ballots.csv", NULL, METHODE_TOUTES);
}

void traiter_parametres_ballots_methode_scrutin() {
    // -i pour une liste de ballots, stockee dans liste.csv
    // -m pour la methode de scrutin, ici uninominale en 2 tours
    // Logging par defaut sur stdout : chemin fichier == NULL
    char* argv[] = { "./scrutin", "-i", "liste.csv", "-m", "uni2" };

    tester_traiter_parametres(5, argv,
        ENTREE_LISTE_BALLOTS, "liste.csv", NULL, METHODE_UNINOMINALE_DEUX_TOURS);

}

void traiter_parametres_ballots_fichier_logging() {
    // -i pour une liste de ballots, stockee dans liste.csv
    // -o pour le fichier de logging se nommant logs.log
    // Methode de scrutin par defaut, toutes les methodes seront executees
    char* argv[] = { "./scrutin", "-i", "liste.csv", "-o", "logs.log" };

    tester_traiter_parametres(5, argv,
        ENTREE_LISTE_BALLOTS, "liste.csv", "logs.log", METHODE_TOUTES);
}

void traiter_parametres_duels() {
    // -d pour une matrice de duels, stockee dans duels.csv
    // Logging par defaut sur stdout : chemin fichier == NULL
    // Methode de scrutin par defaut, toutes les methodes seront executees
    char* argv[] = { "./scrutin", "-d", "duels.csv" };

    tester_traiter_parametres(3, argv,
        ENTREE_MATRICE_DUELS, "duels.csv", NULL, METHODE_TOUTES);
}

void traiter_parametres_duels_fichier_et_methode() {
    // -i pour une liste de ballots, stockee dans liste.csv
    // -o pour le fichier de logging se nommant logs.log
    // -m pour la methode de scrutin, ici uninominale en 2 tours
    char* argv[] = { "./scrutin", "-i", "liste.csv", "-o", "logs.log", "-m", "cm" };

    tester_traiter_parametres(7, argv,
        ENTREE_LISTE_BALLOTS, "liste.csv", "logs.log", METHODE_CONDORCET_MINIMAX);
}


// Script principal des tests unitaires de ce module
void tests_unitaires_utils_main() {
    traiter_parametres_ballots_aucune_option();
    traiter_parametres_ballots_methode_scrutin();
    traiter_parametres_ballots_fichier_logging();
    traiter_parametres_duels();
    traiter_parametres_duels_fichier_et_methode();
}
