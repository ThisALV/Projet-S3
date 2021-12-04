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

int departager_candidats(t_candidat c1, t_candidat c2) {
    int rang_age_c1 = RANG_AGE_NON_TROUVE;
    int rang_age_c2 = RANG_AGE_NON_TROUVE;

    // On parcourt chaque rang dans le classement d'anciennetee
    for (int rang_age = 0; plus_au_moins_age[rang_age] != NULL; rang_age++) {
        assigne_rang_candidat_si_trouve(rang_age, &rang_age_c1, c1.nom);
        assigne_rang_candidat_si_trouve(rang_age, &rang_age_c2, c2.nom);
    }

    // Si on connait l'age des 2 candidats
    if (rang_age_c1 != RANG_AGE_NON_TROUVE && rang_age_c2 != RANG_AGE_NON_TROUVE) {
        // On retourne l'ID du candidat le plus age

        if (rang_age_c1 < rang_age_c2)
            return c1.id;

        return c2.id;
    }

    // Sinon on retourne simplement le 1er candidat donne
    return c1.id;
}
