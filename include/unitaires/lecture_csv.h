#ifndef LECTURE_CSV_UNITAIRES_H
#define LECTURE_CSV_UNITAIRES_H


// convertir_mat_duels
void convertir_mat_duels_non_carree();
void convertir_mat_duels_vide();
void convertir_mat_duels_ok();

// obtenir_candidats_duels
void obtenir_candidats_duels_csv_vide();
void obtenir_candidats_duels_nb_colonnes_invalide();
void obtenir_candidats_duels_ok();

// obtenir_candidats_ballots
void obtenir_candidats_ballots_csv_vide();
void obtenir_candidats_ballots_nb_colonnes_invalide();
void obtenir_candidats_ballots_ok();

// creer_mat_duels_absolue
void creer_mat_duels_absolue_erreurs_ballots();
void creer_mat_duels_absolue_csv_bon();

// completer_mat_duels
void completer_mat_duels_test();

// creer_mat_duels
void creer_mat_duels_csv_vide();
void creer_mat_duels_aucun_electeur();
void creer_mat_duels_nb_colonnes_invalide();
void creer_mat_duels_ok();

/// \fn Execute les tests unitaires pour le module lecture_csv
void tests_unitaires_lecture_csv();


#endif // LECTURE_CSV_UNITAIRES_H
