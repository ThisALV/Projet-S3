/// \file Fonctions pour lire le fichier CSV et l'interpreter en initialisant les SDD contenant les votes
/// \author CHARRIERE Lelio
/// \date 22/11/2021


#ifndef LECTURE_CSV_H
#define LECTURE_CSV_H

#include <stdio.h>
#include <utils_sd.h>


/// \fn Lis le fichier donne au format CSV et stocke les lignes et les mots, en-tete comprise, dans les cases de la matrice
/// \param[in] fichier_csv Pointeur vers le fichier CSV ouvert
/// \param[in] separateurs Liste des caracteres utilisees pour separer les mots au sein d'une meme ligne
/// \param[out] mots Matrice qui contiendra dans chaque ligne les mots lues dans le fichier
void lire_fichier_votes(FILE* fichier_csv, char* separateurs, t_mat_char_star_dyn* mots);

/// \fn Convertit la matrice de mots CSV en une matrice de duels entre les candidats
/// \param[in] mots_csv Duels au format CSV brut lus dans le fichier
/// \param[out] duels Matrice de duels contenant des entiers representant les scores en %. Vaudra NULL en cas d'erreur recuperable.
void convertir_mat_duels(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels);

/// \fn Obtient la liste des candidats 
/// \param[in] mots_csv Duels au format CSV brut lus dans le fichier
/// \param[out] candidats Liste des candidats participant a l'election contenue dans le fichier CSV. Pointeur NULL et taille -1 en cas d'erreur recuperable
void obtenir_candidats_duels(t_mat_char_star_dyn mots_csv, t_candidats* candidats);


#endif // LECTURE_CSV_H
