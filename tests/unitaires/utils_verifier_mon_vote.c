#include <unitaires/utils_verifier_mon_vote.h>

#include <utils_verifier_mon_vote.h>
#include <assert.h>
#include <string.h>


//
// hash_electeur
//

// Teste si pour le nom d'electeur et la cle privee donnee, on obtient bien le
// hash d'electeur passe en parametre
static void tester_hash_electeur(char* nom_electeur, char* cle_privee, char* hash_attendu) {
    char hash_obtenu[TAILLE_HASH_ELECTEUR + 1];
    hash_electeur(nom_electeur, cle_privee, hash_obtenu);

    assert(strcmp(hash_obtenu, hash_attendu) == 0);
}
 
void hash_electeur_nom_vide() {
    char* nom_electeur = "";
    char* cle_privee =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char* hash_attendu =
        "0f45e858fbc4176cdf4e411f88281edefc390ae5afe7df0f44cd9297f0a64580";

    tester_hash_electeur(nom_electeur, cle_privee, hash_attendu);
}

void hash_electeur_nom_electeur() {
    char* nom_electeur = "MACHIN Truc";
    char* cle_privee =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    char* hash_attendu =
        "86244289c722529343bb01182622f1ae45ef1bf039831f070a8aba2de7411fc3";

    tester_hash_electeur(nom_electeur, cle_privee, hash_attendu);
}


// Script principale pour les tests

void tests_unitaires_utils_verifier_mon_vote() {
    hash_electeur_nom_vide();
    hash_electeur_nom_electeur();
}
