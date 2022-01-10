#include <erreur.h>
#include <log.h>
#include <utils_main.h>
#include <lecture_csv.h>
#include <uninominales.h>
#include <condorcet_minimax.h>
#include <condorcet_schulze.h>
#include <stdlib.h>


// Verifie qu'un tableau des tetes de listes a bien ete construit et sinon,
// affiche un message expliquant l'erreur qui s'est produite
void verifier_tetes_de_listes(t_tab_int_dyn* vainqueurs_ballots) {
    if (vainqueurs_ballots == NULL)
        erreur_fatale(2, "Il doit y avoir au moins 1 candidat et 1 electeur dans"
            " la liste des ballots");
}

// Verifei qu'une matrice des duels a bien ete construite et sinon,
// affiche un message expliquant l'erreur qui s'est produite
void verifier_matrice_des_duels(t_mat_int_dyn mat_duels) {
    if (est_t_mat_int_dyn_erreur(mat_duels))
        erreur_fatale(2, "Il doit y avori au moins 1 candidat et 1 electeur pour"
            " construire une matrice des duels");
}


// Detruit proprement un tableau dynamique de t_tab_int_dyn, en detruisant chaque
// element avec detruire_t_tab_int_dyn() puis en appelant free() sur le tableau
void detruire_vainqueurs_ballots(t_tab_int_dyn* vainqueurs_ballots, int nb_ballots) {
    for (int ballot_i = 0; ballot_i < nb_ballots; ballot_i++)
            detruire_t_tab_int_dyn(&(vainqueurs_ballots[ballot_i]));
    
    free(vainqueurs_ballots);
}

// Affiche les resultats et details du scrutin sous format normalise.
// nb_votants et score_final sont optionnels, s'ils valent 0 ils ne seront
// pas affiches.
void afficher_scrutin(char* nom_methode, t_candidats candidats,
    int nb_votants, int vainqueur_id, int score_final)
{
    printf("Mode de scrutin : %s, %d candidats", nom_methode, candidats.nb);

    if (nb_votants != 0)
        printf(", %d votants", nb_votants);

    printf(", vainqueur = %s", candidats.elems[vainqueur_id].nom);

    if (score_final != 0)
        printf(", score = %d%%", score_final);

    putchar('\n');
}


// Execute toutes les methodes de scrutin sur la matrice CSV donnee
void executer_tous_scrutins(t_mat_char_star_dyn mots_csv, t_candidats bdd_candidats) {
    // On aura besoin des 2 structures de donnees, puisque toutes les methodes
    // seront executees
    t_mat_int_dyn mat_duels;
    t_tab_int_dyn* vainqueurs_ballots;
    int nb_ballots = mots_csv.lignes - 1;

    // On construit ces 2 SDD depuis la liste de ballots CSV
    creer_mat_duels(mots_csv, &mat_duels);
    vainqueurs_ballots = tetes_de_listes(mots_csv);
    
    // On verifie qu'elles ont bien ete construites, que le fichier CSV n'etait
    // pas malforme
    verifier_matrice_des_duels(mat_duels);
    verifier_tetes_de_listes(vainqueurs_ballots);

    int vainqueur_id;
    // On execute toutes les methodes de scrutin, sachant qu'on connait le nb
    // de votants puisqu'on a forcement une liste de ballots pour les methodes
    // uninominales

    vainqueur_id = uninominale_un_tour(bdd_candidats, vainqueurs_ballots, nb_ballots);
    afficher_scrutin("uninomianle a un tour", bdd_candidats, nb_ballots, vainqueur_id, 0);

    vainqueur_id = condorcet_minimax(mat_duels, bdd_candidats);
    afficher_scrutin("Condorcet minimax", bdd_candidats, nb_ballots, vainqueur_id, 0);

    vainqueur_id = condorcet_schulze(mat_duels, bdd_candidats);
    afficher_scrutin("Condorcet Schulze", bdd_candidats, nb_ballots, vainqueur_id, 0);

    // On n'oublie pas de liberer les ressources des sdd utilisees par les methodes
    detruire_t_mat_int_dyn(&mat_duels);
    detruire_vainqueurs_ballots(vainqueurs_ballots, nb_ballots);
}

// Execute une des methodes uninominales
void executer_scrutin_uninominale(t_mat_char_star_dyn mots_csv, t_candidats bdd_candidats, int methode_scrutin) {
    // Les methodes uninominales ont besoin des premiers de chaque ballots
    t_tab_int_dyn* vainqueurs_ballots = tetes_de_listes(mots_csv);
    // On verifie que le fichier CSV soit bien forme
    verifier_tetes_de_listes(vainqueurs_ballots);

    // Il y a autant de ballots que de ligne -1 puisqu'il faut ignorer l'en-tete
    int nb_ballots = mots_csv.lignes - 1;

    int vainqueur_id;
    char* nom_methode;
    // On execute la methode uninominale choisie
    if (methode_scrutin == METHODE_UNINOMINALE_UN_TOUR) {
        vainqueur_id = uninominale_un_tour(bdd_candidats, vainqueurs_ballots, nb_ballots);
        nom_methode = "uninominale a un tour";
    } else {
        erreur_fatale(3, "Uninominale deux tours pas encore implementee");

        // TODO: Implementer uninominale deux tours
        vainqueur_id = -1;
        nom_methode = "uninomnale a deux tours";
    }

    // Il y a autant de ballots que de votants
    afficher_scrutin(nom_methode, bdd_candidats, nb_ballots, vainqueur_id, 0);

    // On n'oublie pas de liberer les tableaux des vainqueurs de chaque ballot
    detruire_vainqueurs_ballots(vainqueurs_ballots, nb_ballots);
}

// Execute une methode de Condorcet
void executer_scrutin_condorcet(t_mat_char_star_dyn mots_csv, t_candidats bdd_candidats, int methode_scrutin) {
    // Les autres methodes ont toutes besoin d'une matrice de duels
    t_mat_int_dyn mat_duels;
    // On interprete la matrice de duels depuis une matrice directement en CSV,
    // ou depuis une liste de ballots en fonction du format d'entree
    int nb_votants; // On essaie aussi de lire dans le fichier le nb de votants
    if (methode_scrutin == ENTREE_MATRICE_DUELS) {
        convertir_mat_duels(mots_csv, &mat_duels);
        // Impossible de connaitre le nb de votants dans une matrice de duels
        nb_votants = 0;
    } else {
        creer_mat_duels(mots_csv, &mat_duels);
        nb_votants = mots_csv.lignes - 1; // Autant de votants que de ballots
    }

    // On verifie que le fichier CSV etait conforme au format d'entree demande
    verifier_matrice_des_duels(mat_duels);

    int vainqueur_id;
    char* nom_methode;
    // On execute la methode de scrutin choisie, qui sera condorcet minimax ou Schulze
    if (methode_scrutin == METHODE_CONDORCET_MINIMAX) {
        vainqueur_id = condorcet_minimax(mat_duels, bdd_candidats);
        nom_methode = "Condorcet minimax";
    } else {
        vainqueur_id = condorcet_schulze(mat_duels, bdd_candidats);
        nom_methode = "Condorcet Schulze";
    }

    // Il n'y a pas de score final en % puisqu'il ne s'agit pas d'une methode
    // uninominale
    afficher_scrutin(nom_methode, bdd_candidats, nb_votants, vainqueur_id, 0);

    // On n'oublie pas de detruire la mat de duels utilisee pour la methode
    detruire_t_mat_int_dyn(&mat_duels);
}


int main(int argc, char** argv) {
    // Initialisation du module de logging
    sortie_logging_par_defaut();

    // Interpretation des arguments du programme
    t_parametres params_scrutin;
    traiter_parametres(argc, argv, &params_scrutin);

    // Redirection du logging si cela est specifie dans les options du programme
    if (params_scrutin.option_logging != NULL)
        if (!rediriger_logging_sur(params_scrutin.option_logging))
            fprintf(stderr, "Le logging n'a pas pu etre redirige vers le fichier avec -o\n");

    // Est-ce qu'une methode uninominale devra etre executee ?
    bool methode_uninominale =
        params_scrutin.option_methode == METHODE_TOUTES ||
        params_scrutin.option_methode == METHODE_UNINOMINALE_UN_TOUR ||
        params_scrutin.option_methode == METHODE_UNINOMINALE_DEUX_TOURS;

    // Verification de la coherence du format d'entree avec la methode choisie
    if (params_scrutin.format_entree == ENTREE_MATRICE_DUELS && methode_uninominale)
        erreur_fatale(1, "Les methodes uni* et all necessitent une liste de ballots");

    // Ouverture du fichier CSV d'entree afin de le lire
    FILE* fichier_csv = fopen(params_scrutin.option_entree, "r");
    // On verifie qu'il a bien ete ouvert
    if (fichier_csv == NULL)
        erreur_fatale(2, "Ouverture fichier de votes");

    // Lecture du contenu CSV du fichier d'entree, les mots sont separes par
    // une virgule
    t_mat_char_star_dyn mots_csv;
    lire_fichier_votes(fichier_csv, ",", &mots_csv);

    // On interprete la liste des candidats present dans l'en-tete des ballots ou de
    // la matrice en fonction du format du CSV choisi
    t_candidats bdd_candidats;
    if (params_scrutin.format_entree == ENTREE_LISTE_BALLOTS)
        obtenir_candidats_ballots(mots_csv, &bdd_candidats);
    else
        obtenir_candidats_duels(mots_csv, &bdd_candidats);

    // Si on a pas pu lire la liste des candidats dans l'en-tete, alors le fichier
    // CSV est mal forme
    if (est_t_candidats_erreur(bdd_candidats))
        erreur_fatale(2, "Impossible de lister les candidats, verifier la presence"
            " d'une en-tete, contenant suffisamment de colonnes si c'est une"
            " liste de ballots.");

    // Execution de la methode choisie et interpretation du fichier CSV en fonction
    // (ou des methodes si on choisit d'executer toutes les methodes)
    if (params_scrutin.option_methode == METHODE_TOUTES)
        executer_tous_scrutins(mots_csv, bdd_candidats);
    else if (methode_uninominale)
        executer_scrutin_uninominale(mots_csv, bdd_candidats, params_scrutin.option_methode);
    else
        executer_scrutin_condorcet(mots_csv, bdd_candidats, params_scrutin.option_methode);

    // On desalloue toutes les ressources allouees pour le programme
    detruire_t_mat_char_star_dyn(&mots_csv);
    detruire_t_candidats_dyn(&bdd_candidats);
    fclose(fichier_csv);
    fermer_fichier_logging();

    return 0;
}