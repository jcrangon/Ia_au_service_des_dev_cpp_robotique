---
name: cpp-solution-arbitration
description: Compare competing AI-generated C++ implementations with identical reproducible tests, blocking defects and explicit evidence levels.
---


# Arbitrage de solutions C++ indépendantes

## Conditions préalables
Les solutions comparées partent d'un même commit et de la même spécification.
Lire `references/scoring.md` et les rapports de validation.

## Étapes
1. Confirmer l'indépendance des worktrees, prompts et modèles réellement actifs.
2. Comparer les diffs et les décisions de conception.
3. Vérifier que les compilateurs et tests utilisés sont comparables.
4. Distinguer E0 affirmation / E1 raisonnement / E2 test / E3 reproduction
   indépendante / E4 preuve approuvée selon le référentiel du TP.
5. Chercher au moins trois tests discriminants, reproductibles.
6. Calculer un score informatif ; écarter tout candidat à défaut bloquant.
7. Proposer ACCEPT/MODIFY/REJECT en expliquant les informations inconnues.

## Interdictions
Pas de choix fondé sur réputation d'un modèle, consensus ou ton affirmatif.
Pas de faux benchmark ni de validation de sûreté sans revue humaine.

## Livrable
Matrice A/B, tests identiques, preuves et limites, décision motivée.
