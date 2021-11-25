#include <lecture_csv.h>

#include <string.h>
#include <stdlib.h>
#include <erreur.h>

// Selon les exemples fournis par Moodle, aucun CSV n'aura une lignes
// de plus de 1024 colonnes
#define LIGNE_MAX_TAILLE 1024


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
        duels = NULL;
        return;
    }

    // On creer la matrice carree de meme taille sans l'en-tete que celle des mots CSV
    if (!creer_t_mat_int_dyn(duels, mots_csv.lignes)) {
        // On traite l'erreur en cas de dim invalide pour la matrice d'entiers
        duels = NULL;
        return;
    }

    // Pour chaque ligne, en commencant par l'indince 1 pour ignorer l'en-tete
    for (int i = 1; i < mots_csv.lignes; i++) {
        for (int j = 0; j < mots_csv.colonnes; j++) {
            int score = atoi(mots_csv.elems[i][j]);

            // Si on a une valeur 0 qui n'est pas sur la diagonale, alors la matrice
            // est mal formee ou une case n'a pas pu etre lue
            if (score == 0 && j != (i - 1)) {
                // Une alloc a deja ete faite, c'est donc une erreur fatale
                erreur_fatale(ERR_INTERNE, "Matrice duels malformee");
            }

            duels->elems[i][j] = score; // On enregistre le score numerique dans les duels
        }
    }
}

void obtenir_candidats_duels(t_mat_char_star_dyn mots_csv, t_candidats* candidats) {
    // On verifie qu'il y ait au moins une en-tete dans le CSV
    if (mots_csv.lignes < 1) { // Sinon on traite l'erreur et on arrete le traitement
        candidats->elems = NULL;
        candidats->nb = -1;

        return;
    }

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
        char* nom = mots_csv.elems[0][id];

        // On alloue la memoire necessaire pour copier le nom
        int taille_nom = strlen(nom);
        c_courant->nom = (char*) malloc(sizeof(char) * taille_nom);

        // On effectue la copie du nom et on affecte l'ID
        strcpy(c_courant->nom, nom);
        c_courant->id = id;
    }
}
