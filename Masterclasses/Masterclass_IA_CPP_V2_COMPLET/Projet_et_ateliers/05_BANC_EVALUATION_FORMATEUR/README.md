# Vérification indépendante des variantes Copilot

Ce banc n'est pas publié dans le dépôt initial pour éviter qu'un candidat
ajuste son code ou ses tests uniquement aux cas cachés.

```bash
bash compare.sh ../industrial-A ../industrial-B
```

Le script utilise un CMake propre, Clang++ et le MÊME banc C++ pour les deux.
Il renvoie **1 si l'un au moins échoue**. Le projet initial doit échouer sur
plusieurs cas ; le corrigé du formateur doit les réussir.

Ce banc vérifie uniquement `classify` REQ-01 à REQ-07. Pour REQ-08/09, les
séquences d'automate sont testées séparément dans le projet corrigé.
Ce protocole limite certains biais mais ne démontre pas l'indépendance parfaite
des modèles ni la sûreté du logiciel.
