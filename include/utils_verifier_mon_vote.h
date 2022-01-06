/// \file Fonctions utilisees par le programme verifier_mon_vote, mises dans un module
/// a part afin d'etre testees
/// \author CHARRIERE Lelio
/// \date 05/01/2022

#ifndef UTILS_VERIFIER_MON_VOTE_H
#define UTILS_VERIFIER_MON_VOTE_H

#include <stdio.h>
#include <utils_sd.h>

/// \def Taille de la chaine de caractere contenant la cle privee, \0 exclu
#define TAILLE_CLE_PRIVEE 80
/// \def Taille d'un hash d'electeur present dans un fichier de ballots, \0 exclu.
/// 32 octets, chaque octet est represente en hexadecimal donc sur 2 chiffres [0-9a-f].
#define TAILLE_HASH_ELECTEUR 64


/// \fn Genere une chaine de caracteres alphanumerique aleatoire d'une longueur de 80,
/// donc suffisamment longue pour ne pas etre brute force
/// \param[out] cle_privee Tableau de 81 caracteres (chaine + caractere \0) pour
/// stocker la cle privee
void generer_cle_privee(char cle_privee[TAILLE_CLE_PRIVEE + 1]);

/// \fn Genere le hash de l'electeur auquel on a concatene a la cle privee
/// \param[in] nom_electeur Nom de l'electeur pour lequel on genere un hash
/// \param[in] cle_privee Cle utilisee par l'electeur pour acceder a ses votes
/// \param[out] hash_electeur Hash qui sera utilise pour cet electeur dans le fichier
void hash_electeur(char* nom_electeur, char* cle_privee, char hash_electeur[TAILLE_HASH_ELECTEUR + 1]);

/// \fn Ecrit la matrice de mots CSV dans le fichier ouvert en ecriture, en separant
/// chaque mot d'une ligne par le separateur donne.
/// \param[in] fichier_csv Pointeur de fichier vers le CSV ouvert en ecriture
/// \param[in] separateur Caractere qui va etre insere entre les mots d'une meme ligne
/// \param[in] mots_csv Mots a ecrire dans le fichier CSV
void ecrire_fichier_votes(FILE* fichier_csv, char separateur, t_mat_char_star_dyn mots_csv);

/// \fn Dans la matrice de mots CSV donnee, chiffre les noms des electeurs et ecrit
/// leurs cles privees associees aux noms dans un fichier de sortie
/// \param[inout] mots_csv Matrice de mots CSV contenu une liste de ballots conforme a
/// ce qui peut etre lu dans le module lecture_csv
/// \param[in] cles_privees Tableau des cles privees, chaque index du tableau possede une
/// cle associee a l'electeur de meme index dans les ballots de votes
/// \param[in] sortie_cles Fichier de sorties dans lequel on ecrit des lignes sous la forme
/// nom_electeur:cle_privee
void chiffrer_ballots_votes(t_mat_char_star_dyn mots_csv, char** cles_privees, FILE* sortie_cles);


#endif // UTILS_VERIFIER_MON_VOTE_H
