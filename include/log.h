/// \file Fonctions pour exploiter la fonctionnalite de logging dans un fichier
/// de sortie defini a l'echelle du programme. La sortie par defaut est `stdout`.
/// \author Lelio CHARRIERE
/// \date 20/12/2021

#ifndef LOG_H
#define LOG_H

#include <utils_sd.h>


/// \brief Ouvre le fichier sous le chemin donne en entree, et redirige le
/// logging vers celui-ci
/// \param[in] chemin_fichier Chemin du fichier ou ecrire le logging
/// \return `true` si le fichier a bien ete ouvert, `false` si ce n'est pas le cas
/// et le logging ecrira toujours sur la sortie par defaut
bool rediriger_logging_sur(char* chemin_fichier);

/// \brief Ferme le fichier de logging courant s'il est different de `stdout`
/// Ensuite, redirige le logging sur stdout.
void fermer_fichier_logging();

/// \brief Ecrit un message de logging depuis le module donne en parametre.
/// Si pour une raison ou une autre, le message n'a pas pu etre ecris, alors
/// on appel `fermer_fichier_logging()`.
/// \param[in] module Module depuis lequel le message a ete emis
/// \param[in] message Message a ecrire dans la sortie du logging
void log_sans_newline(char* module, char* message);

/// \brief Meme comportement que `log_sans_newline`, mais rajoute un \n
/// apres le message.
/// \param[in] module Module depuis lequel le message a ete emis
/// \param[in] message Message a ecrire dans la sortie du logging
void log(char* module, char* message);

/// \brief Ecrit dans la sortie de logging le contenu du tableau d'entiers
/// \param[in] tab Tableau d'entiers a ecrire
/// \param[in] ligne `true` si un \n doit etre ajoute apres le logging
void log_t_tab_int_dyn(t_tab_int_dyn tab, bool ligne);

/// \brief Ecrit dans la sortie de logging le contenu de la matrice d'entiers.
/// Un \n sera ajoute a la fin de chaque ligne de la matrice.
/// \param[in] mat Matrice d'entiers a ecrire
void log_t_mat_int_dyn(t_mat_int_dyn mat);


#endif // LOG_H
