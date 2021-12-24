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

/// \struct Liste des arcs initialement presents a l'interieur d'un graphe et pouvant
/// etre references ou non par les points de ce graphe
struct s_arcs {
    /// Tableau dynamique des arcs
    t_arc_p* elems;
    /// Nombre d'arcs
    int nb;
} typedef t_arcs;

/// \struct Candidat pour lequel un electeur peut voter
struct s_candidat {
    /// Nom dynamiquement alloue
    char* nom;
    /// Identifiant qui est utilise dans les algorithmes
    int id;
} typedef t_candidat;

/// \struct Liste des candidats participant a une election
struct s_candidats {
    /// Tableau dynamique des candidats
    t_candidat* elems;
    /// Nombre de candidats participant a l'election
    int nb;
} typedef t_candidats;

/// \struct Cellule contenant un element de `t_liste_simple_int` et l'element suviant dans la liste
struct s_cellule_simple_int {
    /// Valeur de cet element de la liste
    int val;
    /// Pointeur vers l'element suivant dans la liste, ou NULL dans le cas d'une fin de liste
    struct s_cellule_simple_int* suiv;
} typedef t_cellule_simple_int;

/// \struct Liste simplement chainee d'entiers a usage generaliste
struct s_liste_simple_int {
    /// Pointeur vers le premier element de la liste, NULL si la liste est vide
    t_cellule_simple_int* elems;
    /// Nb de cellules (d'elements) contenus dans la liste
    int taille;
} typedef t_liste_simple_int;


/// \fn Creer un tableau d'entiers dynamique
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

/// \fn Creer un tableau de candidats dynamique
/// \param[in] dim Taille du tableau cree
/// \return Pointeur vers un tableau dynamique a la dimension dim ou NULL en cas d'erreur
t_candidat* creer_tab_candidats(int dim);

/// \fn Creer un tableau d'arcs ponderes dynamique
/// \param[in] dim Taille du tableau cree
/// \return Pointeur vers un tableau dynamique a la dimension dim ou NULL en cas d'erreur
t_arc_p* creer_tab_arcs(int dim);

/// \fn Creer un tableau d'entiers dynamique
/// \param[out] tab Tableau a initialiser
/// \param[in] dim Taille du tableau
/// \return `true` si le tableau a bien ete initialise
bool creer_t_tab_int_dyn(t_tab_int_dyn* tab, int dim);

/// \fn Detruit proprement le tableau d'entiers dynamique en desallouant sa memoire
/// \param[inout] tab Tableau a desallour, la dimension sera mise a 0 et le pointeur ira vers NULL
void detruire_t_tab_int_dyn(t_tab_int_dyn* tab);

/// \fn Met un tableau d'entiers non initalise en mode erreur
/// \param[out] tab Tableau a mettre en mode erreur
void mettre_t_tab_int_dyn_erreur(t_tab_int_dyn* tab);

/// \fn Verifie si un tableau est en mode erreur
/// \param[in] tab Tableau a verifier
/// \return `true` si le tableau est en mode erreur
bool est_t_tab_int_dyn_erreur(t_tab_int_dyn tab);

/// \fn Creer une matrice carree d'entiers dynamique
/// \param[out] mat Matrice a initialiser
/// \param[in] dim Longueur et hauteur de la matrice
/// \return `true` si la matrice a bien ete initialisee
bool creer_t_mat_int_dyn(t_mat_int_dyn* mat, int dim);

/// \fn Detruit proprement la matrice d'entiers dynamique en desallouant sa memoire
/// \param[inout] mat Matrice a desallouer, la dimension sera mise a 0 et le pointeur ira vers NULL
void detruire_t_mat_int_dyn(t_mat_int_dyn* mat);

/// \fn Met une matrice d'entiers non initalisee en mode erreur
/// \param[out] mat Matrice a mettre en mode erreur
void mettre_t_mat_int_dyn_erreur(t_mat_int_dyn* mat);

/// \fn Verifie si une matrice est en mode erreur
/// \param[in] mat Matrice a verifier
/// \return `true` si le matrice est en mode erreur
bool est_t_mat_int_dyn_erreur(t_mat_int_dyn mat);

/// \fn Creer un tableau d'entiers dynamique
/// \param[out] tab Tableau a initialiser
/// \param[in] dim Taille du tableau
/// \return `true` si le tableau a bien ete initialise
bool creer_t_tab_char_star_dyn(t_tab_char_star_dyn* tab, int dim);

/// \fn Detruit proprement le tableau de chaines de caracteres dynamique en desallouant sa memoire
/// \param[inout] tab Tableau a desallouer, la dimension sera mise a 0 et le pointeur ira vers NULL
void detruire_t_tab_char_star_dyn(t_tab_char_star_dyn* tab);

/// \fn Met un tableau de chaines de caracteres non initalise en mode erreur
/// \param[out] tab Tableau a mettre en mode erreur
void mettre_t_tab_char_star_dyn_erreur(t_tab_char_star_dyn* tab);

/// \fn Verifie si un tableau est en mode erreur
/// \param[in] tab Tableau a verifier
/// \return `true` si le tableau est en mode erreur
bool est_t_tab_char_star_dyn_erreur(t_tab_char_star_dyn tab);

/// \fn Creer une matrice de chaines de caracteres dynamique
/// \param[out] mat Matrice a initialiser
/// \param[in] lignes Hauteur de la matrice
/// \param[in] colonnes Longueur d'une ligne de la matrice
/// \return `true` si la matrice a bien ete initialisee
bool creer_t_mat_char_star_dyn(t_mat_char_star_dyn* mat, int lignes, int colonnes);

/// \fn Detruit proprement la matrice de chaines de caracteres dynamique en desallouant sa memoire
/// \param[inout] mat Matrice a desallouer, les dimensions seront mises a 0 et le pointeur ira vers NULL
void detruire_t_mat_char_star_dyn(t_mat_char_star_dyn* mat);

/// \fn Met une matrice de chaines de caracteres non initalisee en mode erreur
/// \param[out] mat Matrice a mettre en mode erreur
void mettre_t_mat_char_star_dyn_erreur(t_mat_char_star_dyn* mat);

/// \fn Verifie si une matrice est en mode erreur
/// \param[in] mat Matrice a verifier
/// \return `true` si le matrice est en mode erreur
bool est_t_mat_char_star_dyn_erreur(t_mat_char_star_dyn mat);

/// \fn Creer un tableau de candidats dynamique
/// \param[in] dim Nombre de candidats, la taille du tableau
/// \param[out] tab Tableau a initialiser
/// \return `true` si le tableau a bien ete initialise
bool creer_t_candidats_dyn(t_candidats* tab, int dim);

/// \fn Detruit proprement le tableau de candidats en desallouant sa memoire
/// \param[inout] tab Tableau a desallouer, la taille sera mise a 0 et le pointeur ira vers NULL
void detruire_t_candidats_dyn(t_candidats* tab);

/// \fn Met un tableau de candidats non initalise en mode erreur
/// \param[out] tab Tableau a mettre en mode erreur
void mettre_t_candidats_erreur(t_candidats* tab);

/// \fn Verifie si un tableau est en mode erreur
/// \param[in] tab Tableau a verifier
/// \return `true` si le tableau est en mode erreur
bool est_t_candidats_erreur(t_candidats tab);

/// \fn Creer un tableau d'arcs dynamique
/// \param[in] dim Nombre d'arcs, la taille du tableau
/// \param[out] tab Tableau a initialiser
/// \return `true` si le tableau a bien ete initialise
bool creer_t_arcs_dyn(t_arcs* tab, int dim);

/// \fn Detruit proprement le tableau d'arcs en desallouant sa memoire
/// \param[inout] tab Tableau a desallouer, la taille sera mise a 0 et le pointeur ira vers NULL
void detruire_t_arcs_dyn(t_arcs* tab);

/// \fn Creer une liste simplement chainee d'entiers
/// \param[out] liste Liste a initialiser
void creer_t_liste_simple_int(t_liste_simple_int* liste);

/// \fn Detruit proprement la liste en desallouant la memoire de chaque cellule
/// \param[inout] liste Liste a desaouller, la taille sera mise a 0 et le pointeur ira vers NULLL
void detruire_t_liste_simple_int(t_liste_simple_int* liste);

/// \fn Insere un entier dans une cellule qui sera le nouveau debut de la liste
/// \param[inout] liste Liste a laquelle sera ajoute le nouvel element. En sortie,
/// `elems` sera ajuste au nouveau debut de liste et `taille` sera incremente.
/// \param[in] val Valeur entiere dans la nouvelle cellule inseree
void inserer_debut_t_liste_simple_int(t_liste_simple_int* liste, int val);

/// \fn Supprime de la l'element contenant la valeur donnee
/// \param[inout] liste Liste a laquelle supprimer un element. En sortie, le
/// pointeur sur le 1er element et la taille seront ajustes a la nouvelle liste.
/// \param[in] val Valeur entiere contenue par la cellule (ou l'element) a supprimer
/// \return `true` si val etait dans la liste et qu'un element a bien ete supprime,
/// sinon `false`.
bool supprimer_valeur_t_liste_simple_int(t_liste_simple_int* liste, int val);

/// \fn Retire et obtient le 1er element de la liste. Effectuer cette operation sur
/// une liste vide mene a une erreur fatale du programme.
/// \param[inout] liste Liste a laquelle on va supprimer le premier element.
/// \return La valeur contenu dans l'element supprime
int retirer_premier_t_liste_simple_int(t_liste_simple_int* liste);


#endif // UTILS_SD_H
