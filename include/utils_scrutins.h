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


#endif // UTILS_SCRUTINS_H
