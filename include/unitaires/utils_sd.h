/// \file Tests unitaires pour le module utils_sd
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
void creer_tab_int_dim_negative();

// creer_tab_char_star
void creer_tab_char_star_dim_ok();
void creer_tab_char_star_dim_negative();

// creer_mat_char_star
void creer_mat_char_star_dim_ok();
void creer_mat_char_star_lignes_negatif();
void creer_mat_char_star_colonnes_negatif();

// creer_t_tab_int_dyn
void creer_t_tab_int_dyn_dim_ok();
void creer_t_tab_int_dyn_dim_negative();

// creer_t_mat_int_dyn
void creer_t_mat_int_dyn_dim_ok();
void creer_t_mat_int_dyn_dim_negative();

// creer_t_tab_char_star
void creer_t_tab_char_star_dim_ok();
void creer_t_tab_char_star_dim_negative();

// creer_t_mat_char_star
void creer_t_mat_char_star_dim_ok();
void creer_t_mat_char_star_lignes_negatif();
void creer_t_mat_char_star_colonnes_negatif();

/// \fn Execute toutes les series de tests unitaires pour ce module
void tests_unitaires_utils_sd();


#endif // UTILS_SD_UNITAIRES_H
