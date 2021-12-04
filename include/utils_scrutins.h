/// \file Fonctions utilitaires pour executer les differentes methodes de scrutin
/// proposees par le programme
/// \author Lelio CHARRIERE
/// \date 03/12/2021

#ifndef UTILS_SCRUTINS_H
#define UTILS_SCRUTINS_H

#include <utils_sd.h>


/// \fn Departage 2 candidats a egalite dans un vote uninominal ou dans un duel pour une matrice
/// \param[in] c1 1er candidat a egalite
/// \param[in] c2 2eme candidat a egalite
/// \return L'ID du candidat gagnant, cad du plus age si on connait l'age des 2 candidats, ou alors c1 si un des ages n'est pas connu
int departager_candidats(t_candidat c1, t_candidat c2);


#endif // UTILS_SCRUTINS_H
