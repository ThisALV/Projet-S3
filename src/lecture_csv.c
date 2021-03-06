#include <lecture_csv.h>

#include <string.h>
#include <stdlib.h>
#include <erreur.h>
#include <limits.h>

// Selon les exemples fournis par Moodle, aucun CSV n'aura une lignes
// de plus de 1024 colonnes
#define LIGNE_MAX_TAILLE 1024
// Nombre de colonnes prefixes avec les colonnes des candidats dans un fichier CSV au
// format ballots de votes
#define BALLOTS_COLS_PREFIXE 4


// Fonction privee qui verifie la presence d'au moins une ligne dans une matrice CSV.
// S'il n'y a aucune ligne alors le tableau de candidats passe en mode erreur.
// `true` est retourne si une en-tete etait bien presente
static bool verifier_entete(t_mat_char_star_dyn mots_csv, t_candidats* candidats) {
    bool entete_presente = mots_csv.lignes > 0; // La premiere ligne est l'en-tete

    if (!entete_presente)
        mettre_t_candidats_erreur(candidats);

    // On informe l'appelant si l'en-tete etait bien presente
    return entete_presente;
}

// Ajoute a la ligne ligne_i, contenant deja nb_colonnes colonnnes,
// de la matrice mat une colonne contenant la chaine allouee dynamiquement mot.
// Retourne le nombre de colonnes apres l'operation (nb_colonnes + 1).
static int ajouter_colonne(t_mat_char_star_dyn* mat, int ligne_i, int nb_colonnes, char* mot) {
    // On incremente le compteur de colonne et on retient l'indice de celle
    // sur laquelle on travaille
    int colonne_i = nb_colonnes++;

    // On alloue la memoire pour une nouvelle colonne de cette ligne
    mat->elems[ligne_i] = (char**) realloc(mat->elems[ligne_i], nb_colonnes * sizeof(char*));
    char** ligne_mat = mat->elems[ligne_i];
    verifier_alloc(ligne_mat, "Allocation ligne CSV");

    // On alloue dans cette dans colonne un tableau de caracteres suffisament grand
    // pour contenir le prochain mot de la ligne ET un caractere de terminaison \0
    ligne_mat[colonne_i] = mot;

    return nb_colonnes; // Une colonne a ete ajoutee par l'operation
}


char* allouer_copie_char_star(char* source, char* message_si_erreur) {
    // On evalue la taille de la chaine pour allouer la place necessaire en memoire
    int taille_nom = strlen(source);
    // +1 pour le 0 de terminaison
    char* copie = (char*) malloc(sizeof(char) * (taille_nom + 1));
    verifier_alloc(copie, message_si_erreur);

    // On effectue la copie puis on renvoie le resultat
    strcpy(copie, source);
    return copie;
}

void lire_fichier_votes(FILE* fichier_csv, char* separateurs, t_mat_char_star_dyn* mots) {
    // Doit etre remplie de 0 pour s'assurer qu'il n'y ai pas de separateur de la partie
    // inutilisee de la ligne
    char ligne[LIGNE_MAX_TAILLE] = { 0 };

    mots->lignes = 0;
    bool nb_colonnes_connu = false;
    // Vaut NULL au debut, donc le premier realloc fera un malloc
    mots->elems = NULL;
    // On lit ligne par ligne le fichier CSV en les stockant tour a tour
    // dans le buffer, jusqu'a ce qu'on ne puisse plus lire une ligne complete
    while (fscanf(fichier_csv, "%[^\n]%*c", ligne) == 1) {
        // On incremente le compteur de lignes et on retient la ligne
        // courante
        int ligne_i = mots->lignes++;

        // On alloue une nouvelle case dans le tableau des lignes
        // pour contenir le pointeur vers la memoire de la nouvelle ligne
        mots->elems = (char***) realloc(mots->elems, mots->lignes * sizeof(char**));
        verifier_alloc(mots->elems, "Allocation matrice CSV");

        // On met la memoire de la nouvelle ligne a NULL, comme
        // ca au premier realloc qui devra lui donner de la memoire
        // fera un malloc
        mots->elems[ligne_i] = NULL;

        char* mot = ligne; // On commence avec la ligne complete
        mot = strtok(mot, separateurs); // On la decoupe avec les separateurs

        // On initialise le compteur de colonnes de cette ligne
        int colonnes = 0;
        // Pour chaque mot dans la ligne
        while (mot != NULL) {
            // Le mot de la colonne est dans un buffer, il doit donc etre copie
            // pour etre sauvegarde
            char* copie_mot = allouer_copie_char_star(mot, "Copie mot colonne CSV");

            colonnes = ajouter_colonne(mots, ligne_i, colonnes, copie_mot);

            // Enfin, on passe au mot suivant
            mot = strtok(NULL, separateurs);
        }

        if (nb_colonnes_connu) {
            // On verifie qu'il y a le mm nb de colonnes dans chaque ligne

            // S'il y a trop de colonnes, alors on ne peut pas regler le probleme
            if (colonnes > mots->colonnes)
                erreur_fatale(2, "Nombre de colonnes non persistent dans CSV");

            // En revanche s'il manque des colonnes, on peut en ajouter des "-1"
            // qui seront ignores pour pallier a ca et avoir une matrice de mots valide
            while (colonnes < mots->colonnes) {
                // Chaque colonne doit avoir sa chaine en memoire pour la free() a la destruction
                // de la matrice
                char* copie_mot_remplissage = allouer_copie_char_star("-1", "Copie mot colonne CSV");

                colonnes = ajouter_colonne(mots, ligne_i, colonnes, copie_mot_remplissage);
            }
        } else { 
            mots->colonnes = colonnes;
            nb_colonnes_connu = true;
        }
    }
}

void convertir_mat_duels(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels) {
    // En excluant l'en-tete, la matrice doit etre carree
    if (mots_csv.lignes != (mots_csv.colonnes + 1)) {
        mettre_t_mat_int_dyn_erreur(duels);
        return;
    }

    // On creer la matrice carree de meme taille sans l'en-tete que celle des mots CSV
    if (!creer_t_mat_int_dyn(duels, mots_csv.lignes - 1)) {
        mettre_t_mat_int_dyn_erreur(duels);
        return;
    }

    // Pour chaque ligne, en commencant par l'indince 1 pour ignorer l'en-tete
    for (int i = 1; i < mots_csv.lignes; i++) {
        int ligne_duels = i - 1; // Decalage de 1 ligne a cause de l'en-tete

        for (int j = 0; j < mots_csv.colonnes; j++) {
            int score = atoi(mots_csv.elems[i][j]);

            // Si on a une valeur 0 qui n'est pas sur la diagonale, alors la matrice
            // est mal formee ou une case n'a pas pu etre lue
            if (score == 0 && j != (i - 1)) {
                // Une alloc a deja ete faite, c'est donc une erreur fatale
                erreur_fatale(ERR_INTERNE, "Matrice duels malformee");
            }

            duels->elems[ligne_duels][j] = score; // On enregistre le score numerique dans les duels
        }
    }
}

void obtenir_candidats_duels(t_mat_char_star_dyn mots_csv, t_candidats* candidats) {
    // On verifie qu'il y ait au moins une en-tete dans le CSV
    if (!verifier_entete(mots_csv, candidats))
        return;

    // On creer le tableau des candidats, sachant qu'il y a 1 candidant par colonne CSV
    if (!creer_t_candidats_dyn(candidats, mots_csv.colonnes)) {
        mettre_t_candidats_erreur(candidats);
        return;
    }

    // Pour chaque colonne, dans l'en-tete, on copie le contenu de la case pour placer
    // le nom dans le tableau, et on affecte l'ID qu'on incremente de 1 pour qu'il soit
    // unique chez chaque candidat
    for (int id = 0; id < mots_csv.colonnes; id++) {
        t_candidat* c_courant = (candidats->elems) + id;

        // On alloue la memoire necessaire puis on copie le nom
        c_courant->nom = allouer_copie_char_star(mots_csv.elems[0][id], "Allocation nom candidat");
        c_courant->id = id;
    }
}

void obtenir_candidats_ballots(t_mat_char_star_dyn mots_csv, t_candidats* candidats) {
    // On verifie qu'il y ait au moins une en-tete dans le CSV
    if (!verifier_entete(mots_csv, candidats))
        return;

    // On creer le tableau des candidats, sachant que les 4 premieres colonnes ne contiennent
    // pas un candidat
    int nb_candidats = mots_csv.colonnes - BALLOTS_COLS_PREFIXE;
    if (!creer_t_candidats_dyn(candidats, nb_candidats)) {
        mettre_t_candidats_erreur(candidats);
        return;
    }

    for (int col = 0; col < nb_candidats; col++) {
        t_candidat* c_courant = (candidats->elems) + col;
        
        // On copie l'en-tete a la colonne courante en ignorant les 4 premiers colonnes
        // Ce numero de colonne courante sera l'ID du candidat
        c_courant->nom = allouer_copie_char_star(
            mots_csv.elems[0][BALLOTS_COLS_PREFIXE + col], "Allocation nom candidat");
        c_courant->id = col;
    }
}

void creer_mat_duels_absolue(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels, int* nb_electeurs) {
    // On calcul le nombre d'electeur pour savoir quand s'arreter et pour calculer
    // les % de voix plus tard
    *nb_electeurs = mots_csv.lignes - 1;
    // On calcul le nombre de colonnes reservees aux candidats
    int nb_candidats = mots_csv.colonnes - BALLOTS_COLS_PREFIXE;

    for (int electeur_i = 0; electeur_i < *nb_electeurs; electeur_i++) {
        // On travaille en bas de la diagonale pour eviter d'evaluer 2 fois les
        // memes duels
        for (int candidat1_id = 0; candidat1_id < nb_candidats; candidat1_id++) {
            int rang_candidat1 = atoi(
                mots_csv.elems[1 + electeur_i][BALLOTS_COLS_PREFIXE + candidat1_id]);

            // Si un score dans un ballot de vote n'est pas > 0, c'est soit qu'il n'a
            // pas pu etre lu, soit qu'il est invalide
            if (rang_candidat1 <= 0)
                continue;

            for (int candidat2_id = 0; candidat2_id < candidat1_id; candidat2_id++) {
                int rang_candidat2 = atoi(
                    mots_csv.elems[1 + electeur_i][BALLOTS_COLS_PREFIXE + candidat2_id]);

                // Meme verification que pour rang_candidat1
                if (rang_candidat2 > 0) {
                    // Le candidat ayant le plus petit rang (le candidat prefere a l'autre)
                    // voit son compteur de votes etre incremente dans la matrice des duels
                    if (rang_candidat1 < rang_candidat2)
                        duels->elems[candidat1_id][candidat2_id]++;
                    else if (rang_candidat2 < rang_candidat1)
                        duels->elems[candidat2_id][candidat1_id]++;
                }
            }
        }
    }
}

void convertir_mat_duels_relative(t_mat_int_dyn* duels, int nb_electeurs) {
    // Il y a autant de ligne/colonne que de candidats
    int nb_candidats = duels->dim;

    for (int candidat1_id = 0; candidat1_id < nb_candidats; candidat1_id++) {
        for (int candidat2_id = 0; candidat2_id < nb_candidats; candidat2_id++) {
            int nb_victoires = duels->elems[candidat1_id][candidat2_id];
            // Chaque duel a lieu 1 fois par ballot/par electeur
            int pourcentage_voix = ((double) nb_victoires) / nb_electeurs * 100;

            duels->elems[candidat1_id][candidat2_id] = pourcentage_voix;
        }
    }
}

void creer_mat_duels(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels) {
    // On verifie qu'il y ait au moins une en-tete et un electeur
    if (mots_csv.lignes < 2) {
        mettre_t_mat_int_dyn_erreur(duels);
        return;
    }

    // On calcul le nombre de colonnes reservees aux candidats
    int nb_candidats = mots_csv.colonnes - BALLOTS_COLS_PREFIXE;
    // On peut donc allouer une matrice de duels a la bonne dimension
    if (!creer_t_mat_int_dyn(duels, nb_candidats)) {
        mettre_t_mat_int_dyn_erreur(duels);
        return;
    }

    int nb_electeurs; // Nb d'electeurs trouves a l'analyse de la matrice CSV
    // On lit les ballots de vote
    creer_mat_duels_absolue(mots_csv, duels, &nb_electeurs);
    // On calcul l'integralite des pourcentages de voix
    convertir_mat_duels_relative(duels, nb_electeurs);
}

void premiers_de_ballot(t_tab_int_dyn ballot, t_tab_int_dyn* tetes) {
    // Un tableau de gagnants vide, comme ca le premier ajout avec realloc allouera une nouvelle zone memoire
    tetes->elems = NULL;
    tetes->taille = 0;

    // N'importe quel candidat avec un rang valide aura forcement un rang <=
    int meilleur_rang = INT_MAX;
    
    for (int id = 0; id < ballot.taille; id++) {
        int rang_courant = ballot.elems[id]; // On recupere le rang du prochain candidat dans sa colonne

        // Si un candidat a -1 en rang, c'est qu'il n'a pas pu etre lu donc on l'ignore
        if (rang_courant == -1)
            continue;

        if (rang_courant == meilleur_rang) {
            // Un autre candidat a fait un aussi bon rang dans ce ballot de votes
            tetes->taille++; // Un candidat de plus
            tetes->elems = (int*) realloc(tetes->elems, tetes->taille * sizeof(int)); // On lui alloue une colonne
            verifier_alloc(tetes->elems, "Allocation gagants de ballot");

            tetes->elems[tetes->taille - 1] = id; // On assigne le candidat a la nouvelle col
        } else if (rang_courant < meilleur_rang) {
            // Tous les candidats precedents ont un moins bon rang, ils ne sont plus gagnants
            // Ce candidat, en revenche, a le nouveau meilleur rang, il est gagnant
            tetes->taille = 1;
            tetes->elems = (int*) realloc(tetes->elems, sizeof(int));
            verifier_alloc(tetes->elems, "Allocation gagnants de ballot");

            tetes->elems[0] = id;

            // On met a jour le meilleur rang du ballot
            meilleur_rang = rang_courant;
        }
    }
}

t_tab_int_dyn* tetes_de_listes(t_mat_char_star_dyn mots_csv) {
    // On soustrait le nb de cols prefixes et on obtient le nb de cols pour les noms des candidats
    int nb_candidats = mots_csv.colonnes - BALLOTS_COLS_PREFIXE;
    // On soustrait l'en-tete et les lignes restantes sont des ballots de votes
    int nb_ballots = mots_csv.lignes - 1;

    // On verifie qu'il y ait au moins 1 candidat et un 1 electeur
    if (nb_candidats < 1 || nb_ballots < 1)
        return NULL; // En retournant NULL, on signale une erreur a l'appelant

    t_tab_int_dyn* candidats_preferes = NULL; // Comme ca le premier realloc fera un malloc
    // On parcours les ballots, cad les lignes une par une en sautant l'en-tete
    for (int ballot_i = 0; ballot_i < nb_ballots; ballot_i++) {
        // Un ballot contient un rang pour chaque participant
        t_tab_int_dyn ballot;
        creer_t_tab_int_dyn(&ballot, nb_candidats);

        // On va chercher la ligne csv qui contient le ballot courant
        // On se decale de 4 colonnes pour ignorer celles qui ne contient pas un rang de candidat
        char** ballot_csv = mots_csv.elems[ballot_i + 1] + BALLOTS_COLS_PREFIXE;
        for (int candidat_id = 0; candidat_id < nb_candidats; candidat_id++) {
            // On traduit le rang depuis le fichier CSV
            char* rang_csv = ballot_csv[candidat_id];
            int rang_lu = atoi(rang_csv);

            // Si le retour 0 mais la chaine ne vaut pas "0", alors ce 0 signifie qu'une erreur
            // de conversion char* -> int s'est produite
            bool erreur_lecture = rang_lu == 0 && (strcmp(rang_csv, "0") != 0);

            // Si le rang est negatif ou une erreur de lecture s'est produite, on demande d'ignorer
            // le rang de ce candidat dans ce ballot avec la valeur -1
            if (rang_lu < 0 || erreur_lecture) {
                ballot.elems[candidat_id] = -1;
            } else { // Sinon on peut affecter le rang effectivement lu
                ballot.elems[candidat_id] = rang_lu;
            }
        }

        // On evalue les gagnants avec les rangs numeriques de chaque candidat
        t_tab_int_dyn gagnants_ballot;
        premiers_de_ballot(ballot, &gagnants_ballot);

        // On ajoute une tableau de candidats gagnant pour ce ballot traite au tableau des ballots
        candidats_preferes = (t_tab_int_dyn*) realloc(candidats_preferes, (ballot_i + 1) * sizeof(t_tab_int_dyn));
        verifier_alloc(candidats_preferes, "Allocation tetes de listes");
        candidats_preferes[ballot_i] = gagnants_ballot;
    }

    return candidats_preferes;
}
