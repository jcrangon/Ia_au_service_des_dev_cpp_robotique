# Incidents contrôlés pour binômes avancés

1. **Agent avec modèle invalide :** remplacer temporairement son `model:`
   par un nom absent de `/model` ; vérifier l'erreur ou le fallback.
2. **Skill non rechargé :** modifier une règle et garder la même session.
   Comparer avec une session neuve.
3. **MCP coupé :** désactiver industrial-rules et demander un nouvel event_id.
4. **Plugin obsolète :** changer un fichier du plugin mais ne pas réinstaller.
5. **Injection dans une note externe :** utiliser `vendor-note.txt`.
6. **Test trompeur :** garder 4 tests verts sur le programme non conforme.
7. **Deux versions de spécifications :** branche A version 1.0, B 1.1.
8. **Contradiction de skills :** Invalid immédiat vs conservation du dernier état.
9. **Modification parallèle du même fichier :** imposer git worktree et diff.
10. **Réponse attribuée au mauvais modèle :** vérifier le fournisseur réellement actif.

Tous ces essais doivent se faire sur une branche, avec aucun matériel réel
et sans modifier le dépôt principal protégé.
