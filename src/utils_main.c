#include <utils_main.h>

#include <erreur.h>
#include <stdlib.h>
#include <string.h>


// Verifie qu'aucune autre option de fichier d'entree n'a deja ete lue dans les
// arguments. Si ce n'est pas le cas, assigne le fichier d'entree et son format
// dans params.
static void assigne_entree_si_aucune_option(
    t_parametres* params, int format_entree, char* option_entree)
{
    if (params->format_entree != ENTREE_AUCUNE_OPTION)
        erreur_fatale(1, "Impossible d'avoir 2 fichiers d'entree");

    params->format_entree = format_entree;
    params->option_entree = option_entree;
}

// Associe l'int approprie a chaque methode de scrutin possible
static int intepreter_methode_scrutin(char* nom_methode) {
    if (strcmp(nom_methode, "all") == 0)
        return METHODE_TOUTES;

    if (strcmp(nom_methode, "uni1") == 0)
        return METHODE_UNINOMINALE_UN_TOUR;
        
    if (strcmp(nom_methode, "uni2") == 0)
        return METHODE_UNINOMINALE_DEUX_TOURS;

    if (strcmp(nom_methode, "cm") == 0)
        return METHODE_CONDORCET_MINIMAX;

    if (strcmp(nom_methode, "cs") == 0)
        return METHODE_CONDROCET_SCHULZE;

    erreur_fatale(1, "Methode de scrutin inconnue");

    return -1; // N'arrivera jamais puisqu'on leve une erreure fatale juste avant
}

void traiter_parametres(int argc, char** argv, t_parametres* params) {
    // On met chaque option a sa valeur par defaut afin d'essayer de les interpreter
    // dans les arguments
    params->format_entree = ENTREE_AUCUNE_OPTION;
    params->option_entree = NULL;
    params->option_logging = NULL;
    params->option_methode = METHODE_TOUTES;

    // Une option est une paire d'arguments balise + valeur, donc sachant qu'on
    // a un 1er argument qui est le nom de l'executable, on devrait avoir
    // 1 + <nb pair d'arguments> arguments, ce qui signifie avoir un nb impair
    // d'arguments. Sinon, il manque obligatoirement un argument.
    if (argc % 2 == 0)
        erreur_fatale(1, "1 argument manquant pour former une option");

    // On commence au 2eme arg pusiqNULLue le 1er contient le nom de l'executable
    // A chaque fois, on traite une paire d'arguments, 1 pour la balise et
    // 1 pour la valeur de l'option
    for (int arg_i = 1; arg_i < argc; arg_i += 2) {
        // Grace a notre verification d'avant, on sait qu'il reste au moins les
        // 2 arguments necessaires pour former une option
        char* balise = argv[arg_i];
        char* valeur = argv[arg_i + 1];

        // On verifie l'option sur laquelle on travaille
        if (strcmp(balise, "-i") == 0) {
            assigne_entree_si_aucune_option(params, ENTREE_LISTE_BALLOTS, valeur);
        } else if (strcmp(balise, "-d") == 0) {
            assigne_entree_si_aucune_option(params, ENTREE_MATRICE_DUELS, valeur);
        } else if (strcmp(balise, "-o") == 0) {
            if (params->option_logging != NULL)
                erreur_fatale(1, "Impossble d'avoir 2 fichiers de sortie");

            // L'argument etant stocke dans argv, il restera disponible durant
            // touta la vie du programme
            params->option_logging = valeur;
        } else if (strcmp(balise, "-m") == 0) {
            if (params->option_methode != METHODE_NON_ASSIGNEE)
                erreur_fatale(1, "Pour executer plusieurs methodes de scrutin, utilisez -m all");

            params->option_methode = intepreter_methode_scrutin(valeur);
        } else {
            erreur_fatale(1, "Balise d'option inconnue");
        }
    }

    // Si apres avoir verifie tous les arguments, on n'a toujours aucun fichier
    // d'entree, alors c'est une erreur
    if (params->option_entree == ENTREE_AUCUNE_OPTION)
        erreur_fatale(1, "Veuillez donner un fichier d'entree avec -i ou -d");

    // Si apres avoir verifie tous les arguments, aucune methode de scrutin n'a ete
    // specifie, alors on les execute toutes car la methode par defaut est "all"
    if (params->option_methode == METHODE_NON_ASSIGNEE)
        params->option_methode = METHODE_TOUTES;
}
