/// \file Applique la methode de condorcet, utilise la methode de Schulze s'il n'y a aucun vainqueur de Condorcet.
/// Contient les outils pour construire le graphe necessaire a l'algorithme de la methode de Schulze
/// \author Lelio CHARRIERE
/// \date 22/12/2021

#ifndef CONDORCET_SCHULZE
#define CONDORCET_SCHULZE

#include <utils_sd.h>


/// \struct Point dans un graphe de duels, represente un candidat et les duels qu'il
/// a perdu
struct s_point {
    /// ID du candidat represente
    int candidat_id;
    /// IDs des arcs ayant ce point pour destination, donc representant les duels
    /// que le candidat a perdu
    t_liste_simple_int defaites_contre;
} typedef t_point;

/// \struct Graphe des candidats et de leurs duels, chaque point represente un candidat
/// et chaque arc va d'un candidat gagnant a un candidat perdant pour representer un
/// duel. La ponderation de l'arc est la difference du % de votes entre les 2 candidats.
struct s_graphe {
    /// Points (ou candidats) du graphes, ranges par ID croissant, allant donc de l'ID
    /// 0 inclu a l'ID nb_candidats exclu
    t_point* points;
    /// Nb de points dans le graphe
    int nb_points;
    /// Arcs initialement presents dans le graphe. ATTENTION : Quand un arc est supprime
    /// du graphe, il n'est pas supprime ici, mais ce sont ses references dans les t_point
    /// qui sont supprimees.
    t_arcs arcs_initiaux;
} typedef t_graphe;


/// \fn Creer un graphe de duels a partir d'une matrice de duels
/// \param[in] candidats Infos sur les candidats utilisees pour les departager en cas
/// d'egalite sur un duel
/// \param[in] mat_duels Matrice contenant tous les duels
/// \param[out] graphe_duels Graphe a remplir avec les points des candidats et les arcs
/// des duels
void creer_graphe_duels(t_candidats candidats, t_mat_int_dyn mat_duels, t_graphe* graphe_duels);

/// \fn Detruit proprement le graphe donnee en desallouant la memoire qui lui est necessaire
/// \param[inout] graphe Graphe a desallouer en memoire
void detruire_graphe_duels(t_graphe* graphe);

/// \brief Determine le vaiqueur de condorcet et, s'il n'y en a pas, departage avec
/// la methode de Schulze
/// \param[in] mat_duels Matrice des duels utilisee pour verifier s'il y a un
/// vainqueur de condorcet et, si ce n'est pas le cas, creer un graphe permettant
/// de determiner la vainqueur de Schulze
/// \param[in] candidats BDD des candidats utilisee pour fournir les infos
/// qui les departageront en cas de 50/50 sur un duel
/// \return L'ID du candidat vainqueur selon cette methode
int condorcet_schulze(t_mat_int_dyn mat_duels, t_candidats candidats);


#endif // CONDORCET_SCHULZE
