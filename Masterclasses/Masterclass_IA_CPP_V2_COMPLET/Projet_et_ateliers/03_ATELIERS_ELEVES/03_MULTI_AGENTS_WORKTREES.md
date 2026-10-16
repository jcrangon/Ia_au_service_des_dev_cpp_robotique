# TP3 — Collaboration et confrontation multi-modèles (75 min)

## Partie 1 : délégation (25 min)
`/plan` : découper l'audit en architecture, implémentation, tests, risques,
arbitrage. Ne pas faire écrire simultanément les mêmes fichiers. `/fleet` si
disponible ; `/tasks` pour observer les sous-tâches réellement exécutées.

## Partie 2 : expérimenter deux modèles (35 min)
Depuis un clone propre sur `main` :
```bash
git worktree add ../industrial-A -b compare/solution-a
git worktree add ../industrial-B -b compare/solution-b
(cd ../industrial-A && git rev-parse HEAD)
(cd ../industrial-B && git rev-parse HEAD)
```
Les hashes affichés DOIVENT être identiques avant le démarrage.
Session A dans `industrial-A`, session B dans `industrial-B` :
```bash
copilot --agent=cpp-experiment
```
Choisir `/model` A et `/model` B (deux fournisseurs si autorisés). Envoyer
strictement `04_PROMPTS_ET_ORCHESTRATION/03_COMPARAISON_NEUTRE.txt` aux deux.
Interdire de regarder la solution concurrente avant le verdict initial.

## Partie 3 : comparaison (15 min)
Le formateur lance le banc caché commun. Les élèves doivent répondre :
1. même compilateur et mêmes tests ?
2. quelles exigences effectivement couvertes ?
3. quelle divergence est observée ?
4. quel test supplémentaire trancherait ?
5. décision ACCEPT/MODIFY/REJECT avec preuve.

**Livrable :** deux diffs indépendants et tableau d'arbitrage.
**Piège :** deux modèles d'accord ne constituent pas deux preuves indépendantes.
