# Structure du projet S3 *REV party*

## Structures de données

*Les structs ci-dessous possèdent toutes un `typedef` où le préfixe `s_` est remplacé par `t_`, on pourra par exemple écrire `t_tab_int_dyn` à la place de `struct s_tab_int_dyn`.*

- `s_tab_int_dyn` un tableau d'entiers à taille dynamique. Il représente une ligne contenue dans une matrice d'entiers à taille dynanique (`s_mat_int_dyn`)
- `s_mat_int_dyn` une matrice d'entiers à taille dynamique. Elle représente une matrice des duels, c'est-à-dire un tableau bidimensionnel des du score qu'à eu un candidat contre un autre candidat.
- `s_tab_char_star_dyn` un tableau de strings (`char*`) à taille dynamique. Il représente une ligne contenue dans une matrice de strings à taille dynamique (`s_mat_char_star_dyn`)
- `s_mat_char_star_dyn` une matrice de strings à taille dynamique. Elle représente les différentes valeurs (les mots) d'un fichier csv lu : chaque ligne représente une ligne du fichier csv et chaque colonne représente une valeur séparée par une séparateur donné (ici, `\t`)
- `s_arc_p` un arc pondéré, c-est-à-dire une origine, une destination et un poids. L'origine est le point du candidat gagnant, la destination le point du candidat perdant et le poids le score du gagnant face au perdant. Cette structure a pour type associé `Elementliste` car c'est pour contenir plusieurs arcs que `liste` est utilisée.
- `liste` une liste statique circulaire pouvant être utilisée comme une pile ou comme une file. Elle est utilisée pour contenir les arcs dans un graphe représentant le résultat d'un scrutin.
- `parametres` les options recueillies dans `argv`, interprétées et valides, afin de savoir de quelle manière le script principal sera exécuté (logs, méthode de scrutin, type des données en entrée).
- `candidat` contient le nom alloué dynamiquement (`char*`) d'un candidat lu dans la matrice de mots ainsi que son ID (`int`)
- `candidats` contient un tableau dynamqiue de `candidat` ainsi que le nombre de candidats

## Fichiers d'en-tête

La structure initiale du projet voudrait utiliser `global.h` pour déclarer toutes les fonctions et structures de données dedans, mais il est préférable d'utiliser des fichiers d'en-tête séparés pour chaque module.

Ainsi, il est possible pour un utilisateur d'un module de n'inclure que celui-ci  (exemple : `utils_sd.h`) et d'éviter de polluer son environnement de travail (autocomplétion, etc.) avec les déclarations utiles pour les autres modules.

- `global.h` contient un include vers tous les fichiers d'en-têtes du projet. En effet, certains programmes de tests fournis par Moodle utilisent toujours ce fichier, donc on s'assure de rester compatible avec eux.
- `utils_sd.h` contient les déclarations des structures de données génériques, c'est-à-dire matrices et tableaux dynamiques d'entiers et de strings, ainsi que des fonctions utilitaires permettant leur création.
- `liste.h` et `elementliste.h` en-têtes non modifiables fournies par Moodle, contiennent respectivement les définitions du type `liste`, des fonctions pour utiliser ce type (`liste.h`) et le type des données stockées dans cette liste ainsi que des fonctions permettant de manipuler ces éléments (`elementliste.h`).
- `lecture_csv.h` fonction de lecture d'un fichier csv permettant d'obtenir une matrice des valeurs du fichier
- `traitements_sd.h` contient les fonctions qui interprètent les valeurs csv lu avec le module `lecture_csv` pour en faire une matrice de duels entre les candidats, qui sera elle-même transformée en graphe des vainqueurs composé de plusieurs arcs pondérés.
- `utils_scrutins.h` contient les fonctions utilitaires permettant d'implémenter les algorithmes communs aux diverses méthodes de scrutin. Dans notre cas, nous avons `vainqueur_uninominal` et `vainqueurs _condorcet` qui seront utilisé par les différentes méthodes de scrutin.
- `uninominales.h` contient les fonctions pour les scrutins uninominaux à un et deux tours, en utilisant le module `uninominale` pour obtenir le gagnant.
- `condorcet.h` contient les fonctions pour les scrutins de condorcet avec les méthodes de résolution de paradoxe que sont Minimax et Shulze.

*`main.c` et le programme `verifier_mon_vote` étant des scripts principaux utilisant les modules précédents sans en créer de nouveaux, ils ne possèdent pas d'en-tête.*

## Fonctions

Les déclarations des fonctions sont réparties dans les différents fichiers headers décrits [ci-dessus](#fichiers-den-tête).

Voici une liste de ces fonctions triées par leur module :

### utils_sd

- `int* creer_tab_int(int dim)` retourne un tableau d'entiers à taille dynamique avec la dimension demandée
- `int** creer_mat_int(int dim)` retourne une matrice **carrée** d'entiers dynamique à la dimension demandée
- `char** creer_tab_char_star(int dim)` retourne un tableau de chaîne de caractères à taille dynamique avec la dimension demandée
- `char*** creer_mat_char_star(int dim)` retourne une matrice **carrée** de chaînes de caractères à la dimension demandée

- `void creer_t_tab_int_dyn(t_tab_int_dyn *tab, int dim)` initialise la structure pour le tableau d'entiers dynamique passé en paramètre avec la dimension demandée
- `void creer_t_mat_int_dyn(t_mat_int_dyn *tab, int dim)` initialise la structure pour le matrice carrée d'entiers dynamique passé en paramètre avec la dimension demandée
- `void creer_t_tab_char_star_dyn(t_tab_char_star_dyn *tab, int dim)` initialise la structure pour le tableau de chaînes de caractères dynamique passé en paramètre avec la dimension demandée
- `void creer_t_mat_char_star_dyn(t_tab_char_star_dyn *tab, int dim)` initialise la structure pour le matrice carrée de chaines de caractères dynamique passé en paramètre avec la dimension demandée

### liste

Les déclarations des fonctions de ce module ont été fournies par Moodle.

- `int nbEltList(liste lst)` retourne le nombre d'éléments contenus dans la liste donnée
- `void createList(liste *p)` initialise la liste donnée en paramètre comme vide
- `void addFrontList(liste *p, ElementListe e)` ajoute l'élément donné en paramètre au début de la liste donnée
- `void addTailList(liste *p, ElementListe e)` ajoute l'élément donné à la fin de la liste
- `void delTailList(liste *p)` supprime un élément à la fin de la liste donnée
- `void delFrontList(liste *p)` supprime un élément au début de la liste donnée
- `void headList(liste p, Elemenliste *e)` affecte au pointeur l'adresse de l'élément au début de la liste
- `void tailList(liste p, Elemenliste *e)` affecte au pointeur l'adresse de l'élément à la fin de la liste
- `bool emptyList(liste p)` renvoie un booléen indiquant si la liste est vide
- `bool fullList(liste p)` renvoie un booléen indiquant si la liste est pleine
- `void dumpList(liste p, FILE *fp)` affiche le contenu de la liste dans le flix de sortie donnée en paramètre
- `void swapEltList(Elementliste *a, Elementliste *b)` échange les éléments aux adresses données de place dans leur liste
- `void bubbleSortList(liste *p)` trie la liste par ordre décroissant en utilisant l'algorithme du tri à bulle
- `void pickEltList(liste l, Elementliste *e,int index)` affecte l'adresse de l'élément ayant l'index donné au pointeur donné en paramètre
- `bool belongEltList(liste p,Elementliste e)` renvoie un booléen indiquant si l'élément donné est présent dans la liste

### elementliste

Les déclarations des fonctions de ce module ont été fournies par Moodle.

- `void saisie_element(Elementliste *e)` affiche un message de prompt et lis les informations sur l'entrée standard afin de construire un nouvel élément de liste : un arc
- `void copie_element(Elementliste *cible, Elementliste source)` copie les données de l'élément source dans l'élément cible. *Ne modifie pas l'élément source*
- `void afficher_element(Elementliste e, FILE *fp)` affiche les informations sur l'élément dans le flux de sortie donné en paramètre
- `bool cmp_elements(Elementliste e1, Elementliste e2)` renvoie un booléen indiquant si les arcs représentés par les deux éléments donnés en paramètre sont identiques

### lecture_csv

- `void lire_fichier_votes(char* nom_fichier, char* separateur, t_mat_char_star_dyn *mots)` lis le fichier au chemin donné en format CSV et stocke les lignes et les mots séparés par le séparateur en paramètre dans la matrice dynamique en paramètre

### traitements_sd

- `void convertir_mat_duels(t_mat_char_star_dyn duels_str, t_mat_int_dyn *duels)` convertit la matrice de duels lue directement dans le fichier CSV avec lire_fichier_votes()(#lecturecsv) en une matrice d'entiers exploitable par les algorithmes de scrutin
- `void obtenir_candidats_ballots(t_mat_char_star_dyn ballots, t_tab_char_star_dyn *candidats)` obtient la liste des noms des candidats depuis une liste de ballots et la stocke dans le tableau dynamique en paramètre
- `void obtenir_candidats_duels(t_mat_char_star_dyn duels, t_tab_char_star_dyn *candidats)` obtient la liste des noms des candidats depuis une matrice de duels et la stocke dans le tableau dynamique en paramètre
- `void creer_mat_duels(t_mat_char_star_dyn ballots, t_mat_int_dyn *duels)` interpète la matrice de mots au format "ballots de vote" pour la stocker sans forme d'une matrice de duels
- `void creer_liste_arcs(t_mat_int_dyn duels, liste* arcs)` créée un graphe des vainqueurs (ou liste des arcs) à partir d'une matrice de duels

### utils_scrutins

- `int vainqueur_uninominale(t_mat_int_dyn duels)` retourne l'ID du candidat vainqueur selon la méthode de scrutin uninominale. En cas d'égalité, l'âge stocké dans une BDD l'intérieur du module est utilisé pour déterminé le gagnant
- `int vainqueur_condorcet(t_mat_int_dyn duels, t_tab_int_dyn *vainqueurs)` retourne l'ID du vainqueur de condorcet s'il y en a un, `CONDORCET_EGALITE` sinon, ce sera alors à la fonction appelante de déterminer le vainqueur avec son propre algorithme.
Algo : on vérifie pour chaque ligne (candidat) si tous les duels sont gagnés, si c'est le cas on a notre vainqueur de condorcet. Si aucune ligne ne remplie ce critère, alors il n'y a pas de vainqueur de condorcet.

### uninominales

- `int uninominale_un_tour(t_mat_int_dyn duels)` retourne l'ID du gagnant sur un scrutin uninominal à un tour
- `int uninominale_deux_tours(t_mat_int_dyn duels_tour1, t_mat_int_dyn duels_tour2)` retourne l'ID du gagnant sur un scrutin uninominale à deux tours. Les votes du 2ème tour sont gérés à l'aide d'un appel à un script python externe fournie depuis Moodle, mais on laisse la fonction appelante donner la matrice des duels en paramètre pour faciliter la flexibilité du programme et les tests unitaires.

### condorcets

- `int condorcet_minimax(t_mat_int_dyn duels)` retourne l'ID du gagnant selon la méthode de condorcet, déterminé à l'aide de la méthode des minimax s'il n'y a pas de vainqueur de condorcet.
Algo : pour chaque ligne, on détermine le minimum, et on teste ensuite si ce minimum est plus grand que le minimax actuellement stocké. Si c'est le cas, alors on le désigne comme étant le nouveau minimax et on sauvegarde le candidat (ligne) actuel pour le désigner comme nouveau vainqueur.
- `int condorcet_shculze(t_mat_int_dyn duels, liste arcs)` retourne l'ID du gagnant selon la méthode de condorcet, déterminé à l'aide de la méthode de Shulze
