# Structure du projet S3 *REV party*

## Structures de données

*Les structs ci-dessous possèdent toutes un `typedef` où le préfixe `s_` est remplacé par `t_`, on pourra par exemple écrire `t_tab_int_dyn` à la place de `struct s_tab_int_dyn`.*

- `s_tab_int_dyn` un tableau d'entiers à taille dynamique. Il représente une ligne contenue dans une matrice d'entiers à taille dynanique (`s_mat_int_dyn`)
- `s_mat_int_dyn` une matrice d'entiers à taille dynamique. Elle représente une matrice des duels, c'est-à-dire un tableau bidimensionnel des du score qu'à eu un candidat contre un autre candidat.
- `s_tab_char_star_dyn` un tableau de strings (`char*`) à taille dynamique. Il représente une ligne contenue dans une matrice de strings à taille dynamique (`s_mat_char_star_dyn`)
- `s_mat_char_star_dyn` une matrice de strings à taille dynamique. Elle représente les différentes valeurs (les mots) d'un fichier csv lu : chaque ligne représente une ligne du fichier csv et chaque colonne représente une valeur séparée par une séparateur donné (ici, `\t`)
- `s_arc_p` un arc pondéré, c-est-à-dire une origine, une destination et un poids. L'origine est le point du candidat perdant, la destination le point du candidat gagnant et le poids le score du gagnant face au perdant. Cette structure a pour type associé `Elementliste` car c'est pour contenir plusieurs arcs que `liste` est utilisée.
- `liste` une liste statique circulaire pouvant être utilisée comme une pile ou comme une file. Elle est utilisée pour contenir les arcs dans un graphe représentant le résultat d'un scrutin.
- `parametres` les options recueillies dans `argv`, interprétées et valides, afin de savoir de quelle manière le script principal sera exécuté (logs, méthode de scrutin, type des données en entrée).

## Fichiers d'en-tête

La structure initiale du projet voudrait utiliser `global.h` pour déclarer toutes les fonctions et structures de données dedans, mais il est préférable d'utiliser des fichiers d'en-tête séparés pour chaque module.

Ainsi, il est possible pour un utilisateur d'un module de n'inclure que celui-ci  (exemple : `utils_sd.h`) et d'éviter de polluer son environnement de travail (autocomplétion, etc.) avec les déclarations utiles pour les autres modules.

- `global.h` contient un include vers tous les fichiers d'en-têtes du projet. En effet, certains programmes de tests fournis par Moodle utilisent toujours ce fichier, donc on s'assure de rester compatible avec eux.
- `utils_sd.h` contient les déclarations des structures de données génériques, c'est-à-dire matrices et tableaux dynamiques d'entiers et de strings, ainsi que des fonctions utilitaires permettant leur création.
- `liste.h` et `elementliste.h` en-têtes non modifiables fournies par Moodle, contiennent respectivement les définitions du type `liste`, des fonctions pour utiliser ce type (`liste.h`) et le type des données stockées dans cette liste ainsi que des fonctions permettant de manipuler ces éléments (`elementliste.h`).
- `lecture_csv.h` fonction de lecture d'un fichier csv permettant d'obtenir une matrice des valeurs du fichier.
- `traitements_sd.h` contient les fonctions qui interprètent les valeurs csv lu avec le module `lecture_csv` pour en faire une matrice de duels entre les candidats, qui sera elle-même transformée en graphe des vainqueurs composé de plusieurs arcs pondérés.
- `utils_scrutins.h` contient les fonctions utilitaires permettant d'implémenter les algorithmes communs aux diverses méthodes de scrutin. Dans notre cas, nous avons `vainqueur_uninominal` et `vainqueurs _condorcet` qui seront utilisé par les différentes méthodes de scrutin.
- `uninominales.h` contient les fonctions pour les scrutins uninominaux à un et deux tours, en utilisant le module `uninominale` pour obtenir le gagnant.
- `condorcet.h` contient les fonctions pour les scrutins de condorcet avec les méthodes de résolution de paradoxe que sont Minimax et Shulze.

*`main.c` et le programme `verifier_mon_vote` étant des scripts principaux utilisant les modules précédents sans en créer de nouveaux, ils ne possèdent pas d'en-tête.*
