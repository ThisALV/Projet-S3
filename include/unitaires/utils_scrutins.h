/// \file

#ifndef UTILS_SCRUTINS_UNITAIRES_H
#define UTILS_SCRUTINS_UNITAIRES_H


// departager_candidats
void departager_candidats_1er_plus_age();
void departager_candidats_3eme_plus_age();
void departager_candidats_1_age_inconnu();
void departager_candidats_2_ages_inconnu();

// compter_voix_ballots
void compter_voix_ballots_un_gagnant();
void compter_voix_ballots_egalites();

// comparer_voix_ballots
void comparer_voix_ballots_normal();
void comparer_voix_ballots_egalite();

// vainqueur_uninominale
void vainqueur_uninominale_avec_egalites();

// vainqueur_condorcet
void vainqueur_condorcet_gagnant();
void vainqueur_condorcet_aucun_gagnant();
void vainqueur_condorcet_gagnant_moins_50();

/// \brief Execute toutes les series de tests unitaires pour ce module
void tests_unitaires_utils_scrutin();


#endif // UTILS_SCRUTINS_UNITAIRES_H
