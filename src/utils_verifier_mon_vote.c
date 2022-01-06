#include <utils_verifier_mon_vote.h>

#include <erreur.h>
#include <sys/random.h>
#include <stdlib.h>

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
