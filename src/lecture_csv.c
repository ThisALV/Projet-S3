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
