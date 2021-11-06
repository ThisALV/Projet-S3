# Structure du projet S3 *REV party*

## Structures de données

*Les structs ci-dessous possèdent toutes un `typedef` où le préfixe `s_` est remplacé par `t_`, on pourra par exemple écrire `t_tab_int_dyn` à la place de `struct s_tab_int_dyn`.*

- `s_tab_int_dyn` un tableau d'entiers à taille dynamique. Il représente une ligne contenue dans une matrice d'entiers à taille dynanique (`s_mat_int_dyn`)
- `s_mat_int_dyn` une matrice d'entiers à taille dynamique. Elle représente une matrice des duels, c'est-à-dire un tableau bidimensionnel des du score qu'à eu un candidat contre un autre candidat.
