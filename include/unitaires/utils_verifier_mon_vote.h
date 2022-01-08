#ifndef UTILS_VERIFIER_MON_VOTE_UNITAIRES_H
#define UTILS_VERIFIER_MON_VOTE_UNITAIRES_H


// hash_electeur
void hash_electeur_nom_vide();
void hash_electeur_nom_electeur();

// chercher_electeur
void chercher_electeur_nb_colonnes_invalide();
void chercher_electeur_electeur_non_trouve();
void chercher_electeur_electeur_trouve();

/// \fn Execute les tests unitaires pour le module utils_verifier_mon_vote
void tests_unitaires_utils_verifier_mon_vote();


#endif // UTILS_VERIFIER_MON_VOTE_UNITAIRES_H
