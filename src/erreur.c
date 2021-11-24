#include <erreur.h>

#include <stdlib.h>
#include <stdio.h>


void verifier_alloc(void* mem, char* message) {
    if (mem == NULL) // Le code 2 signale une erreur interne au processus
        erreur_fatale(ERR_INTERNE, message);
}

void erreur_fatale(int code, char* message) {
    fprintf(stderr, "%s\n", message);
    exit(code);
}
