/// \file Fonctions utilisees par le programme verifier_mon_vote, mises dans un module
/// a part afin d'etre testees
/// \author CHARRIERE Lelio
/// \date 05/01/2022

#ifndef UTILS_VERIFIER_MON_VOTE_H
#define UTILS_VERIFIER_MON_VOTE_H

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


#endif // UTILS_VERIFIER_MON_VOTE_H
