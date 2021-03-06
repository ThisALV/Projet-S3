#include <log.h>
#include <unitaires/utils_sd.h>
#include <unitaires/lecture_csv.h>
#include <unitaires/utils_scrutins.h>
#include <unitaires/condorcet_minimax.h>
#include <unitaires/condorcet_schulze.h>
#include <unitaires/utils_main.h>
#include <unitaires/utils_verifier_mon_vote.h>
#include <stdio.h>


void test_unitaire_module(char* nom_module, void (*script_tests)()) {
    printf("Tests unitaires pour %s...\n", nom_module);
    script_tests();
    printf("Module OK !\n");
}

int main() {
    sortie_logging_par_defaut();

    test_unitaire_module("utils_sd", tests_unitaires_utils_sd);
    test_unitaire_module("lecture_csv", tests_unitaires_lecture_csv);
    test_unitaire_module("utils_scrutin", tests_unitaires_utils_scrutin);
    test_unitaire_module("condorcet_minimax", tests_unitaires_condorcet_minimax);
    test_unitaire_module("condorcet_schulze", tests_unitaires_condorcet_schulze);
    test_unitaire_module("utils_main", tests_unitaires_utils_main);
    test_unitaire_module("utils_verifier_mon_vote", tests_unitaires_utils_verifier_mon_vote);

    return 0;
}