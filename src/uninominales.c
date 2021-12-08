#include <uninominales.h>

#include <utils_scrutins.h>


int uninominale_un_tour(t_candidats candidats, t_tab_int_dyn* ballots, int nb_ballots) {
    return vainqueur_uninominale(candidats, ballots, nb_ballots);
}
