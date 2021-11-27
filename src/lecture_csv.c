#include <lecture_csv.h>

#include <string.h>
#include <stdlib.h>
#include <erreur.h>

// Selon les exemples fournis par Moodle, aucun CSV n'aura une lignes
// de plus de 1024 colonnes
#define LIGNE_MAX_TAILLE 1024
// Nombre de colonnes prefixes avec les colonnes des candidats dans un fichier CSV au
// format ballots de votes
#define BALLOTS_COLS_PREFIXE 4


// Fonction privee utilitaire permettant d'obtenir une copie de chaine de caractere
// allouee a la bonne taille en memoire
char* allouer_copie_char_star(char* source, char* message_si_erreur) {
    // On evalue la taille de la chaine pour allouer la place necessaire en memoire
    int taille_nom = strlen(source);
    char* copie = (char*) malloc(sizeof(char) * taille_nom);
    verifier_alloc(copie, message_si_erreur);

    // On effectue la copie puis on renvoie le resultat
    strcpy(copie, source);
    return copie;
}

// Fonction privee qui verifie la presence d'au moins une ligne dans une matrice CSV.
// S'il n'y a aucune ligne alors le tableau de candidats passe en mode erreur.
// `true` est retourne si une en-tete etait bien presente
bool verifier_entete(t_mat_char_star_dyn mots_csv, t_candidats* candidats) {
    bool entete_presente = mots_csv.lignes > 0; // La premiere ligne est l'en-tete

    // S'il n'y en a pas, on passe le tableau des candidats en mode erreur
    if (!entete_presente) {
        candidats->elems = NULL;
        candidats->nb = -1;
    }

    // On informe l'appelant si l'en-tete etait bien presente
    return entete_presente;
}


void lire_fichier_votes(FILE* fichier_csv, char* separateurs, t_mat_char_star_dyn* mots) {
    char ligne[LIGNE_MAX_TAILLE];

    mots->lignes = 0;
    mots->colonnes = 0;
    // Vaut NULL au debut, donc le premier realloc fera un malloc
    mots->elems = NULL;
    // On lit ligne par ligne le fichier CSV en les stockant tour a tour
    // dans le buffer
    while (fgets(ligne, LIGNE_MAX_TAILLE, fichier_csv) != NULL) {
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
        mots->colonnes = 0;
        // Pour chaque mot dans la ligne
        while (mot != NULL) {
            // On incremente le compteur de colonne et on retient l'indice de celle
            // sur laquelle on travaille
            int colonne_i = mots->colonnes++;

            // On alloue la memoire pour une nouvelle colonne de cette ligne
            mots->elems[ligne_i] = (char**) realloc(mots->elems[ligne_i], mots->colonnes * sizeof(char*));
            char** ligne_mat = mots->elems[ligne_i];
            verifier_alloc(ligne_mat, "Allocation ligne CSV");

            // On alloue dans cette dans colonne un tableau de caracteres suffisament grand
            // pour contenir le prochainn mot de la ligne
            ligne_mat[colonne_i] = (char*) malloc(strlen(mot) * sizeof(char));
            verifier_alloc(ligne_mat[colonne_i], "Allocation mot CSV");

            // Et on copie le mot dans la matrice
            strcpy(ligne_mat[colonne_i], mot);

            // Enfin, passe au mot suivant
            mot = strtok(NULL, separateurs);
        }

        char* dernier_mot = mots->elems[ligne_i][mots->colonnes - 1];
        // On remplace supprime le saut de ligne \n en avancant la fin de la chaine
        // de caracteres
        dernier_mot[strlen(dernier_mot) - 1] = '\0';
    }
}

void convertir_mat_duels(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels) {
    // En excluant l'en-tete, la matrice doit etre carree
    if (mots_csv.lignes != (mots_csv.colonnes + 1)) {
        duels->elems = NULL;
        duels->dim = -1;

        return;
    }

    // On creer la matrice carree de meme taille sans l'en-tete que celle des mots CSV
    if (!creer_t_mat_int_dyn(duels, mots_csv.lignes - 1)) {
        // On traite l'erreur en cas de dimension invalide pour la matrice d'entiers
        duels->elems = NULL;
        duels->dim = -1;

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
        candidats->elems = NULL;
        candidats->nb = -1;

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
        // Si nb candidat n'est pas strictement positif, on a pas assez de colonnes, donc
        // la creation du tableau echoue
        candidats->elems = NULL;
        candidats->nb = -1;

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

bool creer_mat_duels_absolue(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels, int* nb_electeurs) {
    // On calcul le nombre d'electeur pour savoir quand s'arreter pour calculer
    // les % de voix plus tard
    *nb_electeurs = mots_csv.lignes - 1;
    // On calcul le nombre de colonnes reservees aux candidats
    int nb_candidats = mots_csv.colonnes - BALLOTS_COLS_PREFIXE;

    bool erreurs = false; // Passe a true si au moins une erreur non fatale est signalee
    for (int electeur_i = 0; electeur_i < nb_electeurs; electeur_i++) {
        // On travaille en bas de la diagonale
        for (int candidat1_id = 0; candidat1_id < nb_candidats; candidat1_id++) {
            // Ainsi on evite d'evaluer 2 fois les memes duels ou d'evaluer les duels
            // entre un seul candidat (diagonale)
            for (int candidat2_id = 0; candidat2_id < candidat1_id; candidat2_id++) {
                int rang_candidat1 = atoi(
                    mots_csv.elems[1 + electeur_i][BALLOTS_COLS_PREFIXE + candidat1_id]);
                int rang_candidat2 = atoi(
                    mots_csv.elems[1 + electeur_i][BALLOTS_COLS_PREFIXE + candidat2_id]);

                // Si un score dans un ballot de vote n'est pas > 0, c'est soit qu'il n'a
                // pas pu etre lu, soit qu'il est invalide
                if (rang_candidat1 <= 0 || rang_candidat2 <= 0) {
                    erreurs = true; // Au moins une erreur a donc eu lieu
                } else {
                    // Le candidat ayant le plus gros rang (le candidat prefere a l'autre)
                    // voit son compteur de votes etre incremente dans la matrice des duels
                    if (rang_candidat1 > rang_candidat2)
                        duels->elems[candidat1_id][candidat2_id]++;
                }
            }
        }
    }

    return !erreurs;
}

void completer_mat_duels(t_mat_int_dyn* duels, int nb_electeurs) {
    // Il y a autant de ligne/colonne que de candidats
    int nb_candidats = duels->dim;

    for (int candidat1_id = 0; candidat1_id < nb_candidats; candidat1_id++) {
        for (int candidat2_id = 0; candidat2_id < candidat1_id; candidat2_id++) {
            int pourcentage_voix = duels->elems[candidat1_id][candidat2_id] / nb_electeurs;

            duels->elems[candidat1_id][candidat2_id] = pourcentage_voix;
            duels->elems[candidat2_id][candidat1_id] = 100 - pourcentage_voix;
        }
    }
}

bool creer_mat_duels(t_mat_char_star_dyn mots_csv, t_mat_int_dyn* duels) {
    // On verifie qu'il y ait au moins une en-tete et un electeur
    if (mots_csv.lignes < 2) {
        duels->elems = NULL;
        duels->dim = -1;

        return;
    }

    // On calcul le nombre de colonnes reservees aux candidats
    int nb_candidats = mots_csv.colonnes - BALLOTS_COLS_PREFIXE;
    // On peut donc allouer une matrice de duels a la bonne dimension
    if (!creer_t_mat_int_dyn(duels, nb_candidats)) {
        duels->elems = NULL;
        duels->dim = -1;

        return;
    }

    int nb_electeurs; // Nb d'electeurs trouves a l'analyse de la matrice CSV
    // On lit les ballots de vote
    bool ballots_sans_erreur = creer_mat_duels_absolue(mots_csv, &duels, &nb_electeurs);
    // On calcul l'integralite des pourcentages de voix
    completer_mat_duels(&duels, nb_electeurs);

    // On indique a l'appelant si certains scores des ballots etait illisibles
    return ballots_sans_erreur;
}
