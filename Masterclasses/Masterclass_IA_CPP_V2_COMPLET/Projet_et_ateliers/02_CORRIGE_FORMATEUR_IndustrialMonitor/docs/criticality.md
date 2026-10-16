# Politique pédagogique de criticité — version 1.0

> Les couleurs suivantes sont un outil d'analyse interne au TP, **pas** une
> certification, ni un substitut à l'analyse de risques réelle.

| Niveau | Conséquence typique | Vérifications minimales | Validation |
|---|---|---|---|
| GREEN | Affichage sans effet automatique | Revue + tests | Relecteur |
| ORANGE | Diagnostic guidant un opérateur | Frontières, erreurs, traçabilité, revue indépendante | Relecteur métier |
| RED | Erreur pouvant contribuer à un mouvement dangereux | Analyse de risques, plan de V&V, tests indépendants, validation normative applicable | Approbation humaine spécialisée |

**Règle cardinale :** on classe selon **l'usage et la conséquence d'une
éventuelle défaillance**, jamais selon le seul nom d'une fonction.

Si l'usage n'est pas connu, l'évaluation reste provisoire.
Aucun agent IA et aucun MCP ne peut accorder une autorisation de sécurité.
