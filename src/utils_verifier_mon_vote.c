#include <utils_verifier_mon_vote.h>

#include <erreur.h>
#include <sys/random.h>
#include <stdlib.h>
#include <string.h>
#include <sha256_utils.h>

// Est appelee pour verifier si une operation d'ecriture s'est terminee
// avec succes, et lever une erreur fatale si ce n'est pas le cas
#define VERIFIER_ECRITURE_CSV(operation_ecriture)\
    if (operation_ecriture < 0)\
        erreur_fatale(2, "Ecriture fichier CSV");

// Nombre de caracteres differents disponibles pour une cle
// 26 lettres * 2 pour les min/majuscules + 10 chiffres = 62
#define CARACTERES_DISPONIBLES 62


// Tableau des caracteres alphanumeriques disponibles pour la
// generation de la cle privee
static char* caracteres_cle =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";


void generer_cle_privee(char cle_privee[TAILLE_CLE_PRIVEE + 1]) {
    unsigned int seed_caracteres;
    // On essaye de stocker un nombre aleatoire dans notre seed, et
    // si cela echoue alors on ne peut pas generer la cle privee
    if (getrandom((void*) &seed_caracteres, sizeof(unsigned int), 0) == -1)
        erreur_fatale(2, "Generation seed aleatoire pour cle privee");

    // Si tout c'est bien passe, on a un seed aleatoire pour notre
    // generation de caracteres, donc notre cle sera completement aleatoire
    srand(seed_caracteres);

    // On genere 1 a 1 les caracteres de notre cle
    for (int caractere_i = 0; caractere_i < TAILLE_CLE_PRIVEE; caractere_i++) {
        // On genere un index de 0 a 61 pour piocher un caractere alphanumerique
        int courant_i = rand() % CARACTERES_DISPONIBLES;
        cle_privee[caractere_i] = caracteres_cle[courant_i];
    }

    // On n'oublie pas d'ajouter la terminaison \0 en plus de la cle
    cle_privee[TAILLE_CLE_PRIVEE] = '\0';
}

void hash_electeur(char* nom_electeur, char* cle_privee, char hash_electeur[TAILLE_HASH_ELECTEUR + 1]) {
    // On alloue une chaine qui contiendra le nom, la cle et \0
    char* nom_et_cle = (char*) malloc(strlen(nom_electeur) + strlen(cle_privee) + 1);
    verifier_alloc(nom_et_cle, "Concatenation nom et cle privee");
    // On copie le nom dans cette chaine
    strcpy(nom_et_cle, nom_electeur);
    // On y ajoute la cle
    strcat(nom_et_cle, cle_privee);

    // On genere le hash dans le tableau de sortie
    sha256ofString((BYTE*) nom_et_cle, hash_electeur);

    // On n'oublie pas de detruire la chaine temporaire a la fin de l'operation
    free(nom_et_cle);
}

void ecrire_fichier_votes(FILE* fichier_csv, char separateur, t_mat_char_star_dyn mots_csv) {
    for (int ligne_i = 0; ligne_i < mots_csv.lignes; ligne_i++) {
        bool premier_mot = true; // Sera mis a false des qu'on aura ecrit un mot CSV

        for (int colonne_i = 0; colonne_i < mots_csv.colonnes; colonne_i++) {
            // Si ce n'est pas le 1er mot de la ligne, il doit etre precede d'un
            // separateur
            if (!premier_mot)
                VERIFIER_ECRITURE_CSV(fprintf(fichier_csv, "%c", separateur));

            // On ecrit le mot dans la ligne courante
            char* mot = mots_csv.elems[ligne_i][colonne_i];
            VERIFIER_ECRITURE_CSV(fprintf(fichier_csv, "%s", mot));

            // On sait qu'on a ecrit au moins 1 mot dans cette ligne
            premier_mot = false;
        }

        // On passe a la ligne suivante
        VERIFIER_ECRITURE_CSV(fprintf(fichier_csv, "\n"));
    }
}
