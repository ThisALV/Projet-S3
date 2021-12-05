/// \file Fonctions utilitaires pour executer les differentes methodes de scrutin
/// proposees par le programme
/// \author Lelio CHARRIERE
/// \date 03/12/2021

#ifndef UTILS_SCRUTINS_H
#define UTILS_SCRUTINS_H

#include <utils_sd.h>


/// \fn Departage plusieurs candidats a egalite dans un vote uninominal ou dans un duel pour une matrice
/// \param[in] candidats Tableau des candidats a egalite
/// \return L'ID du candidat gagnant, cad du plus age si on connait
/// l'age de tous les candidats, ou alors l'ID du 1er candidat donne si
/// un des ages est inconnu
int departager_candidats(t_candidats candidats);

/// \fn Compte le nb de voix pour chaque candidats en utilisant les candidats preferes
/// de chaque ballot de votes
/// \param[out] voix Decompte des voix pour chaque candidat, un indice du tab est un ID de candidat
/// \param[in] candidats Necessaire pour connaitre les infos qui serviront a
/// departager les candidats en cas d'egalite
/// \param[in] tetes_de_listes Tableau des candidats preferes de chaque ballots
/// \param[in] nb_ballots Nb d'elements dans `tetes_de_listes`
void compter_voix_ballots(t_tab_int_dyn* voix, t_candidats candidats, t_tab_int_dyn* tetes_de_listes, int nb_ballots);

/// \fn Comparer les voix obtenues par les candidats pour determiner et departager
/// si necessaire qui est le vainqueur
/// \param[in] voix Nb de voix obtenu pour chaque candidat, un indice du tab est un ID de candidat
/// \param[in] candidats Necessaire pour connaitre les infos qui serviront a
/// departager les candidats en cas d'egalite
/// \return L'ID du candidat qui a ete calcule comme etant vainqueur
int comparer_voix_ballots(t_tab_int_dyn voix, t_candidats candidats);

/// \fn Calcule le vainqueur d'un tour de vote selon la methode uninominale
/// \param[in] candidats Necessaire pour connaitre le nb de candidats et pour acceder a
/// leurs noms/ages qui serviront a les departager en cas d'egalite
/// \param[in] tetes_de_listes Tableau des candidats preferes de chaque ballots
/// \param[in] nb_ballots Nombre de tableaux des preferes dans `tetes_de_listes`
/// \return L'ID du candidat designe vainqueur selon cette methode
int vainqueur_uninominale(t_candidats candidats, t_tab_int_dyn* tetes_de_listes, int nb_ballots);


#endif // UTILS_SCRUTINS_H
