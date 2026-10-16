---
name: cpp-criticality-audit
description: Classify operational criticality of industrial C++ functions based on failure consequences, with evidence, controls and human escalation.
---


# Audit de criticité industrielle

## Quand déclencher ?
Lorsque l'on doit évaluer les conséquences d'un défaut d'une fonction C++
dans un contexte de surveillance, diagnostic ou commande industrielle.

## À lire avant toute conclusion
- `docs/criticality.md` : politique approuvée.
- `docs/requirements.md` : exigences applicables.
- Fonction et, si accessibles, ses appelants et sorties.

## Procédure explicite
1. Décrire ENTRÉES, SORTIES, EFFETS et dépendances.
2. Écrire une phrase « Si cette fonction se trompe, alors… ».
3. Imaginer une défaillance de valeur, de timing et de disponibilité.
4. Classer selon le contexte GREEN/ORANGE/RED et citer la règle.
5. Proposer pour chaque risque une preuve reproductible ou préciser le manque.
6. Lister les actions relevant obligatoirement de la revue humaine.

## Rejet obligatoire
Aucune « conformité sécurité » n'est attribuée sur la seule recommandation IA.
Une fonction informative réutilisée pour autoriser un mouvement doit être
réévaluée dans son nouveau contexte.

## Sortie contrôlable
| Fonction et usage | Conséquence | Niveau provisoire | Preuve disponible | Contrôle | Question ouverte |

## Critères de fin
Aucun niveau sans conséquence explicitée ; les inconnues sont nommées.
