/// \file
/// \brief Certaines erreurs de ce programme sont fatales : il n'y a aucun
/// moyen de les reparer
/// \author CHARRIERE Lelio
/// \date 24/11/2021


#ifndef ERREUR_H
#define ERREUR_H

/// \brief Code retourne par le processus lorsqu'une erreur interne se produit
#define ERR_INTERNE 2

/// \brief Verifie qu'une allocation memoire s'est bien deroulee, sinon signale
/// une erreur fatale
/// \param[in] mem Memoire retournee par malloc/realloc/calloc
/// \param[in] message Message a afficher en cas d'erreur d'allocation
void verifier_alloc(void* mem, char* message);

/// \brief Affiche un message d'erreur et interromp le programme en cas
/// d'erreur fatale
/// \param[in] code Status que le processus doit retourner
/// \param[in] message Message a afficher dans stderr avant de fermer le processus
void erreur_fatale(int code, char* message);


#endif // ERREUR_H
