/// \file
/// \brief Tests unitaires pour le module utils_sd
/// \author CHARRIERE Lelio
/// \date 21/11/2021

#ifndef UTILS_SD_UNITAIRES_H
#define UTILS_SD_UNITAIRES_H

#include <utils_sd.h>


// creer_tab_int
void creer_tab_int_dim_ok();
void creer_tab_int_dim_nulle();
void creer_tab_int_dim_negative();

// creer_mat_int
void creer_mat_int_dim_ok();
void creer_mat_int_dim_negative();

// creer_tab_char_star
void creer_tab_char_star_dim_ok();
void creer_tab_char_star_dim_negative();

// creer_mat_char_star
void creer_mat_char_star_dim_ok();
void creer_mat_char_star_lignes_negatif();
void creer_mat_char_star_colonnes_negatif();

// creer_tab_candidats
void creer_tab_candidats_dim_ok();
void creer_tab_candidats_dim_negative();

// creer_t_tab_int_dyn
void creer_t_tab_int_dyn_dim_ok();
void creer_t_tab_int_dyn_dim_negative();

// detruire_t_tab_int_dyn
void detruire_t_tab_int_dyn_test();

// creer_t_mat_int_dyn
void creer_t_mat_int_dyn_dim_ok();
void creer_t_mat_int_dyn_dim_negative();

// detruire_t_mat_int_dyn
void detruire_t_mat_int_dyn_test();

// creer_t_tab_char_star
void creer_t_tab_char_star_dyn_dim_ok();
void creer_t_tab_char_star_dyn_dim_negative();

// detruire_t_tab_char_star_dyn
void detruire_t_tab_char_star_dyn_test();

// creer_t_mat_char_star
void creer_t_mat_char_star_dyn_dim_ok();
void creer_t_mat_char_star_dyn_lignes_negatif();
void creer_t_mat_char_star_dyn_colonnes_negatif();

// detruire_t_char_star_star_dyn
void detruire_t_mat_char_star_dyn_test();

// creer_t_candidats_dyn
void creer_t_candidats_dyn_dim_ok();
void creer_t_candidats_dyn_dim_negative();

// detruire_t_candidats_dyn
void detruire_t_candidats_dyn_test();

// mettre_t_*_erreur
void mettre_t_tab_int_dyn_erreur_test();
void mettre_t_mat_int_dyn_erreur_test();
void mettre_t_tab_char_star_dyn_erreur_test();
void mettre_t_mat_char_star_dyn_erreur_test();
void mettre_t_candidats_erreur_test();

// est_t_*_erreur
void est_t_tab_int_dyn_erreur_ok();
void est_t_tab_int_dyn_erreur_erreur();

// creer_t_liste_simple_int
void creer_t_liste_simple_int_test();

// detruire_t_liste_simple_int
void detruire_t_liste_simple_int_non_vide();
void detruire_t_liste_simple_int_vide();

// inserer_debut_t_liste_simple_int
void inserer_debut_t_liste_simple_int_test();

// supprimer_valeur_t_liste_simple_int
void supprimer_valeur_t_liste_simple_int_non_trouve();
void supprimer_valeur_t_liste_simple_int_trouve_debut();
void supprimer_valeur_t_liste_simple_int_trouve_milieu();

// retirer_premier_t_liste_simple_int
void retirer_premier_t_liste_simple_int_plusieurs_elems();
void retirer_premier_t_liste_simple_int_unique_elem();

// valeur_existe_t_liste_simple_int
void valeur_existe_t_liste_simple_int_trouvee();
void valeur_existe_t_liste_simple_int_non_trouvee();

/// \brief Execute toutes les series de tests unitaires pour ce module
void tests_unitaires_utils_sd();


#endif // UTILS_SD_UNITAIRES_H
