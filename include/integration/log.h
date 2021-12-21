#ifndef LOG_INTEGRATION_H
#define LOG_INTEGRATION_H


// sortie_logging_par_defaut
void sortie_logging_par_defaut_test();

// rediriger_logging_sur
void rediriger_logging_sur_ok();
void rediriger_logging_sur_creation_fichier_impossible();

// fermer_fichier_logging
void fermer_fichier_logging_sortie_par_defaut();
void fermer_fichier_logging_fichier_ouvert();

// log_ligne
void log_ligne_message_simple();
void log_ligne_message_avec_donnees();

// log_t_tab_int_dyn et log_t_mat_int_dyn
void log_t_tab_int_dyn_ok();
void log_t_mat_int_dyn_ok();


// Execute les tests de ce module, tout en veillant a l'ouverture et
// a la fermeture du fichier test de logging
void tests_integration_log();


#endif // LOG_INTEGRATION_H
