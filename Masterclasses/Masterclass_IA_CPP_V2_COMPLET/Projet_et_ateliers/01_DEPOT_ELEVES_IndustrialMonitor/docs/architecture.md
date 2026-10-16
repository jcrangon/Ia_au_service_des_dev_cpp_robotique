# Architecture : décision pédagogique

Deux responsabilités **différentes** sont représentées dans le corrigé :

1. **`classify(reading, limits)`** : calcul instantané, aucune mémoire de mesure,
   donc facile à tester par partitions, frontières et cas invalides.
2. **`Monitor::ingest(reading)`** : automate à état internalisé, donc tests de
   séquences requis. Le traitement des capteurs invalides est prioritaire.

La version de départ n'implémente volontairement que la première responsabilité,
avec plusieurs défauts. Les élèves doivent proposer et défendre l'architecture
nécessaire à REQ-08 et REQ-09.

## Contraintes non couvertes par ce TP
- aucune connexion au robot ;
- pas de temps réel dur démontré ;
- pas de thread-safety ;
- pas de processus de certification ;
- pas de décision automatique de commande.
