#include <unitaires/condorcet_schulze.h>

#include <condorcet_schulze.h>
#include <assert.h>

// Nb de candidats dans les tests unitaires de ce module
#define NB_CANDIDATS_TESTS 4
// Nb de duels effectues, et donc d'arcs initiaux attendus dans le graphe
#define NB_ARCS_ATTENDUS ((NB_CANDIDATS_TESTS * (NB_CANDIDATS_TESTS - 1)) / 2)


// BDD des candidats dans le cadre de ces tests, il y aura des egalites
// donc cette BDD est necessaire pour determiner le vainqueur

static t_candidat elems_candidats[NB_CANDIDATS_TESTS] = {
    { "B", 0 },
    { "H", 1 },
    { "S", 2 },
    { "L", 3 }
};

static t_candidats candidats = { elems_candidats, NB_CANDIDATS_TESTS };


// Initialise dynamiquement la SDD matrice donnee avec le nb de candidats
// et en copiant le contenu de la matrice `duels`
static void creer_mat_duels_compatible(int duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS], t_mat_int_dyn* mat) {
    creer_t_mat_int_dyn(mat, NB_CANDIDATS_TESTS);

    for (int l = 0; l < NB_CANDIDATS_TESTS; l++)
        for (int c = 0; c < NB_CANDIDATS_TESTS; c++)
            mat->elems[l][c] = duels[l][c];
}

// Verifier qu'une liste donnee contient les elements attendus dans le bon ordre
static void verifier_elements_liste(t_liste_simple_int liste, t_tab_int_dyn attendus) {
    assert(liste.taille == attendus.taille); // On verifie le nb d'elements

    // On verifie les elements eux-memes, cellule d'element par cellule d'element
    t_cellule_simple_int* cell_courante = liste.elems;
    for (int i = 0; i < liste.taille; i++) { // Tant que toute la liste n'a pas ete parcourue
        // On verifie que l'element a cette position contient le meme element que le
        // tab des elements attendus au meme endroit
        assert(cell_courante->val == attendus.elems[i]);

        cell_courante = cell_courante->suiv; // On passe a la cellule suivante
    }
}


//
// creer_graphe_duels
//

void creer_graphe_duels_test() {
    // Creation de la matrice de duels qui sera fournie en entree
    int elems_mat_duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS] = {
        {  0, 87, 33, 42 },     // ID des arcs   {  ,  ,  ,  }
        { 13,  0,  1, 50 },     // contenant les { 0,  ,  ,  }
        { 67, 99,  0, 25 },     // infos de      { 1, 2,  ,  }
        { 58, 50, 75,  0 }      // chaque duel : { 3, 4, 5,  }
    };
    t_mat_int_dyn mat_duels;
    creer_mat_duels_compatible(elems_mat_duels, &mat_duels);

    // On genere le graphe a l'aide des duels crees et de la BDD des candidats
    t_graphe graphe_duels;
    creer_graphe_duels(candidats, mat_duels, &graphe_duels);

    // On commence par verifier les points du graphe

    assert(graphe_duels.nb_points == NB_CANDIDATS_TESTS); // Autant de pts que de candidats

    // On s'assure que chaque candidat a un point dont l'index dans le graphe est
    // egal a l'ID du candidat qu'il represente
    for (int candidat_id = 0; candidat_id < NB_CANDIDATS_TESTS; candidat_id++)
        assert(graphe_duels.points[candidat_id].candidat_id == candidat_id);

    // Le candidat 0 a perdu les 4eme et 2eme duels dans la matrice de duels, donc
    // ses arcs perdants auront les ID respectifs 3 et 1 dans le tableau des arcs
    // initiaux du graphe (on rappel que 3 vient avant 1 car les elements sont inseres
    // en debut de liste)
    int elems_defaites_de_0[2] = { 3, 1 };
    t_tab_int_dyn defaites_de_0 = { elems_defaites_de_0, 2 };
    // On verifie que les defaites du candidat 0 sont bien les 2eme et 3eme duels
    // dans la matrice
    verifier_elements_liste(graphe_duels.points[0].defaites_contre, defaites_de_0);

    // Le candidat 1 a perdu les duels 2 et 0, il est plus age que le
    // candidat 3 car H est plus age que L, donc il gagne le duel 4 avec
    // un score de 50 %
    int elems_defaites_de_1[2] = { 2, 0 };
    t_tab_int_dyn defaites_de_1 = { elems_defaites_de_1, 2 };
    verifier_elements_liste(graphe_duels.points[1].defaites_contre, defaites_de_1);

    // Le candidat 2 a perdu le duel 5
    int elems_defaites_de_2[1] = { 5 };
    t_tab_int_dyn defaites_de_2 = { elems_defaites_de_2, 1 };
    verifier_elements_liste(graphe_duels.points[2].defaites_contre, defaites_de_2);

    // Le candidat 3 a perdu le duel 4
    int elems_defaites_de_3[1] = { 4 };
    t_tab_int_dyn defaites_de_3 = { elems_defaites_de_3, 1 };
    verifier_elements_liste(graphe_duels.points[3].defaites_contre, defaites_de_3);

    // Ensuite on verifie que la BDD des arcs initiaux dans le graphe contient tous
    // les arcs attendus dans le bon ordre

    t_arc_p arcs_attendus[NB_ARCS_ATTENDUS] = {
        // { ID_candidat_origine, ID_candidat_destination, Difference_score }
        { 0, 1, 87 - 13 },
        { 2, 0, 67 - 33 },
        { 2, 1, 99 - 1  },
        { 3, 0, 58 - 42 },
        { 1, 3, 50 - 50 },
        { 3, 2, 75 - 25 }
    };

    // On verifie qu'on a le bon nb d'arcs initialement presents dans le graphe
    assert(graphe_duels.arcs_initiaux.nb == NB_ARCS_ATTENDUS);
    // On verifie le contenu de chaque arc present
    for (int arc_id = 0; arc_id < NB_ARCS_ATTENDUS; arc_id++) {
        t_arc_p* arc_courant = &(graphe_duels.arcs_initiaux.elems[arc_id]);
        t_arc_p* arc_attendu = &(arcs_attendus[arc_id]);

        assert(arc_courant->orig == arc_attendu->orig);
        assert(arc_courant->dest == arc_attendu->dest);
        assert(arc_courant->poids == arc_attendu->poids);
    }

    // On desalloue les ressources du test
    detruire_graphe_duels(&graphe_duels);
    detruire_t_mat_int_dyn(&mat_duels);
}


//
// condorcet_schulze
//

// Un candidat gagne tous ses duels
void condorcet_schulze_vainqueur_condorcet() {
    int elems_duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS] = {
        {  0, 13, 54, 68 },
        { 87,  0, 63, 50 }, // Candidat 1 = H qui est plus age que...
        { 46, 37,  0,  1 },
        { 32, 50, 99,  0 }  // ...candidat 3 = L, donc candidat H l'emporte
    };
    // On creer notre matrice de duels ou le candidat 1 gagne tous ses duels
    t_mat_int_dyn duels;
    creer_mat_duels_compatible(elems_duels, &duels);

    // On a un vainqueur de condorcet 1
    assert(condorcet_schulze(duels, candidats) == 1);

    detruire_t_mat_int_dyn(&duels);
}

// Aucun candidat ne gagne tous ses duels, on applique la methode de Schulze
void condorcet_schulze_vainqueur_schulze() {
    int elems_duels[NB_CANDIDATS_TESTS][NB_CANDIDATS_TESTS] = {
        // B,  H,  S,  L
        {  0, 78, 50, 39 }, // Candidat 0 = B, B est plus age que S, B l'emporte
        { 12,  0, 51, 54 }, // Candidat 1 = H
        { 50, 49,  0,  1 }, // Candidat 2 = S
        { 61, 46, 99,  0 }  // Candidat 3 = L
    };
    // On creer notre matrice de duels
    t_mat_int_dyn duels;
    creer_mat_duels_compatible(elems_duels, &duels);

    // Avec notre matrice duels, on va obtenir le graphe compose des arc :
    // B --64--> H
    // H ---8--> L
    // L --22--> B
    // B ---0--> S
    // H ---2--> S
    // L --98--> S
    //
    // Aucun des pts L, B, ou H n'est pas battu par autre chose que L, B ou H
    // On trouve notre ensemble de Schwartz = { L, B, H }
    // 
    // On supprime l'arc de + faible ponderation H --8--> L
    // On se retrouve avec un pt L vers lequel aucun arc ne mene, il constitue
    // alors le singleton { L } qui est le nouvel ensemble de Schwartz.
    //
    // On a notre vainqueur de Schulze L, represente par le candidat 3

    // On a un vainqueur de condorcet 1
    assert(condorcet_schulze(duels, candidats) == 3);

    detruire_t_mat_int_dyn(&duels);
}


// Script principal executant tous les tests de ce module
void tests_unitaires_condorcet_schulze() {
    creer_graphe_duels_test();

    condorcet_schulze_vainqueur_condorcet();
    condorcet_schulze_vainqueur_schulze();
}
