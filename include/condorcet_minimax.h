/// \file Applique la methode de condorcet, utilise la methode des minimax s'il n'y a aucun vainqueur de Condorcet
/// \author Lelio CHARRIERE
/// \date 14/12/2021

#ifndef CONDORCET_MINIMAX_H
#define CONDORCET_MINIMAX_H

#include <utils_sd.h>


/// \brief Determine le vaiqueur de condorcet et, s'il n'y en a pas, departage avec minimax
/// \param[in] duels Matrice des duels utilisee pour verifier s'il y a un
/// vainqueur de condorcet et, si ce n'est pas le cas, quel est le candidat
/// qui a la moins pire des pires defaites de chaque candidat
/// \param[in] candidats BDD des candidats utilisee pour fournir les infos
/// qui les departageront en cas de meme pire score
/// \return L'ID du candidat vainqueur selon cette methode
int condorcet_minimax(t_mat_int_dyn duels, t_candidats candidats);


#endif // CONDORCET_MINIMAX_H
