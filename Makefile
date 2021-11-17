# Les fichiers d'en-têtes .h sont rangés dans un répertoire à part "include"
CFLAGS := -Iinclude/

# Fonction appelée pour compiler l'exécutable de la target courante et afficher
# un message à l'utilisateur
define generer_executable
$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ -o $(REP_BINAIRES)/$@
@echo "Exécutable $@ généré dans le répertoire $(REP_BINAIRES) !"
endef

# Fonction appelée pour compiler le fichier .c de la target courante, en ignorant
# les autres dépendances qui ne sont pas des fichiers sources
define generer_objet
$(CC) -c $(CPPFLAGS) $(CFLAGS) $(filter %.c, $^) -o $@
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

# Programme principal : script main.c utilisant les modules
scrutin: $(REP_SOURCES)/main.c $(MODULES)
	$(generer_executable)
# Consulation de son ballot de vote : script verifier_mon_vote.c qui utilise
# des fonctions déjà codées dans les modules programmés
verifier_mon_vote: $(REP_SOURCES)/verifier_mon_vote.c $(MODULES)
	$(generer_executable)
# Tests unitaires : testent le comportement isolé des fonctions dans les différents
# modules
tests_unitaires: tests/main_unitaires.c $(TESTS_UNITAIRES) $(MODULES)
	$(generer_executable)
# Tests d'intégration : testent le comportement sur le système et sur le reste du
# programme des fonctions dans les différents modules
tests_integration: tests/main_integration.c $(TESTS_INTEGRATION) $(MODULES)
	$(generer_executable)

# Fichiers objets des modules compilés séparément dans "build" depuis "src"
$(MODULES): $(REP_BINAIRES)/%.o: $(REP_SOURCES)/%.c $(REP_BINAIRES)
	$(generer_objet)
# Fichiers objets des tests unitaires des modules compilés séparément dans
# "build/unitaires" depuis "tests/unitaires"
$(TESTS_UNITAIRES): $(REP_BINAIRES_UNITS)/%.o: $(REP_UNITAIRES)/%.c $(REP_BINAIRES) $(REP_BINAIRES_UNITS)
	$(generer_objet)
# Fichiers objets des tests d'intégration des modules compilés séparément dans
# "build/integration" depuis "tests/integration"
$(TESTS_INTEGRATION): $(REP_BINAIRES_INTE)/%.o: $(REP_INTEGRATION)/%.c $(REP_BINAIRES) $(REP_BINAIRES_INTE)
	mkdir -p $(REP_BINAIRES_INTE)
	$(generer_objet)

# Cette target est requise avant de générer n'importe quel fichier binaire,
# elle s'assure que le dossier de sortie "build" existe
# -p créer le répertoire uniquement s'il n'existe pas déjà
$(REP_BINAIRES):
	mkdir -p $(REP_BINAIRES)
# Même principe pour le dossier de sortie des objets des tests unitaires
$(REP_BINAIRES_UNITS):
	mkdir -p $(REP_BINAIRES_UNITS)
# Même principe pour le dossier de sortie des objets des tests d'intégration
$(REP_BINAIRES_INTE):
	mkdir -p $(REP_BINAIRES_INTE)

# Nettoyer les binaires générés (.o et exécutables)
clean:
	rm -rf build/
