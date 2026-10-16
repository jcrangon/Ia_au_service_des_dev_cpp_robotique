---
name: cpp-test-design
description: Derive adversarial C++ tests from requirements, with numeric boundaries, invalid floating points, recovery sequences and independent expected outcomes.
---


# Conception de tests C++ pilotée par les exigences

## Déclenchement
Quand on conçoit, complète ou réévalue une campagne de tests C++.

## Documents
Lire `docs/requirements.md` puis `references/boundary-cases.md`.

## Algorithme d'analyse
1. Construire une matrice REQ / comportement attendu / classes d'équivalence.
2. Pour chaque comparaison de seuil : tester <, = et > avec `std::nextafter`.
3. Tester `NaN`, +infini, -infini, âge négatif, âge maximum et +1.
4. Pour REQ-08/09, construire des tests de SÉQUENCE : A→B→A, Invalid→valid,
   Invalid→valid→Invalid→valid→valid.
5. Vérifier les préconditions et les échecs attendus du constructeur.
6. Créer une suite minimale « red » sur le code initial puis « green » sur le
   correctif, SANS modifier les attendus quand le test échoue.

## Principe fondamental
L'implémentation ne détermine pas l'attendu ; seule la spécification le fait.
Une question ouverte se transforme en demande de décision, pas en test inventé.

## Livrable
| REQ | Classe d'entrée | Paramètres | Attendu | Observé | Commande | Statut |

## Critère de réussite
Aucune case « validée » sans exécution réelle du test ou justification
alternative explicite ; garder visibles les exigences non couvertes.
