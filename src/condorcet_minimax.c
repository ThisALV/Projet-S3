#include <condorcet_minimax.h>

#include <utils_scrutins.h>


int condorcet_minimax(t_mat_int_dyn duels) {
    // On essaie de trouver un vainqueur de condorcet
    int vainqueur = vainqueur_condorcet(duels);

    // S'il y en a un, inutile d'utiliser minimax pour departager les
    // candidats
    if (vainqueur != CONDORCET_AUCUN_VAINQUEUR)
        return vainqueur;

    int nb_candidats = duels.dim;

    int pire_score = 101; // Comme ca le 1er score sera forcement le pire
    int pire_score_id; // Donc cette variable sera obligatoirement initialisee pendant la boucle
    for (int candidat_i = 0; candidat_i < nb_candidats; candidat_i++) {
        // On cherche le score minimum, on commence par partir du principe
        // que c'est le 1er
        int candidat_pire_score = duels.elems[candidat_i][0];

        // Puis on verifie s'il y en a des inferieurs pour chaque adversaire
        for (int adversaire_i = 0; adversaire_i < nb_candidats; adversaire_i++) {
            // Ignore la diagonale de la matrice (scores de 0)
            if (adversaire_i == candidat_i)
                continue;

            int score = duels.elems[candidat_i][adversaire_i];
            if (score < candidat_pire_score)
                candidat_pire_score = score;
        }

        // Enfin, on compare le pire score de ce candidat au pire score general
        if (pire_score > candidat_pire_score) {
            pire_score = candidat_pire_score;
            pire_score_id = candidat_i;
        }
    }

    return pire_score_id;
}
