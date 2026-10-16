# TP1 — Prompts d'ingénierie et contre-expertise (45 min)
**Mission :** transformer « Corrige classify, rends robuste » en mandat auditable.

## Exercice A (10 min) : liste des dix ambiguïtés
Sur le code `src/monitor.cpp`, décrire : exigences, standard, périmètre,
contraintes d'API, effets de bord, droits d'outils, seuils, tests attendus,
critères d'acceptation et procédure en cas de spécification manquante.

## Exercice B (15 min) : prompt P-R-O-U-V-E
Écrire un prompt avec Périmètre, Règles, Objectif, Usage des outils,
Validation, Exceptions. Demander une table REQ / défaut / cas / conséquence.
À cette étape, interdire toute modification de code.

## Exercice C (20 min) : essai comparatif
Session A : prompt simple. Session B : votre prompt d'ingénierie.
Même modèle, commit, droits et demandes. Conserver deux transcriptions.

**Question :** combien de défauts *confirmés*, de contre-exemples et de fausses
alertes ? Ne pas noter « meilleur » sur longueur ou confiance apparente.

**Livrable :** `reports/prompt-comparison.md` (2 prompts, 2 résultats, méthode,
écarts et cas non détectés). Prolongement : écrire un prompt hostile chargé de
rechercher le défaut le plus dangereux d'une solution pourtant « validée ».
