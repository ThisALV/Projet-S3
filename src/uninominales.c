#include <uninominales.h>

#include <log.h>
#include <utils_scrutins.h>


// Nom du module, utilise pour le logging
static char* module = "uninominales";


int uninominale_un_tour(t_candidats candidats, t_tab_int_dyn* ballots, int nb_ballots) {
    int vainqueur = vainqueur_uninominale(candidats, ballots, nb_ballots);
    log_ligne(module, "Vainqueur selon la methode uninominale 1 tour : %d", vainqueur);

    return vainqueur;
}
