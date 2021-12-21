#include <integration/lecture_csv.h>
#include <integration/log.h>
#include <stdio.h>


void test_integration_module(char* nom_module, void (*script_tests)()) {
    printf("Tests d'integration pour %s...\n", nom_module);
    script_tests();
    printf("Module OK !\n");
}

int main() {
    test_integration_module("lecture_csv", tests_integration_lecture_csv);
    test_integration_module("log", tests_integration_log);

    return 0;
}
