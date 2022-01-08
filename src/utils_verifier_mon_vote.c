#include <utils_verifier_mon_vote.h>

#include <erreur.h>
#include <sys/random.h>
#include <stdlib.h>
#include <string.h>
#include <sha256_utils.h>
#include <lecture_csv.h>

// Est appelee pour verifier si une operation d'ecriture s'est terminee
// avec succes, et lever une erreur fatale si ce n'est pas le cas
#define VERIFIER_ECRITURE_CSV(operation_ecriture)\
    if (operation_ecriture < 0)\
        erreur_fatale(2, "Ecriture fichier CSV");

// Nombre de caracteres differents disponibles pour une cle
// 26 lettres * 2 pour les min/majuscules + 10 chiffres = 62
#define CARACTERES_DISPONIBLES 62

// Le nom de l'electeur se trouve a la 4eme colonne d'une ligne CSV dans un fichier
// de ballots
#define NOM_ELECTEUR_COLONNE_I 3


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

bool chiffrer_ballots_votes(t_mat_char_star_dyn mots_csv, char** cles_privees, FILE* sortie_cles) {
    // On verifie qu'il y a bien suffisament de colonnes pour contenu un nom d'electeur
    // Si ce n'est pas le cas, on retourne que l'operation a echoue
    if (mots_csv.colonnes < NOM_ELECTEUR_COLONNE_I + 1)
        return false;

    // Sachant que la 1ere ligne est l'en-tete, donc pas un ballot de votes
    int nb_electeurs = mots_csv.lignes - 1;

    // Pour chaque ballot de votes
    for (int electeur_i = 0; electeur_i < nb_electeurs; electeur_i++) {
        int ligne_i = electeur_i + 1; // On a saute l'en-tete du CSV
        char** ligne_courante = mots_csv.elems[ligne_i];

        // On obtient le nom de l'electeur a la 4eme colonne
        char* nom_electeur = ligne_courante[NOM_ELECTEUR_COLONNE_I];
        // On obtient la cle privee qui lui est associee
        char* cle_privee = cles_privees[electeur_i];

        // On affiche la cle privee pour que l'utilisateur pour s'authentifier
        // avec par la suite
        if (fprintf(sortie_cles, "%s:%s\n", nom_electeur, cle_privee) < 0) {
            // En verifiant que l'ecriture dans le fichier de sortie a reussi
            fclose(sortie_cles);
            erreur_fatale(2, "Ecriture nom electeur et cle privee");
        }

        //
        // On remplace le nom par le hash obtenu dans la matrice de mots CSV
        //

        // On commence par generer le hash qui est alloue dynamiquement pour
        // rester disponible en memoire jusqu'a l'ecriture dans le fichier CSV
        char* hash = (char*) malloc((TAILLE_HASH_ELECTEUR + 1) * sizeof(char));
        hash_electeur(nom_electeur, cle_privee, hash);
        // Le nom etait alloue dynamiquement en memoire, il faudra donc le liberer
        free(nom_electeur);
        // Enfin, on peut affecter le hash a la colonne CSV correspondante
        ligne_courante[NOM_ELECTEUR_COLONNE_I] = hash;
    }

    return true; // L'operation s'est fini sans erreur
}

// Si le hash de l'electeur courant est celui recherche, alors on affecte les scores
// donnes par cet electeur aux differents candidats et on retourne `true`.
// Sinon, on retourne `false`.
static bool analyser_ballot_electeur(
    char** ligne_electeur, int nb_colonnes, char* hash_recherche,
    t_tab_char_star_dyn* scores_candidats)
{
    char* hash_courant = ligne_electeur[NOM_ELECTEUR_COLONNE_I];
    if (strcmp(hash_recherche, hash_courant) != 0)
        return false; // On passe cet electeur, ce n'est pas celui recherche

    // Pour chaque colonne contenant un score de candidats, ces colonnes se
    // situant juste apres celle contenant le hash/nom de l'electeur
    for (int col_i = NOM_ELECTEUR_COLONNE_I + 1; col_i < nb_colonnes; col_i++) {
        char* score = allouer_copie_char_star(ligne_electeur[col_i], "Copie score ballot");

        // On calcul l'index du score a partir du nombre de colonnes prefixes
        // et de l'index de sa colonne dans la matrice de mots CSV
        int score_i = col_i - 4;
        scores_candidats->elems[score_i] = score;
    }

    // On a effectue notre traitement sur l'electeur recherche
    return true;
}

bool chercher_electeur(
    t_mat_char_star_dyn mots_csv, char* nom_electeur, char* cle_privee,
    t_tab_char_star_dyn* noms_candidats, t_tab_char_star_dyn* scores_candidats)
{
    int nb_colonnes = mots_csv.colonnes;

    // On doit avoir suffisamment de colonnes par le hash de l'electeur et 1
    // candidat
    if (nb_colonnes < 5)
        return false; // Sinon l'operation est impossible

    // On forme la BDD des candidats a partir de la liste de ballots pour avoir
    // acces a leur nom
    t_candidats bdd_candidats;
    obtenir_candidats_ballots(mots_csv, &bdd_candidats);

    // On affecte les noms de tous les candidats
    int nb_candidats = bdd_candidats.nb;
    creer_t_tab_char_star_dyn(noms_candidats, nb_candidats);
    for (int candidat_id = 0; candidat_id < nb_candidats; candidat_id++) {
        char* nom = allouer_copie_char_star(
            bdd_candidats.elems[candidat_id].nom, "Copie nom candidat");

        noms_candidats->elems[candidat_id] = nom;
    }

    // On aura autant de colonnes de score que de candidats
    creer_t_tab_char_star_dyn(scores_candidats, nb_candidats);

    // C'est ce hash que l'on cherchera dans la colonne electeur des mots CSV
    char hash_recherche[TAILLE_HASH_ELECTEUR + 1];
    hash_electeur(nom_electeur, cle_privee, hash_recherche);
    
    // Au debut, on n'a pas encore trouver l'electeur
    bool electeur_trouve = false;
    // Sachant que la 1ere ligne est l'en-tete, donc pas un ballot de votes
    int nb_electeurs = mots_csv.lignes - 1;
    // Pour chaque electeur, tant qu'on a pas trouve celui qu'on recherche
    for (int electeur_i = 0; !electeur_trouve && electeur_i < nb_electeurs; electeur_i++) {
        int ligne_i = electeur_i + 1;
        char** ligne_courante = mots_csv.elems[ligne_i];

        // Si l'analyse a trouve l'electeur recherche, alors on peut arreter la
        electeur_trouve = analyser_ballot_electeur(
            ligne_courante, nb_colonnes, hash_recherche, scores_candidats);
    }

    // On libere les ressources utilisees par l'operation
    detruire_t_candidats_dyn(&bdd_candidats);

    // Si on a pas trouve l'electeur, alors on le signale en passant les scores
    // en mode erreur
    if (!electeur_trouve)
        mettre_t_tab_char_star_dyn_erreur(scores_candidats);

    // Peu importe si on a trouve l'electeur ou non, l'operation s'est terminee
    // normalement
    return true;
}
