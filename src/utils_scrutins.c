#include <utils_scrutins.h>

#include <stdlib.h>
#include <string.h>

// Rang d'anciennetee pour les candidats dont on ne connait pas l'age
#define RANG_AGE_NON_TROUVE -1


// En cas d'egalite, c'est le premier candidat present dans ce tableau qui
// gagne
char* plus_au_moins_age[] = {
    "B", // 70 ans, le 25 mai
    "M", // 70 ans, le 19 aout
    "S", // 66 ans
    "H", // 64 ans
    "L", // 53 ans
    NULL
};


// Pour un candidat donnee, verifie si son rang est encore introuve et, si c'est le
// cas, s'il correspond au rang du candidat courant a l'aide de son nom.
// Si ces deux conditions sont validees, alors on assigne le rang courant au rang
// du candidat analyse.
static void assigne_rang_candidat_si_trouve(int rang_courant, int* rang_c, char* nom_c) {
    char* nom_rang_courant = plus_au_moins_age[rang_courant];

    if (*rang_c == RANG_AGE_NON_TROUVE && strcmp(nom_c, nom_rang_courant) == 0)
        *rang_c = rang_courant;
}

// Verifie si tous les ages de tous les candidats sont connus
// Retourne l'index du 1er candidat (0) si un des candidats n'a pas d'age
// enregistre, sinon l'index du candidat avec le rang le plus bas
static int verifier_ages_candidats(t_tab_int_dyn rangs)  {
    // On commence par designe le premier candidat donne comme etant le +
    // age
    int plus_age_i = 0;

    for (int candidat_i = 1; candidat_i < rangs.taille; candidat_i++) {
        // On sauvegarde le rang du candidat courant
        int rang_candidat_i = rangs.elems[candidat_i];

        // Par defaut, on considere que c'est le 1er candidat donne en
        // entree qui est vainqueur
        if (rang_candidat_i == RANG_AGE_NON_TROUVE)
            return 0;

        if (rangs.elems[plus_age_i] > rangs.elems[candidat_i])
            plus_age_i = candidat_i;
    }

    // Si l'age de tous les candidats etait connu, alors on retorune l'ID
    // du plus age
    return plus_age_i;
}

int departager_candidats(t_candidats candidats) {
    // Un indice de ce tableau est l'indice d'un candidat dans le tableau
    // fourni en entree, auquel est associ son rang dans le classement
    // d'anciennetee
    t_tab_int_dyn rangs_age;
    creer_t_tab_int_dyn(&rangs_age, candidats.nb);

    // Au debut, on ne connait le rang d'aucun candidat dans le classement
    // par anciennetee
    for (int rang_i = 0; rang_i < rangs_age.taille; rang_i++)
        rangs_age.elems[rang_i] = RANG_AGE_NON_TROUVE;

    // On parcourt chaque rang dans le classement d'anciennetee
    for (int rang_age = 0; plus_au_moins_age[rang_age] != NULL; rang_age++) {
        for (int candidat_i = 0; candidat_i < candidats.nb; candidat_i++) {
            // Pointeur vers le rang du candidat sur lequel on travaille
            int* rang_candidat_i = &(rangs_age.elems[candidat_i]);
            // Nom du candidat sur lequel on travaille
            char* nom_candidat_i = candidats.elems[candidat_i].nom;

            assigne_rang_candidat_si_trouve(rang_age, rang_candidat_i, nom_candidat_i);
        }
    }

    int vainqueur_i = verifier_ages_candidats(rangs_age);
    // Liberation des ressources de l'algorithme
    detruire_t_tab_int_dyn(&rangs_age);

    // Grace a l'indice obtenu, on accede a l'ID du candidat vainqueur
    return candidats.elems[vainqueur_i].id;
}
