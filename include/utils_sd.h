/// \file Fonctions utilitaires pour utiliser les structures de donnees basiques
/// \author CHARRIERE Lelio
/// \date 21/11/2021

#ifndef UTILS_SD_H
#define UTILS_SD_H

#include <stdbool.h>


/// \struct Tableau d'entiers a taille dynamique
struct s_tab_int_dyn {
    /// Pointeur vers le tableau alloue en memoire
    int* elems;
    /// Taille du tableau
    int taille;
} typedef t_tab_int_dyn;

/// \struct Matrice carree d'entiers a dimension dynamique
struct s_mat_int_dyn {
    /// Pointeur vers la matrice allouee en memoire
    int** elems;
    /// Longueur et hauteur de la matrice
    int dim;
} typedef t_mat_int_dyn;

/// \struct Tableau de chaines de caracteres a taille dynamique
struct s_tab_char_star_dyn {
    /// Pointeur vers le tableau alloue en memoire
    char** elems;
    /// Taille du tableau
    int taille;
} typedef t_tab_char_star_dyn;

/// \struct Matrice de chaines de caracteres a dimension dynamique
struct s_mat_char_star_dyn {
    /// Pointeur vers la matrice allouee en memoire
    char*** elems;
    /// Hauteur de la matrice
    int lignes;
    /// Longueur d'une ligne de la matrice
    int colonnes;
} typedef t_mat_char_star_dyn;

/// \struct Arc pondere, defini par sa source, sa destination et son poids
struct s_arc_p {
    /// Source : ID du candidat ayant gagne le duel
    int orig;
    /// Destination : ID du candidant ayant perdu le duel
    int dest;
    /// Poids : nombre de voix pour le gagnant
    int poids;
} typedef t_arc_p;


/// \fn Creer un tableau d'eniers dynamique
/// \param[in] dim Taille du tableau cree
/// \return Pointeur vers un tableau dynamique a la dimension dim ou NULL en cas d'erreur
int* creer_tab_int(int dim);

/// \fn Creer une matrice carree d'entiers dynamique
/// \param[in] dim Longueur et hauteur de la matrice
/// \return Pointeur vers une matrice dynamqiue a la dimension dim*dim ou NULL en cas d'erreur
int** creer_mat_int(int dim);

/// \fn Desalloue une matrice carree d'entiers dynamique
/// \param[inout] mat Memoire a desallouer
/// \param[in] dim Dimension de la matrice carree
void detruire_mat_int(int** mat, int dim);

/// \fn Creer un tableau de chaines de caracteres dynamique
/// \param[in] dim Taille du tableau cree
/// \return Pointeur vers un tableau dynamique a la dimension dim ou NULL en cas d'erreur
char** creer_tab_char_star(int dim);

/// \fn Creer une matrice de chaines de caracteres dynamique
/// \param[in] lignes Hauteur de la matrice
/// \param[in] colonnes Longueur d'une ligne de la matrice
/// \return Pointeur vers une matrice dynamqiue a la dimension lignes*colonnes ou NULL en cas d'erreur
char*** creer_mat_char_star(int lignes, int colonnes);

/// \fn Desalloue une matrice de chaines de caracteres dynamique
/// \param[inout] mat Memoire a desallouer
/// \param[in] lignes Haute de la matrice
void detruire_mat_char_star(char*** mat, int lignes);

/// \fn Creer un tableau d'entiers dynamique
/// \param[out] tab Tableau a initialiser
/// \param[in] dim Taille du tableau
/// \return `true` si le tableau a bien ete initialise
bool creer_t_tab_int_dyn(t_tab_int_dyn* tab, int dim);

/// \fn Detruit proprement le tableau d'entiers dynamique en desallouant sa memoire
/// \param[inout] tab Tableau a desalloue, la dimension sera mise a 0 et le pointeur ira vers NULL
void detruire_t_tab_int_dyn(t_tab_int_dyn* tab);

/// \fn Creer une matrice carree d'entiers dynamique
/// \param[out] mat Matrice a initialiser
/// \param[in] dim Longueur et hauteur de la matrice
/// \return `true` si la matrice a bien ete initialisee
bool creer_t_mat_int_dyn(t_mat_int_dyn* mat, int dim);

/// \fn Detruit proprement la matrice d'entiers dynamique en desallouant sa memoire
/// \param[inout] mat Matrice a desallouee, la dimension sera mise a 0 et le pointeur ira vers NULL
void detruire_t_mat_int_dyn(t_mat_int_dyn* mat);

/// \fn Creer un tableau d'entiers dynamique
/// \param[out] tab Tableau a initialiser
/// \param[in] dim Taille du tableau
/// \return `true` si le tableau a bien ete initialise
bool creer_t_tab_char_star_dyn(t_tab_char_star_dyn* tab, int dim);

/// \fn Detruit proprement le tableau de chaines de caracteres dynamique en desallouant sa memoire
/// \param[inout] tab Tableau a desalloue, la dimension sera mise a 0 et le pointeur ira vers NULL
void detruire_t_tab_char_star_dyn(t_tab_char_star_dyn* tab);

/// \fn Creer une matrice de chaines de caracteres dynamique
/// \param[out] mat Matrice a initialiser
/// \param[in] lignes Hauteur de la matrice
/// \param[in] colonnes Longueur d'une ligne de la matrice
/// \return `true` si la matrice a bien ete initialisee
bool creer_t_mat_char_star_dyn(t_mat_char_star_dyn* mat, int lignes, int colonnes);

/// \fn Detruit proprement la matrice de chaines de caracteres dynamique en desallouant sa memoire
/// \param[inout] mat Matrice a desallouee, les dimensions seront mises a 0 et le pointeur ira vers NULL
void detruire_t_mat_char_star_dyn(t_mat_char_star_dyn* mat);


#endif // UTILS_SD_H
