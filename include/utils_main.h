/// \file Contient les fonctions utilisees dans le script principal du programme,
/// mises dans un module a part afin de pouvoir les tester.
/// \author Lelio CHARRIERE
/// \date 28/12/2021

#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

/// \def Aucune option -d ou -i n'a ete rencontree
#define ENTREE_AUCUNE_OPTION 0
/// \def Option -i rencontree
#define ENTREE_LISTE_BALLOTS 1
/// \def Option -d rencontree
#define ENTREE_MATRICE_DUELS 2

/// \def Pas encore d'option rencontree, utilise en interne par le module mais
/// ne devrait pas arriver a l'utilisateur
#define METHODE_NON_ASSIGNEE -1
/// \def Option all, par defaut
#define METHODE_TOUTES 0
/// \def Option uni1
#define METHODE_UNINOMINALE_UN_TOUR 1
/// \def Option uni2
#define METHODE_UNINOMINALE_DEUX_TOURS 2
/// \def Option cm
#define METHODE_CONDORCET_MINIMAX 3
/// \def Option cs
#define METHODE_CONDROCET_SCHULZE 4


/// \struct Decrit les options donnees au programme interpretees par les foncitons
/// de ce module.
struct s_parametres {
    /// Option d'entree rencontree (voir les constantes ENTREE_*)
    int format_entree;
    /// Nom dans argv du fichier d'entree, quel que soit son format
    char* option_entree;
    /// Nom dans argv alloue du fichier de log a ouvrir, NULL si aucun
    char* option_logging;
    /// Methode de scrutin a utiliser (voir les constantes METHODE_*)
    int option_methode;
} typedef t_parametres;


/// \brief Interpretes les arguments donnes au programme pour connaitre la methode
/// de scrutin, le fichier d'entree, son format et le fichier de logging
/// \param[in] argc Nombre d'arguments donnes, argv[0] (chemin de l'executable) compris
/// \param[in] argv Tableau des arguments donnes, argv[0] (chemin de l'executable) compris
/// \param[out] params Contient les options interpretees pour les exploiter ensuite
void traiter_parametres(int argc, char** argv, t_parametres* params);


#endif // UTILS_MAIN_H
