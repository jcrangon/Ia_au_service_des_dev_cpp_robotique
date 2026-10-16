# TP7 — Challenge intégrateur IM-011 (35 min)

## Nouvelle exigence (NE PAS AJOUTER AVANT LE CHALLENGE)
Une température **finie** strictement inférieure à -273,15 °C doit produire
Invalid. Une température exactement égale à -273,15 °C est admissible au regard
de cette règle seulement. Les autres contrôles restent applicables.

## Consignes
1. Définir impact API / métier avec l'architecte.
2. Produire deux solutions indépendantes si possible.
3. Adapter la configuration et le traitement Invalid.
4. Construire des tests `nextafter(-273.15, ±INFINITY)`.
5. Passer Clang + CTest et analyse de criticité.
6. Interroger le MCP : vérifier aussi si REQ-11 est présent dans sa source.
7. Préparer PR GitHub + conclusion ACCEPT/MODIFY/REJECT.

## Résultat
Ne pas confondre absence de bug apparent, conformité aux exigences et sécurité.
Un outil MCP dont le dictionnaire ignore REQ-11 doit signaler UNKNOWN plutôt
qu'inventer la nouvelle règle. La version de spécification doit évoluer.
