# TP2 — Configurer une équipe de cinq agents (55 min)

## Dossier fourni
`.github/agents/` contient architecte, implémenteur, testeur, auditeur,
arbitre et un profil d'expérience neutre.

## Exercice A : inspecter (15 min)
Identifier dans chaque `.agent.md` : responsabilité, outils, modèle,
sources de vérité, interdits, preuves exigées, contradiction possible.
La présence de `tools: ["bash"]` n'est pas une sandbox à elle seule.

## Exercice B : adapter (15 min)
Modifier `cpp-safety-auditor` pour traiter le contexte « diagnostic seul »
versus « autorisation d'un mouvement dangereux ». Recharger l'agent.

## Exercice C : confrontation (15 min)
Même prompt de diagnostic, d'abord architecte, ensuite auditeur. Noter où
ils divergent et quel test permet de trancher.

## Exercice D : modèles (10 min)
Avec `/model`, confirmer les modèles autorisés par le compte. Remplacer les
valeurs `model:` qui ne sont pas réellement disponibles, conserver la preuve
qu'il s'agit d'IA différentes lorsque c'est possible.

**Livrable :** matrice agent / droit / modèle effectif / résultat / risque.
**Question piège :** « Ne modifie pas le code » dans un prompt empêche-t-il
techniquement un agent qui possède l'outil edit de le modifier ? **Non.**
