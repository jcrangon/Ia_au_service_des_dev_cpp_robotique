# Barème d'arbitrage — accompagnement, pas autorité de sûreté

| Axe | Poids |
|---|---:|
| Exactitude fonctionnelle | 30 % |
| Comportements invalides / défaillances | 25 % |
| Couverture des exigences | 20 % |
| Maintenabilité | 15 % |
| Ressources mesurées | 10 % |

Chaque axe : score de 0 à 5, ou **NON MESURÉ**.
Score total informatif : somme(poix * score / 5).

**Veto :** un défaut bloquant confirmé rend le candidat non acceptable,
même si son total numérique paraît excellent.

Niveaux de preuve :
- E0 : affirmation non étayée ;
- E1 : raisonnement statique ;
- E2 : test reproductible ;
- E3 : reproduction indépendante ;
- E4 : preuve approuvée par le processus humain.
