/// \file Fonction pour lire un fichier CSV depuis le systeme de fichiers et en extraire les valeurs separees
/// \author CHARRIERE Lelio
/// \date 22/11/2021


#ifndef LECTURE_CSV_H
#define LECTURE_CSV_H

#include <stdio.h>
#include <utils_sd.h>


/// \fn Lis le fichier donne au format CSV et stocke les lignes et les mots, en-tete comprise, dans les cases de la matrice
/// \param[in] fichier_csv Pointeur vers le fichier CSV ouvert
/// \param[in] separateurs Liste des caracteres utilisees pour separer les mots au sein d'une meme ligne
/// \param[out] mots Matrice qui contiendra dans chaque ligne les mots lues dans le fichier, en cas d'erreur d'allocation memoire, elems vaudra NULL
void lire_fichier_votes(FILE* fichier_csv, char* separateurs, t_mat_char_star_dyn* mots);


#endif // LECTURE_CSV_H
