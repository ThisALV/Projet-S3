#include <condorcet_minimax.h>

#include <log.h>
#include <utils_scrutins.h>


// Nom du module, utilise pour le logging
static char* module = "condorcet_minimax";


// Retourne le pire score effectue par ce candidat dans tous ses duels
static int pire_score_candidat(t_mat_int_dyn duels, int candidat_id) {
    // On cherche le score minimum, on met au-dessus du % maximal pour
    // etre sur d'avoir un pire score issu de la matrice a la fin
    int candidat_pire_score = 101;
    // Une ligne/colonne par candidat
    int nb_candidats = duels.dim;

    // Puis on verifie s'il y en a des inferieurs pour chaque adversaire
    for (int adversaire_i = 0; adversaire_i < nb_candidats; adversaire_i++) {
        // Ignore la diagonale de la matrice (scores de 0)
        if (adversaire_i == candidat_id)
            continue;

        int score = duels.elems[candidat_id][adversaire_i];
        if (score < candidat_pire_score)
            candidat_pire_score = score;
    }

    return candidat_pire_score;
}

int condorcet_minimax(t_mat_int_dyn duels, t_candidats candidats) {
    log_ligne(module, "Methode de condorcet, departagee avec minimax sur :");
    log_t_mat_int_dyn(module, duels);

    // On essaie de trouver un vainqueur de condorcet
    int vainqueur = vainqueur_condorcet(duels);

    // S'il y en a un, inutile d'utiliser minimax pour departager les
    // candidats
    if (vainqueur != CONDORCET_AUCUN_VAINQUEUR) {
        log_ligne(module, "Vainqueur de condorcet : %d", vainqueur);
        return vainqueur;
    }

    log_ligne(module, "Aucun vainqueur de condorcet. Calcul minimax...");

    int nb_candidats = duels.dim;

    int meilleur_pire_score = -1; // Comme ca le 1er pire score sera forcement le meilleur
    int meilleur_pire_score_id; // Donc cette variable sera obligatoirement initialisee pendant la boucle
    for (int candidat_i = 0; candidat_i < nb_candidats; candidat_i++) {
        int candidat_pire_score = pire_score_candidat(duels, candidat_i);
        log_ligne(module, "Candidat %d, pire score : %d", candidat_i, candidat_pire_score);

        // Enfin, on compare le pire score de ce candidat au pire score general
        if (candidat_pire_score > meilleur_pire_score) {
            meilleur_pire_score = candidat_pire_score;
            meilleur_pire_score_id = candidat_i;
        // En cas d'egalite dans le pire score, on doit departager un gagnant
        } else if (candidat_pire_score == meilleur_pire_score) {
            log_ligne(module, "Egalite du meilleur des pires scores pour les candidats %d et %d.", meilleur_pire_score_id, candidat_i);

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
            log_ligne(module, "Le candidat retenu pour le meilleur pire score est le %d", meilleur_pire_score_id);

            detruire_t_candidats_dyn(&egalite);
            detruire_t_tab_int_dyn(&ids_egalite);
        }
    }

    log_ligne(module, "Meilleur des pires scores : %d par candidat %d", meilleur_pire_score, meilleur_pire_score_id);
    return meilleur_pire_score_id;
}
