#include <condorcet_schulze.h>

#include <log.h>
#include <erreur.h>
#include <stdlib.h>
#include <utils_scrutins.h>


// Nom du module, utilise pour le logging
static char* module = "condorcet_schulze";


// Initialise la partie "points" d'un t_graphe a partir d'une matrice de duels
static void initialiser_points_graphe(t_mat_int_dyn mat_duels, t_graphe* graphe_duels) {
    int nb_candidats = mat_duels.dim;

    // On a des candidats avec un ID de 0 inclu a nb_candidats exclu. Pour chacun
    // d'eux, on creer le point associe dans le graphe.
    graphe_duels->nb_points = nb_candidats;
    graphe_duels->points = (t_point*) malloc(nb_candidats * sizeof(t_point));
    verifier_alloc(graphe_duels->points, "Allocation tableau points du graphe");

    for (int candidat_id = 0; candidat_id < nb_candidats; candidat_id++) {
        log_ligne(module, "Ajout d'un point sur le graphe pour le candidat %d", candidat_id);

        // On recupere le point alloue en memoire pour le candidat courant
        t_point* point_candidat = &(graphe_duels->points[candidat_id]);

        point_candidat->candidat_id = candidat_id; // On assigne l'ID courant
        creer_t_liste_simple_int(&(point_candidat->defaites_contre)); // On creer la liste des vide des defaites
    }
}

// Creer un arc representant le duel entre les 2 candidats dans la matrice donnee
static void creer_arc_pour_duel(
    t_candidats candidats, t_mat_int_dyn duels, int candidat_id, int adversaire_id,
    t_arc_p* arc_duel)
{
    // On obtient les % de voix pour les 2 candidats
    int voix_candidat = duels.elems[candidat_id][adversaire_id];
    int voix_adversaire = duels.elems[adversaire_id][candidat_id];

    int gagnant_id;
    int perdant_id;
    int poids;

    // On determine le vainqueur
    if (voix_candidat > voix_adversaire) {
        gagnant_id = candidat_id;
    } else if (voix_candidat < voix_adversaire) {
        gagnant_id = adversaire_id;
    } else { // Gestion du cas d'egalite entre les 2 candidats
        t_tab_int_dyn candidats_egalite;
        creer_t_tab_int_dyn(&candidats_egalite, 2); // On a 2 candidats a egalite
        candidats_egalite.elems[0] = candidat_id;
        candidats_egalite.elems[1] = adversaire_id;

        // Recuperation des infos permettant de departager les 2 candidats
        t_candidats egalite;
        recuperer_infos_pour(&egalite, candidats, candidats_egalite);

        gagnant_id = departager_candidats(egalite);
    }

    // On determine le perdant comme etant celui qui n'a pas gagne, et le poids comme
    // la difference du % de voix entre le gagnant et le perdant
    if (gagnant_id == candidat_id) {
        perdant_id = adversaire_id;
        poids = voix_candidat - voix_adversaire;
    } else {
        perdant_id = candidat_id;
        poids = voix_adversaire - voix_candidat;
    }

    log_ligne(module, "Ajout d'un arc allant de %d a %d avec un poids de %d",
              gagnant_id, perdant_id, poids);

    // L'arc va du gagnant au perdant
    arc_duel->orig = gagnant_id;
    arc_duel->dest = perdant_id;
    arc_duel->poids = poids;
}

// Initialise la partie "arcs ponderes" d'un t_graphe a partir d'une matrice de duels
static void initialiser_arcs_graphe(t_candidats candidats, t_mat_int_dyn mat_duels, t_graphe* graphe_duels) {
    // Chaque candidat affrontant tous les candidats sauf lui-meme, il affronte
    // (nb_candidats - 1) candidats.
    // Applique a chaque candidat de la matrice, on a
    // nb_duels = nb_candidats * (nb_candidats - 1)
    // Cependant, pour eviter d'evalue 2 fois les memes duels (avec 0 vs 1 puis 1 vs 0
    // par exemple), on ne va traiter que la partie inferieure a la diagonale de la matrice.
    // On ne va donc traiter que la moitie des duels
    int nb_duels = (candidats.nb * (candidats.nb - 1)) / 2;
    creer_t_arcs_dyn(&(graphe_duels->arcs_initiaux), nb_duels);

    // L'index (ou ID) de l'arc dans lequel on va stocker les infos du prochain duel
    int arc_id = 0;
    // Pour chaque candidat
    for (int candidat_id = 0; candidat_id < candidats.nb; candidat_id++) {
        // Pour chaque duel qu'il a fait, EN-DESSOUS DE LA DIAGONALE DE LA MATRICE
        for (int adversaire_id = 0; adversaire_id < candidat_id; adversaire_id++) {
            // Arc dans lequel on va stocker les infos
            t_arc_p* arc_courant = &(graphe_duels->arcs_initiaux.elems[arc_id++]);

            // On assigne a l'arc les donnees du duel entre les 2 candidats
            creer_arc_pour_duel(candidats, mat_duels, candidat_id, adversaire_id, arc_courant);
        }
    }
}

// Relie les points entre eux en ajoutant a leur listes d'arcs les IDs des arcs
// representant les duels qu'ils ont perdu
static void relier_points_graphe(t_graphe* graphe_duels) {
    // Pour chaque arc
    for (int arc_id = 0; arc_id < graphe_duels->arcs_initiaux.nb; arc_id++) {
        // On informe au point destination de cet arc qu'il a perdu ce duel
        int perdant_id = graphe_duels->arcs_initiaux.elems[arc_id].dest;

        // Les points sont ranges par ID croissant du candidat, on peut donc
        // directement acceder au point du perdant depuis l'ID du candidat perdant
        t_liste_simple_int* arcs_perdant = &(graphe_duels->points[perdant_id].defaites_contre);
        // On ajoute le duel courant dans lequel il a perdu
        inserer_debut_t_liste_simple_int(arcs_perdant, arc_id);
    }
}


void creer_graphe_duels(t_candidats candidats, t_mat_int_dyn mat_duels, t_graphe* graphe_duels) {
    initialiser_points_graphe(mat_duels, graphe_duels);
    initialiser_arcs_graphe(candidats, mat_duels, graphe_duels);
    relier_points_graphe(graphe_duels);
}

void detruire_graphe_duels(t_graphe* graphe) {
    // On detruit le tableau des arcs
    detruire_t_arcs_dyn(&(graphe->arcs_initiaux));
    
    // On detruit les listes a l'interieur des points
    for (int pt_i = 0; pt_i < graphe->nb_points; pt_i++)
        detruire_t_liste_simple_int(&(graphe->points[pt_i].defaites_contre));

    // Enfin on detruit le tableau des points
    free(graphe->points);
}
