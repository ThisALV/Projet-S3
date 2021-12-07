#include <utils_scrutins.h>

#include <stdlib.h>
#include <string.h>
#include <erreur.h>

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

// Pour chaque ID dans ids, retrouve les infos du candidat ayant cet ID dans
// candidats, et l'ajoute dans le tableau `filtres`
// Il est important que `candidats` soit une BDD de candidats conforme au
// programme, cad que l'indice ce chaque candidat soit == a son ID, donc dans l'ordre
static void recuperer_infos_pour(t_candidats* filtres, t_candidats candidats, t_tab_int_dyn ids) {
    filtres->elems = NULL; // 1er realloc = malloc, puis realloc quand on veut agrandir le tableau
    filtres->nb = 0;

    for (int id_i = 0; id_i < ids.taille; id_i++) {
        int id_candidat = ids.elems[id_i]; // ID de candidat selectionne

        // On ajoute une place dans le tableau
        filtres->nb++;
        filtres->elems = (t_candidat*) realloc(
            filtres->elems, filtres->nb * sizeof(t_candidat));
        verifier_alloc(filtres->elems, "Allocation infos candidats");

        // On assigne le candidat selectionne a cette place
        filtres->elems[filtres->nb - 1] = candidats.elems[id_candidat];
    }
}

void compter_voix_ballots(t_tab_int_dyn* voix, t_candidats candidats, t_tab_int_dyn* tetes_de_listes, int nb_ballots) {
    for (int ballot_i = 0; ballot_i < nb_ballots; ballot_i++) {
        t_tab_int_dyn preferes = tetes_de_listes[ballot_i];
        int vainqueur_ballot_id;

        // S'il y a une egalite dans ce ballot de votes
        if (preferes.taille != 1) { // Alors on departage
            t_candidats infos_preferes;
            recuperer_infos_pour(&infos_preferes, candidats, preferes);

            vainqueur_ballot_id = departager_candidats(infos_preferes);

            detruire_t_candidats_dyn(&infos_preferes);
        } else { // Sinon on recupere l'ID de l'unique vainqueur du ballot
            vainqueur_ballot_id = preferes.elems[0];
        }

        // On a determine l'unique candidat pour qui vote l'electeur ayant
        // fait ce ballot de votes, on peut le compter comme une voix
        voix->elems[vainqueur_ballot_id]++;
    }
}

int comparer_voix_ballots(t_tab_int_dyn voix, t_candidats candidats) {
    // On recupere l'ID du candidat ayant obtenu le maximum de voix
    int vainqueur_id = 0;
    int nb_voix_vainqueur = voix.elems[vainqueur_id];
    for (int candidat_id = 1; candidat_id < candidats.nb; candidat_id++) {
        int nb_voix_courant = voix.elems[candidat_id];

        // S'il y a une egalite entre le meilleur candidat et le candiat courant
        if (nb_voix_courant == nb_voix_vainqueur) {
            // On creer notre situation d'egalitee...
            t_candidats egalite;
            creer_t_candidats_dyn(&egalite, 2);
            egalite.elems[0] = candidats.elems[vainqueur_id];
            egalite.elems[1] = candidats.elems[candidat_id];

            // ...pour l'analyser afin de determiner qui est le candidat prioritaire
            vainqueur_id = departager_candidats(egalite);

            // On n'oublie de detruire notre situation d'egalitee
            detruire_t_candidats_dyn(&egalite);
        } else if (nb_voix_courant > nb_voix_vainqueur) {
            // Si le candidat courant a juste plus de voix, c'est le nouveau
            // vainqueur a depasser pour l'instant
            vainqueur_id = candidat_id;
            nb_voix_vainqueur = voix.elems[vainqueur_id];
        }
    }

    return vainqueur_id;
}

int vainqueur_uninominale(t_candidats candidats, t_tab_int_dyn* tetes_de_listes, int nb_ballots) {
    // Dans ce tableau, chaque indice est l'ID d'un candidat auquel un associe
    // un entier qui est son nb de voix
    t_tab_int_dyn voix;
    creer_t_tab_int_dyn(&voix, candidats.nb); // Toutes les voix commencent a 0

    compter_voix_ballots(&voix, candidats, tetes_de_listes, nb_ballots);
    return comparer_voix_ballots(voix, candidats);
}
