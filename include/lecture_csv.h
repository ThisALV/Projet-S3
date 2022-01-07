/// \file Fonctions pour lire le fichier CSV et l'interpreter en initialisant les SDD contenant les votes
/// \author CHARRIERE Lelio
/// \date 22/11/2021


#ifndef LECTURE_CSV_H
#define LECTURE_CSV_H

#include <stdio.h>
#include <utils_sd.h>


/// \fn Alloue dynamiquement une copie de la chaine donnee
/// \param[in] source Chaine a copier
/// \param[in] message_si_erreur Message a afficher si l'allocation en memoire echoue
/// \return Le pointeur vers la copie allouee dynamiquement en memoire
char* allouer_copie_char_star(char* source, char* message_si_erreur);

/// \fn Lis le fichier donne au format CSV et stocke les lignes et les mots, en-tete comprise, dans les cases de la matrice
/// \param[in] fichier_csv Pointeur vers le fichier CSV ouvert
/// \param[in] separateurs Liste des caracteres utilisees pour separer les mots au sein d'une meme ligne
/// \param[out] mots Matrice qui contiendra dans chaque ligne les mots lues dans le fichier
void lire_fichier_votes(FILE* fichier_csv, char* separateurs, t_mat_char_star_dyn* mots);

/// \fn Convertit la matrice de mots CSV en une matrice de duels entre les candidats
/// \param[in] mots_csv Duels au format CSV brut lus dans le fichier
/// \param[out] duels Matrice de duels contenant des entiers representant les scores en %. Pointeur NULL et dimension -1 en cas d'erreur recuperable
void convertir_mat_duels(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels);

/// \fn Obtient la liste des candidats 
/// \param[in] mots_csv Duels au format CSV brut lus dans le fichier
/// \param[out] candidats Liste des candidats participant a l'election contenue dans le fichier CSV. Pointeur NULL et taille -1 en cas d'erreur recuperable
void obtenir_candidats_duels(t_mat_char_star_dyn mots_csv, t_candidats* candidats);

/// \fn Obtient la liste des candidats depuis une matrice de ballots CSV
/// \param[in] mots_csv Ballots de vote au format CSV lus dans le fichier
/// \param[out] candidats Liste des candidats participant a l'election contenue dans le fichier CSV. Pointeur NULL et taille -1 en cas d'erreur recuperable
void obtenir_candidats_ballots(t_mat_char_star_dyn mots_csv, t_candidats* candidats);

/// \fn Obtient une matrice de duels, remplie pour les cases en bas de la diagonale, avec les scores sous forme de nb de voix
/// \param[in] mots_csv Ballots de vote au format CSV lus dans le fichier
/// \param[out] duels Matrice de duels contenant des entiers representant les scores en nb de voix
/// \param[out] nb_electeurs Nombre d'electeurs analyses dans la matrice CSV
void creer_mat_duels_absolue(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels, int* nb_electeurs);

/// \fn Pour chaque duel deja evalue, on passe le score final en % de voix, et on assigne au duel oppose, lui en haut de la diagonale, le % approprie. Exemple : ScoreDe(B vs A) = 100 - ScoreDe(A vs B)
/// \param[inout] duels Matrice de duels contenant les scores absolus, contiendra les scores relatifs en % apres l'appel
/// \param[in] nb_electeurs Nb de ballots de vote lus dans le fichier CSV
void completer_mat_duels(t_mat_int_dyn* duels, int nb_electeurs);

/// \fn Obtient une matrice de duels depuis une matrice de ballots CSV (ou liste de ballots)
/// \param[in] mots_csv Ballots de vote au format CSV lus dans le fichier
/// \param[out] duels Matrice de duels contenant des entiers representant les scores en %. Pointeur NULL et dimension -1 en cas d'erreur recuperable
void creer_mat_duels(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels);

/// \fn Obtient le candidat prefere au sein de ce ballot de votes
/// \param[in] ballot Tableau d'entier representant le rang de chaque candidat dans ce ballot, un rang -1 signifie qu'il n'a pas pu etre lu
/// \param[out] tetes L'ID de chaque candidat prefere dans ce ballot, peut etre vide
/// \pre Il doit y avoir AU MOINS 1 CANDIDAT
void premiers_de_ballot(t_tab_int_dyn ballot, t_tab_int_dyn* tetes);

/// \fn Obtient les candidats preferes de chaque ballot de vote
/// \param[in] mots_csv Ballots de vote au format CSV lus dans le fichier
/// \return Tableau des candidats gagnants dans chaque ballots, chaque element du 1er tableau est un 2eme tableau contenant les IDs des candidats gagnants dans ce ballot. NULL si erreur CSV.
/// La taille du 1er tableau de sortie est le nb de lignes moins l'en-tete, cad le nb de ballots de votes.
t_tab_int_dyn* tetes_de_listes(t_mat_char_star_dyn mots_csv);


#endif // LECTURE_CSV_H
