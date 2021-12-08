#ifndef UNINOMINALES_H
#define UNINOMINALES_H

#include <utils_sd.h>


/// \fn Scrutin uninominale a un tour, le candidats prefere de chaque ballot
/// est utilise pour determine qui est choisi par un electeur
/// \param[in] candidats BDD des candidats, pour acceder a leur infos qui
/// permettent de les departager en cas d'egalite
/// \param[in] ballots Ballots de vote permettant d'obtenir le candidat prefere
/// de chaque electeur
/// \param[in] nb_ballots Taille du tableau dynamique `ballots`
/// \return L'ID du vainqueur designe par le scrutin uninominale a 1 tour
int uninominale_un_tour(t_candidats candidats, t_tab_int_dyn* ballots, int nb_ballots);


#endif // UNINOMINALES_H
