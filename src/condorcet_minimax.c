#include <condorcet_minimax.h>

#include <utils_scrutins.h>


int condorcet_minimax(t_mat_int_dyn duels, t_candidats candidats) {
    // On essaie de trouver un vainqueur de condorcet
    int vainqueur = vainqueur_condorcet(duels);

    // S'il y en a un, inutile d'utiliser minimax pour departager les
    // candidats
    if (vainqueur != CONDORCET_AUCUN_VAINQUEUR)
        return vainqueur;

    int nb_candidats = duels.dim;

    int meilleur_pire_score = -1; // Comme ca le 1er pire score sera forcement le meilleur
    int meilleur_pire_score_id; // Donc cette variable sera obligatoirement initialisee pendant la boucle
    for (int candidat_i = 0; candidat_i < nb_candidats; candidat_i++) {
        // On cherche le score minimum, on met au-dessus du % maximal pour
        // etre sur d'avoir un pire score issu de la matrice a la fin
        int candidat_pire_score = 101;

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
        if (candidat_pire_score > meilleur_pire_score) {
            meilleur_pire_score = candidat_pire_score;
            meilleur_pire_score_id = candidat_i;
        // En cas d'egalite dans le pire score, on doit departager un gagnant
        } else if (candidat_pire_score == meilleur_pire_score) {
            // On creer un tableau contenant les IDs des candidats qui ont le meme
            // pire score
            t_tab_int_dyn ids_egalite;
            creer_t_tab_int_dyn(&ids_egalite, 2);
            ids_egalite.elems[0] = meilleur_pire_score_id;
            ids_egalite.elems[1] = candidat_i;

            // On recupere les infos de ces candidats depuis la BDD en utilisant
            // ce tableau
            t_candidats egalite;
            recuperer_infos_pour(&egalite, candidats, ids_egalite);

            // L'ID du nouveau candidat gagnant est assigne au gagnant departage
            meilleur_pire_score_id = departager_candidats(egalite);

            detruire_t_candidats_dyn(&candidats);
            detruire_t_tab_int_dyn(&ids_egalite);
        }
    }

    return meilleur_pire_score_id;
}
