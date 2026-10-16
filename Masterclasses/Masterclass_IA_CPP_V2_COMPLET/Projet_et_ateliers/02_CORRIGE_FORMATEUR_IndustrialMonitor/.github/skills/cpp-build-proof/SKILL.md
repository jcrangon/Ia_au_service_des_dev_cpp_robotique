---
name: cpp-build-proof
description: Build the IndustrialMonitor C++ project with Clang and CTest, and produce reproducible evidence of compilation and test outcomes.
---


# Preuve de construction C++

## Principe
Un rapport compilatoire n'est pas une preuve de conformité fonctionnelle.
Ce skill ne doit jamais écrire « tests passés » sans avoir exécuté ces tests.

## Préconditions
- OS Linux ou WSL2 ; Clang, Ninja, CMake opérationnels.
- Répertoire du dépôt reconnu ; script `scripts/validate.sh` audité.
- Permission explicite pour utiliser le shell.

## Procédure
1. Lire le script `scripts/validate.sh` et la configuration CMake.
2. Demander l'accord avant l'exécution d'un script changé / non revu.
3. Lancer `bash .github/skills/cpp-build-proof/scripts/validate.sh`.
4. Relever compilateur, options, exit code, cas CTest et échecs éventuels.
5. Distinguer « compilation réussie » de « couverture des exigences suffisante ».

## Sortie
Environnement ; commit ; commandes ; code de sortie ; résultats ;
cas non couverts ; observations sans surinterprétation.
