#include <log.h>

// Verifie si l'appel a la fonction ecriture a ete execute avec succes,
// ou s'il faut fermer le fichier de logging et arreter l'ecriture de la
// ligne pour cause d'erreur
#define VERIFIER_ECRITURE_DONNEES(ecriture) \
    if (ecriture < 0) {\
        fermer_fichier_logging();\
        va_end(donnees);\
        return;\
    }

/// Meme chose que VERIFIER_ECRITURE_DONNEES, mais sans avoir besoin de
/// liberer "donnees" a la fin
#define VERIFIER_ECRITURE(ecriture) \
    if (ecriture < 0) {\
        fermer_fichier_logging();\
        return;\
    }


// Garde en memoire la sortie du logging utilisee par le programme
static FILE* sortie;


FILE* sortie_logging_courante() {
    return sortie;
}

void sortie_logging_par_defaut() {
    sortie = stdout;
}

bool rediriger_logging_sur(char* chemin_fichier) {
    sortie = fopen(chemin_fichier, "w"); // On ouvre le fichier de sortie en ecriture

    bool sortie_ouverte = sortie != NULL;

    if (!sortie_ouverte) // On verifie s'il a bien ete ouvert
        sortie_logging_par_defaut();

    // On informe si le fichier de sortie a bien ete applique au logging
    return sortie_ouverte;
}

void fermer_fichier_logging() {
    // Rien a faire si aucun fichier de logging n'est ouvert
    if (sortie == stdout)
        return;

    // Sinon on ferme le fichier et on repasse sur stdout
    fclose(sortie);
    sortie_logging_par_defaut();
}   

void log_ligne(char* module, char* format, ...) {
    // On lit les donnees a formatter pour les transmettre a vfprintf plus tard
    va_list donnees;
    va_start(donnees, format);

    // Ecriture de la ligne :
    VERIFIER_ECRITURE_DONNEES(fprintf(sortie, "[%s] : ", module)); // Source du message
    VERIFIER_ECRITURE_DONNEES(vfprintf(sortie, format, donnees)); // Contenu du message
    VERIFIER_ECRITURE_DONNEES(fprintf(sortie, "\n")); // Fin de la ligne

    // On oublie pas de liberer les ressources utilisees par la lecture des paramettres
    va_end(donnees);
}

void log_t_tab_int_dyn(char* module, t_tab_int_dyn tab) {
    // On commence par ecrire le nom du module
    VERIFIER_ECRITURE(fprintf(sortie, "[%s] : ", module));

    // On ecrit element par element le tableau dans la sortie, en verifiant
    // a chaque fois qu'il se soit bien ecrit, et en les separant par " | "
    for (int elem_i = 0; elem_i < tab.taille; elem_i++)
        VERIFIER_ECRITURE(fprintf(sortie, " | %d", tab.elems[elem_i]));

    // Fin de la ligne apres le tableau
    VERIFIER_ECRITURE(fprintf(sortie, "\n"));
}

void log_t_mat_int_dyn(char* module, t_mat_int_dyn mat) {
    // On ecrit chaque ligne de la ligne comme etant un tableau d'elements
    for (int ligne_i = 0; ligne_i < mat.dim; ligne_i++) {
        // On creee STATIQUEMENT la SDD pour la ligne qui va etre affichee
        t_tab_int_dyn ligne = { mat.elems[ligne_i], mat.dim };

        log_t_tab_int_dyn(module, ligne);
    }
}

void log_t_liste_simple_int(char* module, t_liste_simple_int liste) {
    // On commence par ecrire le nom du module
    VERIFIER_ECRITURE(fprintf(sortie, "[%s] : ", module));

    // On ecrit cellule par cellule le contenu de chaque cellule jusqu'a la fin
    // de la liste
    for (t_cellule_simple_int* cell = liste.elems; cell != NULL; cell = cell->suiv)
        VERIFIER_ECRITURE(fprintf(sortie, " | %d", cell->val));

    // Fin de la ligne apres la liste
    VERIFIER_ECRITURE(fprintf(sortie, "\n"));
}
