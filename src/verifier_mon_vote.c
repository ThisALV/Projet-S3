#include <erreur.h>
#include <string.h>
#include <stdio.h>
#include <lecture_csv.h>
#include <utils_verifier_mon_vote.h>
#include <stdlib.h>

// Taille maximale d'un nom d'electeur
#define TAILLE_MAX_NOM 120


// Message a afficher en cas d'erreur sur la ligne de commande
static char* erreur_parametres =
    "Utilisation : <chemin_csv> [--chiffrer]\n"
    "\n"
    "chemin_csv : chemin du fichier contenant les ballots de votes\n"
    "--chiffrer : option permettant de chiffrer les noms d'electeurs dans"
    " <chemin_csv> et d'ecrire les paires nom_electeur:cle_privee dans"
    " la sortie standard";


// Dans la matrice de mots CSV donee, chiffre le nom
// de chaque electeur et ecrit sa cle privee dans stdout,
void chiffrer_electeurs_fichier_csv(t_mat_char_star_dyn mots_csv) {
    // Generation d'une cle privee pour chaque electeur
    int nb_electeurs = mots_csv.lignes - 1;
    t_tab_char_star_dyn cles_privees;
    creer_t_tab_char_star_dyn(&cles_privees, nb_electeurs);
    for (int electeur_i = 0; electeur_i < nb_electeurs; electeur_i++) {
        // On alloue dynamiquement une nouvelle cle privee
        char* cle = (char*) malloc((TAILLE_CLE_PRIVEE + 1) * sizeof(char));
        generer_cle_privee(cle);
        // On l'assigne a la cle de l'electeur courant
        cles_privees.elems[electeur_i] = cle;
    }

    // On utilise les cles qu'on affiche dans stdout pour chiffrer les noms
    chiffrer_ballots_votes(mots_csv, cles_privees.elems, stdout);

    // Destruction des ressources utilisees par le traitement

    for (int cle_i = 0; cle_i < nb_electeurs; cle_i++)
        free(cles_privees.elems[cle_i]);

    detruire_t_tab_char_star_dyn(&cles_privees);
}

// Recherche un hash correspondant aux identifiants de l'electeur et affiche
// son ballot de vote s'il est trouve
void afficher_ballot_electeur(t_mat_char_star_dyn mots_csv, char* nom_electeur, char* cle_privee) {
    // Informations relevees dans le fichier CSV
    t_tab_char_star_dyn noms_candidats;
    t_tab_char_star_dyn scores_candidats;

    bool fichier_csv_valide = chercher_electeur(
        mots_csv, nom_electeur, cle_privee, &noms_candidats, &scores_candidats);

    // Si le fichier CSV est mal formee, on signale une erreur
    if (!fichier_csv_valide)
        erreur_fatale(2, "Fichier CSV mal forme, pas assez de colonnes");

    // Si on a trouve un ballot avec le hash de l'electeur...
    if (!est_t_tab_char_star_dyn_erreur(scores_candidats)) {
        printf("Nom du candidat : score attribue\n");

        // Alors pour chaque candidat, on affiche le score que l'electeur lui a
        // a donne
        int nb_candidats = noms_candidats.taille;
        for (int candidat_id = 0; candidat_id < nb_candidats; candidat_id++) {
            char* nom_candidat = noms_candidats.elems[candidat_id];
            char* score = scores_candidats.elems[candidat_id];

            printf("%s : %s\n", nom_candidat, score);
        }
    } else { // Sinon, on affiche que rien n'a ete trouve
        printf("Aucun vote trouve pour cet electeur, verifiez le nom d'electeur"
               " ainsi que la cle privee ou le fichier CSV d'entree.\n");
    }

    // Destruction des ressources du traitement, si necessaire
    detruire_t_tab_char_star_dyn(&noms_candidats);
    // S'il y un tab de resultat, on le detruit
    if (!est_t_tab_char_star_dyn_erreur(scores_candidats))
        detruire_t_tab_char_star_dyn(&scores_candidats);
}

int main(int argc, char* argv[]) {
    // On commence par recuperer le fichier de ballots de votes sur lequel
    // on va travailler
    if (argc < 2) // Un 1er parametre doit contenir le chemin du fichier
        erreur_fatale(1, erreur_parametres);

    char* chemin_fichier_csv = argv[1];

    // On lit le contenu du fichier pour l'assigner a de la matrice de mots CSV
    // sur laquelle on va travailler
    FILE* fichier_csv = fopen(chemin_fichier_csv, "r");
    t_mat_char_star_dyn mots_csv;
    lire_fichier_votes(fichier_csv, ",", &mots_csv);
    fclose(fichier_csv); // Tout est stocke, on peut fermer le fichier en lecture

    // Si un 2eme parametre contient "--chiffrer", alors le programme chiffrera
    // les noms des electeurs dans le fichier
    if (argc > 2 && strcmp(argv[2], "--chiffrer") == 0) {
        chiffrer_electeurs_fichier_csv(mots_csv);

        // On reouvre le fichier en ecriture cette fois-ci pour remplacer les
        // noms des electeurs par leur hash respectifs
        fichier_csv = fopen(chemin_fichier_csv, "w");
        ecrire_fichier_votes(fichier_csv, ',', mots_csv);
        fclose(fichier_csv);

        // On libere les ressources utilisees par le programme
        detruire_t_mat_char_star_dyn(&mots_csv);

        return 0;
    }

    printf("Si vous souhaitez chiffrer les noms des electeurs dans %s, utilisez"
           " l'option --chiffrer a la fin de la ligne de commande.\n\n",
           chemin_fichier_csv);

    // Par securite, on lira les identifiants de l'electeur (nom + cle) dans
    // l'entree standard plutot que dans la ligne de commande

    char nom_electeur[TAILLE_MAX_NOM + 1];
    char cle_privee[TAILLE_CLE_PRIVEE + 1];
    char fin_de_ligne;

    printf("Nom de l'electeur :\n");
    // On lit le nom sur toute la ligne, en ignorant le \n
    scanf("%[^\n]%c", nom_electeur, &fin_de_ligne);

    printf("Cle privee de l'electeur :\n");
    // On lit la cle privee comme un seul mot de maximum 80 caracteres
    scanf("%80s", cle_privee);

    afficher_ballot_electeur(mots_csv, nom_electeur, cle_privee);
    detruire_t_mat_char_star_dyn(&mots_csv);

    return 0;
}
