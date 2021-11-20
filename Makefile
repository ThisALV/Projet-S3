# Les fichiers d'en-têtes .h sont rangés dans un répertoire à part "include"
CFLAGS := -Iinclude/

# Fonction appelée pour compiler l'exécutable de la target courante et afficher
# un message à l'utilisateur
define generer_executable
$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $@
@echo "Exécutable $@ généré dans le répertoire $(REP_BINAIRES) !"
endef

# Fonction appelée pour compiler le fichier .c de la target courante
define generer_objet
@mkdir -p $(dir $@)
$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@
endef

# Les fichiers sources .c sont rangés dans le répertoire "src"
REP_SOURCES := src
# Les fichiers objets et les exécutables sont générés dans le répertoire "build"
REP_BINAIRES := build
# Les fichiers objets contenant le code des tests sont rangés dans les répertoires
# "build/unitaires" et "build/integration"
REP_BINAIRES_UNITS := $(REP_BINAIRES)/unitaires
REP_BINAIRES_INTE := $(REP_BINAIRES)/integration

# Les fichiers sources .c des tests unitaires sont dans "tests/unitaires"
REP_UNITAIRES := tests/unitaires
# Les fichiers sources .c des tests integration sont dans "tests/integration"
REP_INTEGRATION := tests/integration

# Fichiers sources .c contenant un "int main(...)" servant de script principal à
# un exécutable du projet
scripts_executables := $(REP_SOURCES)/main.c $(REP_SOURCES)/verifier_mon_vote.c
# Les fichiers sources des modules sont tous les fichiers de "src" exceptés ceux
# contenant les scripts principaux des exécutables
sources_modules := $(filter-out $(scripts_executables), $(wildcard $(REP_SOURCES)/*.c))
# Les fichiers objets des modules à compilés ont une extension .o et sont rangés
# dans le dossier "build" et non pas dans "src"
MODULES := $(patsubst $(REP_SOURCES)/%.c, $(REP_BINAIRES)/%.o, $(sources_modules))

# Les fichiers sources contenant les fonctions de tests (unitaire ou d'intégration)
# sont stiués dans "tests/unitaires" et "tests/integration"
sources_unitaires := $(wildcard $(REP_UNITAIRES)/*.c)
sources_integration := $(wildcard $(REP_INTEGRATION)/*.c)
# Les fichiers objets des tests compilés ont une extension .o et sont rangés dans
# "build/unitaires" ou "build/.integration", et non pas dans "tests/*"
TESTS_UNITAIRES := $(patsubst $(REP_UNITAIRES)/%.c, $(REP_BINAIRES_UNITS)/%.o, $(sources_unitaires))
TESTS_INTEGRATION := $(patsubst $(REP_INTEGRATION)/%.c, $(REP_BINAIRES_INTE)/%.o, $(sources_integration))

# Noms des targets/exécutables que l'utilisateur de Make peut générer
executables := scrutin verifier_mon_vote tests_unitaires tests_integration
# Pour chacun de ses exécutables, on redirige vers la target préfixée de "build/",
# qui est la target de fichier et qui se charger de la compilation et du linkage
# si nécessaire
$(executables): %: $(REP_BINAIRES)/%
# Les targets ne commençant pas par "build/" ne sont pas les vrais chemins des
# exécutables mais plutôt des raccourcis pour que l'utilisateur puisse les créer
.PHONY: $(executables)

# Programme principal : script main.c utilisant les modules
$(REP_BINAIRES)/scrutin: $(REP_SOURCES)/main.c $(MODULES)
	$(generer_executable)
# Consulation de son ballot de vote : script verifier_mon_vote.c qui utilise
# des fonctions déjà codées dans les modules programmés
$(REP_BINAIRES)/verifier_mon_vote: $(REP_SOURCES)/verifier_mon_vote.c $(MODULES)
	$(generer_executable)
# Tests unitaires : testent le comportement isolé des fonctions dans les différents
# modules
$(REP_BINAIRES)/tests_unitaires: tests/main_unitaires.c $(TESTS_UNITAIRES) $(MODULES)
	$(generer_executable)
# Tests d'intégration : testent le comportement sur le système et sur le reste du
# programme des fonctions dans les différents modules
$(REP_BINAIRES)/tests_integration: tests/main_integration.c $(TESTS_INTEGRATION) $(MODULES)
	$(generer_executable)

# Fichiers objets des modules compilés séparément dans "build" depuis "src"
$(MODULES): $(REP_BINAIRES)/%.o: $(REP_SOURCES)/%.c
	$(generer_objet)
# Fichiers objets des tests unitaires des modules compilés séparément dans
# "build/unitaires" depuis "tests/unitaires"
$(TESTS_UNITAIRES): $(REP_BINAIRES_UNITS)/%.o: $(REP_UNITAIRES)/%.c
	$(generer_objet)
# Fichiers objets des tests d'intégration des modules compilés séparément dans
# "build/integration" depuis "tests/integration"
$(TESTS_INTEGRATION): $(REP_BINAIRES_INTE)/%.o: $(REP_INTEGRATION)/%.c
	mkdir -p $(REP_BINAIRES_INTE)
	$(generer_objet)


# Nettoyer les binaires générés (.o et exécutables)
clean:
	rm -rf build/
# clean est une tâche à effectuer de nouveau à chaque fois que l'utilisateur le
# demande
.PHONY: clean

# Affiche un message d'aide quant à l'utilisateur de ce Makefile
help:
	@echo "Makefile du projet REV-party par Lélio CHARRIÈRE"
	@echo "Targets disponibles :"
	@echo "  scrutin           : Programme principale, exécution de scrutins"
	@echo "  verifier_mon_vote : Programme pour regarder la liste de votes d'un utilisateur"
	@echo "  tests_unitaires   : Tests unitaires pour les fonctions couvertes"
	@echo "  tests_integration : Tests d'intégration pour les fonctions qui le nécessitent"
# Comme pour clean, c'est une tâche de Makefile et aucun fichier n'est créé : il
# faut la réexécuter à chaque fois
.PHONY: help
