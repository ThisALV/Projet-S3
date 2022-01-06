/// \file Fonctions utilisees par le programme verifier_mon_vote, mises dans un module
/// a part afin d'etre testees
/// \author CHARRIERE Lelio
/// \date 05/01/2022

#ifndef UTILS_VERIFIER_MON_VOTE_H
#define UTILS_VERIFIER_MON_VOTE_H


/// \def Taille de la chaine de caractere contenant la cle privee, \0 exclu
#define TAILLE_CLE_PRIVEE 80


/// \fn Genere une chaine de caracteres alphanumerique aleatoire d'une longueur de 80,
/// donc suffisamment longue pour ne pas etre brute force
/// \param[out] cle_privee Tableau de 81 caracteres (chaine + caractere \0) pour
/// stocker la cle privee
void generer_cle_privee(char cle_privee[TAILLE_CLE_PRIVEE + 1]);


#endif // UTILS_VERIFIER_MON_VOTE_H
