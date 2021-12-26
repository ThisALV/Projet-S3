#include <condorcet_schulze.h>

#include <log.h>
#include <erreur.h>
#include <stdlib.h>
#include <utils_scrutins.h>

// Decrit une situation ou un point n'a plus aucun defaite que nous n'avons pas
// deja pris en compte, et donc c'est la fin de notre groupe de tete
#define FIN_GROUPE_DE_TETE -1
// ID candidat pour un point ne faisant plus partie du graphe
#define POINT_SUPPRIME -1
// ID utilise pour signifier qu'aucun candidat n'a encore ete designe vainqueur
// par la methode de Schulze
#define SCHULZE_AUCUN_VAINQUEUR -1


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


// Determine recursivement un groupe de tete a l'interieur d'un graphe en partant
// d'un point precis
static void groupe_de_tete(t_graphe* duels, t_liste_simple_int* groupe_parcouru, t_point* depart) {
    // En partant de ce point pour regarder ses defaites et etendre le groupe, on
    // l'ajoute au groupe de tete
    inserer_debut_t_liste_simple_int(groupe_parcouru, depart->candidat_id);

    // Prochain point que l'on va explorer dans le graphe pour etendre le groupe
    // de tete
    int point_non_explore = FIN_GROUPE_DE_TETE;
    // Pour chaque duel perdu par le candidat courant
    t_cellule_simple_int* premier_defaite = depart->defaites_contre.elems;
    for (t_cellule_simple_int* defaite = premier_defaite; defaite != NULL; defaite = defaite->suiv) {
        int arc_id = defaite->val; // Arc representant le duel perdu
        // ID du candidat ayant gagne ce duel, donc de l'adversaire du candidat courant
        int adversaire_id = duels->arcs_initiaux.elems[arc_id].orig;

        // Si on n'a pas deja explore ce point, alors on va etendre le groupe
        // de tete vers lui
        if (!valeur_existe_t_liste_simple_int(*groupe_parcouru, adversaire_id)) {
            point_non_explore = adversaire_id;
            break; // On ne va explorer qu'un seul nouveau point
        }
    }

    if (point_non_explore == FIN_GROUPE_DE_TETE)
        return; // Plus rien a explorer dans le graphe, groupe de tete etabli

    // Ce point etant dans le groupe de tete, on ajoute au groupe les points ayant
    // gagne contre lui
    t_point* point_suviant = &(duels->points[point_non_explore]); // Prochain pt a partir de son ID
    groupe_de_tete(duels, groupe_parcouru, point_suviant); // On continue le parcour depuis le prochain pt
}

// Determine le point depuis lequel on a trouve le plus petit groupe de tete pour un graphe donne,
// en utilisant le tableau des groupes de tetes obtenu depuis respectivement chaque point du graphe
static int groupe_de_tete_minimal(t_graphe* graphe, t_liste_simple_int* groupes_de_tete) {
    // Pour l'instant, on n'a pas encore de groupe de tete minimal
    int minimal_i;
    // On choisi une taille minimale volontairement impossible a atteindre, comme ca le 1er point
    // non supprime sera forcement designe pour le nouveau groupe de tete minimal
    int taille_minimale = graphe->nb_points + 1;

    log_ligne(module, "Groupe de tete en partant du point 0 :");
    log_t_liste_simple_int(module, groupes_de_tete[0]);

    // On verifie pour chaque point si son groupe de tete ne serait pas plus petit
    for (int point_i = 0; point_i < graphe->nb_points; point_i++) {
        // Si le point a ete supprime, alors aucun groupe de tete ne lui a ete assigne
        // On doit donc l'ignorer
        if (graphe->points[point_i].candidat_id == POINT_SUPPRIME)
            continue;

        log_ligne(module, "Groupe de tete en partant du point %d :", point_i);
        log_t_liste_simple_int(module, groupes_de_tete[point_i]);

        // Taille du groupe courant
        int taille_groupe_i = groupes_de_tete[point_i].taille;
        
        // Sera forcement vrai la 1ere fois puisque la taille maximal d'un group de tete
        // est nb_points
        if (taille_groupe_i < taille_minimale) { // On a un nouveau groupe minimal
            minimal_i = point_i;
            taille_minimale = taille_groupe_i;
        }
    }

    log_ligne(module, "Groupe de tete minimal, obtenu en partant du point %d :", minimal_i);
    log_t_liste_simple_int(module, groupes_de_tete[minimal_i]);

    return minimal_i;
}

// Supprime dans chaque point du graphe les references aux arcs avec les IDs donnees
static void supprimer_refs_arc_dans_graphe(t_graphe* duels, t_liste_simple_int ids_arcs_supprimes) {
    // Pour chaque point du graphe
    for (int point_i = 0; point_i < duels->nb_points; point_i++) {
        t_point* pt_courant = &(duels->points[point_i]);

        // Si le point n'est plus dans le graphe, on l'ignore
        if (pt_courant->candidat_id == POINT_SUPPRIME)
            continue;

        // Pour chaque arc supprime
        for (t_cellule_simple_int* cell = ids_arcs_supprimes.elems; cell != NULL; cell = cell->suiv) {
            int arc_id = cell->val; // ID de l'arc perime

            // On essaye de supprime la ref de l'arc
            if (supprimer_valeur_t_liste_simple_int(&(pt_courant->defaites_contre), arc_id))
                log_ligne(module, "Arc %d supprime pour le point %d", arc_id, point_i);
        }
    }
}

// Elimine tous les points du graphe ne faisant pas partie de l'ensemble donne
// en mettant leur ID candidat a une valeur negative et en supprimant les arcs
// y faisant reference.
// On indiquera en valeur de sortie les IDs des arcs faisant references a des points
// supprimes du graphe.
static void supprimer_points_graphe(
    t_graphe* duels, t_liste_simple_int points_gardes,
    t_liste_simple_int* refs_arcs_perimees)
{
    // On initialise la valeur en sortie permettant de savoir quels arcs ont doivent
    // etre supprimes
    creer_t_liste_simple_int(refs_arcs_perimees);

    // Pour chaque point du graphe representant son candidat
    for (int candidat_id = 0; candidat_id < duels->nb_points; candidat_id++) {
        // Point du candidat courant
        t_point* pt_courant = &(duels->points[candidat_id]);

        // Si le candidat est deja elimine du graphe, on peut l'ignorer
        if (pt_courant->candidat_id == POINT_SUPPRIME)
            continue;

        // Si le candidat est dans le groupe de tete, on ne procede pas a son
        // elimination
        if (valeur_existe_t_liste_simple_int(points_gardes, candidat_id))
            continue;

        log_ligne(module, "Suppresion du point %d", candidat_id);

        // On marque le point comme appartenant a un candidat eliminie du graphe
        pt_courant->candidat_id = POINT_SUPPRIME;
        // Pour chaque arc qui peut etre dans le graphe (ou supprime du graphe)
        for (int arc_id = 0; arc_id < duels->arcs_initiaux.nb; arc_id++) {
            t_arc_p* arc_courant = &(duels->arcs_initiaux.elems[arc_id]);

            // S'il fait reference au point supprime, on l'ajoute a la liste des arcs
            // perimes s'il n'y est pas deja
            if (arc_courant->orig == candidat_id || arc_courant->dest == candidat_id)
                if (!valeur_existe_t_liste_simple_int(*refs_arcs_perimees, arc_id))
                    inserer_debut_t_liste_simple_int(refs_arcs_perimees, arc_id);
        }
    }
}

// Mets en sortie la liste des IDs des arcs encore references par au moins un
// point dans le graphe
static void arcs_encore_references_du_graphe(t_graphe* duels, t_liste_simple_int* arcs_encore_references) {
    // On initialise la liste vide des arcs pour y ajouter les arcs encore presents par la suite
    creer_t_liste_simple_int(arcs_encore_references);

    // Pour chaque point du graphe
    for (int point_i = 0; point_i < duels->nb_points; point_i++) {
        t_point* pt_courant = &(duels->points[point_i]);

        // On ignore les points supprimes du graphe
        if (pt_courant->candidat_id == POINT_SUPPRIME)
            continue;

        // Pour chaque arc reference jusqu'a la fin de la liste
        t_cellule_simple_int* premiere_ref = pt_courant->defaites_contre.elems;
        for (t_cellule_simple_int* cell = premiere_ref; cell != NULL; cell = cell->suiv) {
            int arc_id = cell->val;

            // On ajoute l'ID de l'arc a la liste, si ce n'est pas deja fait
            if (!valeur_existe_t_liste_simple_int(*arcs_encore_references, arc_id))
                inserer_debut_t_liste_simple_int(arcs_encore_references, arc_id);
        }
    }

    log_ligne(module, "IDs des arcs encore presents dans le graphe :");
    log_t_liste_simple_int(module, *arcs_encore_references);
}

// Mets en sortie la liste les arcs ayant le poids minimal de tous les arcs du
// graphe, ou une liste vide s'il n'y a plus aucun arcs references
static void arcs_poids_minimal_du_graphe(t_graphe* duels, t_liste_simple_int* arcs_poids_minimal) {
    // On commence par une liste vide des arcs ayant le poids minimal
    creer_t_liste_simple_int(arcs_poids_minimal);

    // On filtre pour ne chercher le poids minimal que parmis les arcs encore dans
    // le graphe
    t_liste_simple_int arcs_verifies;
    arcs_encore_references_du_graphe(duels, &arcs_verifies);

    // S'il n'y a plus aucun arc dans le graphe, alors la taille vaudra 0 et on
    // gardera la liste des arcs au poids minimal vide
    if (arcs_verifies.taille != 0) {
        // On commence par considerer le 1er arc comme ayant le poids minimal
        t_cellule_simple_int* premiere_cell = arcs_verifies.elems;
        // On obtient le 1er arc a partir de l'ID du 1er arc encore dans le graphe
        int premier_arc_id = premiere_cell->val;
        t_arc_p* premier_arc = &(duels->arcs_initiaux.elems[premier_arc_id]);

        int poids_minimal = premier_arc->poids;
        inserer_debut_t_liste_simple_int(arcs_poids_minimal, premier_arc_id);

        // Ensuite pour chacun des autres arcs encore dans le graphe (on parcours
        // la liste a partir de la 2eme cell)
        for (t_cellule_simple_int* cell = premiere_cell->suiv; cell != NULL; cell = cell->suiv) {
            int id_arc_courant = cell->val;
            t_arc_p* arc_courant = &(duels->arcs_initiaux.elems[id_arc_courant]);
            int poids_arc_courant = arc_courant->poids;

            // Si on a un nouveau poids minimal parmis les arcs du graphe
            if (poids_arc_courant < poids_minimal) {
                // On modifie le poids minimal connu parmis les arcs
                poids_minimal = poids_arc_courant;
                // On reinitialise la liste des arcs possedant ce nouveau poids minimal
                detruire_t_liste_simple_int(arcs_poids_minimal);
                // On y met l'ID de l'arc au poids minimal qu'on vient de decouvrir
                inserer_debut_t_liste_simple_int(arcs_poids_minimal, id_arc_courant);
            } else if (poids_arc_courant == poids_minimal) {
                // Sinon, si on a un nouvel arc avec le poids minimal du graphe,
                // on l'ajoute a la liste des arcs ayant le poids minimal
                inserer_debut_t_liste_simple_int(arcs_poids_minimal, id_arc_courant);
            }
        }

        log_ligne(module, "Le poids minimal des arcs du graphe est %d", poids_minimal);
    }

    log_ligne(module, "Arcs presents dans le graphe ayant le poids minimal :");
    log_t_liste_simple_int(module, *arcs_poids_minimal);

    // Dans tous les cas, on n'oublie la liberation des ressources utilisees pour
    // cette operation
    detruire_t_liste_simple_int(&arcs_verifies);
}


// Fais une iteration de l'algorithme de Schulze sur le graphe, cad garde seulement
// le groupe de tete minimal et renvoi l'ID du vainqueur OU supprime les arcs de
// plus faible ponderation et renvoi que l'algorithme n'est pas fini.
static int iteration_schulze_graphe(t_graphe* duels) {
    // On aura autant de groupes de tete que de points depuis lesquels on part
    t_liste_simple_int* groupes_de_tete = (t_liste_simple_int*)
        malloc(duels->nb_points * sizeof(t_liste_simple_int));

    // Pour chaque point duquel on part, on va determiner un groupe de tete
    // du graphe
    for (int point_i = 0; point_i < duels->nb_points; point_i++) {
        // Groupe de tete determine en partant du pt courant
        t_liste_simple_int* groupe_de_tete_i = &(groupes_de_tete[point_i]);
        // Point depuis lequel on part
        t_point* pt_courant = &(duels->points[point_i]);

        // On ignore les points supprimes du graphe, en signalant qu'il faut aussi les ignorer
        // lorsqu'on recherchera le groupe de tete minimal
        if (pt_courant->candidat_id == POINT_SUPPRIME)
            continue;

        log_ligne(module, "Parcours du graphe depuis le point %d du candidat %d",
                  point_i, pt_courant->candidat_id);

        // On commence par creer un groupe de tete initial vide
        creer_t_liste_simple_int(groupe_de_tete_i);
        // Puis on le rempli on parcourant recursivement le graphe
        groupe_de_tete(duels, groupe_de_tete_i, pt_courant);
    }

    // L'ensemble de Schwartz qui va etre conserver pour la prochaine iteration
    // de l'algorithme
    log_ligne(module, "Recherche de l'ensemble de Schwartz...");
    int ensemble_schwartz_i = groupe_de_tete_minimal(duels, groupes_de_tete);
    // Pour stocker les arcs associes aux pts qui vont etre supprimes
    t_liste_simple_int arcs_perimes;
    // On supprime tous les points ne figurant pas dans l'ensemble de Schwartz
    log_ligne(module, "Suppression des points hors de l'ensemble de Schwartz...");
    supprimer_points_graphe(duels, groupes_de_tete[ensemble_schwartz_i], &arcs_perimes);
    // On supprime les arcs relies aux pts supprimes
    log_ligne(module, "Suppression des arcs relies aux points hors de l'ensemble de Schwartz...");
    supprimer_refs_arc_dans_graphe(duels, arcs_perimes);

    // On fait la liste des arcs toujours dans le graphe apres sa modification
    t_liste_simple_int arcs_encore_presents;
    log_ligne(module, "Recherche des arcs encore presents dans le graphe...");
    arcs_encore_references_du_graphe(duels, &arcs_encore_presents);
    // S'il n'y en a plus, alors le point restant determine notre vainqueur
    // de Schulze
    int vainqueur;
    if (arcs_encore_presents.taille == 0) {
        log_ligne(module, "Plus aucun arc dans le graphe, methode de Schulze terminee.");
        // Notre vainqueur est donc le candidat represente par le 1er, seul pt
        // restant du graphe
        vainqueur = duels->points[0].candidat_id;
    } else {
        log_ligne(module, "Pas encore de vainqueur de Schulze, la methode continue.");
        // On ne sait toujours pas qui est le vainqueur
        vainqueur = SCHULZE_AUCUN_VAINQUEUR;

        // Pour assurer la terminaison de notre algorithme et garantir un groupe
        // de tete plus petit a la prochaine iteration, on detruit les defaites
        // la plus serrees de graphe
        t_liste_simple_int ids_arcs_poids_minimal;
        log_ligne(module, "Recherche des arcs ayant le poids minimal du graphe...");
        arcs_poids_minimal_du_graphe(duels, &ids_arcs_poids_minimal);

        // On supprime les defaites les plus serres pour reduire le prochain
        // groupe de tete minimal
        log_ligne(module, "Suppression du graphe ayant le poids minimal du graphe...");
        supprimer_refs_arc_dans_graphe(duels, ids_arcs_poids_minimal);

        // On detruit les ressources alloues a cette recherche des poids minimaux
        detruire_t_liste_simple_int(&ids_arcs_poids_minimal);
    }

    // On desalloue en memoire chaque groupe de tete
    for (int groupe_i = 0; groupe_i < duels->nb_points; groupe_i++)
        detruire_t_liste_simple_int(&(groupes_de_tete[groupe_i]));

    // On desalloue en memoire le tableau des groupes de tete
    free(groupes_de_tete);
    // On desalloue toutes les ressources utilisees temporairement pour cette
    // iteraation de l'algo
    detruire_t_liste_simple_int(&arcs_perimes);
    detruire_t_liste_simple_int(&arcs_encore_presents);

    return vainqueur;
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

int condorcet_schulze(t_mat_int_dyn mat_duels, t_candidats candidats) {
    log_ligne(module, "Methode de condorcet, departagee avec schulze sur :");
    log_t_mat_int_dyn(module, mat_duels);

    // On essaie de trouver un vainqueur de condorcet
    int vainqueur = vainqueur_condorcet(mat_duels);

    // S'il y en a un, inutile d'utiliser minimax pour departager les
    // candidats
    if (vainqueur != CONDORCET_AUCUN_VAINQUEUR) {
        log_ligne(module, "Vainqueur de condorcet : %d", vainqueur);
        return vainqueur;
    }

    // Si on n'a pas de vainqueur de condorcet, on doit utiliser la methode de Schulze
    // On va donc devoir creer un graphe pour representer les duels
    t_graphe graphe_duels;
    log_ligne(module, "Creation du graphe a partir de la matrice...");
    creer_graphe_duels(candidats, mat_duels, &graphe_duels);

    // Pour l'instant, Schulze n'a pas fini de s'executer sur ce graphe
    vainqueur = SCHULZE_AUCUN_VAINQUEUR;
    // Tant que le graphe n'a pas ete completement traite
    while (vainqueur == SCHULZE_AUCUN_VAINQUEUR)
        vainqueur = iteration_schulze_graphe(&graphe_duels);
    // On detruit le graphe quand on a termine le traitement de Schulze
    detruire_graphe_duels(&graphe_duels);

    // Enfin, on donne le vainqueur determine par Schulze
    log_ligne(module, "Vainqueur departage par Schulze : %d", vainqueur);
    return vainqueur;
}
