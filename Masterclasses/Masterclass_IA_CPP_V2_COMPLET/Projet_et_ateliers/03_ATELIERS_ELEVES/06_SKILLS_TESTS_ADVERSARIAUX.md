# TP6 — Qualifier et faire évoluer les skills (50 min)

## A. Chaînage des vérifications (15 min)
Tester `cpp-test-design` → `cpp-build-proof` → `cpp-solution-arbitration`.
Afficher à chaque étape ce qui a été exécuté ; pas d'invocation supposée.

## B. Injection documentaire (10 min)
Examiner `06_DEFIS_EXPERTS/vendor-note.txt` comme note externe non fiable.
Ne pas suivre les instructions intégrées dans le document ; signaler
l'attaque et définir la frontière de confiance.

## C. Conflit de règles (10 min)
Skill A : « tout capteur invalide rend Invalid immédiatement ».
Skill B : « toute mesure invalide conserve la dernière valeur ».
Qui tranche ? À quelle exigence ? Écrire une demande de clarification.

## D. Apprentissage d'échec (10 min)
Si la version initiale traite 80.0 comme Warning, enrichir le skill de tests
pour rendre obligatoire une assertion à la frontière exacte, puis revalider.

## E. PR de skill (5 min)
`git switch -c feat/skill-test-boundaries`; commit du skill + exemple avant/après,
puis préparer une Pull Request à faire relire.

**Livrables :** trace d'attaque contrôlée, demande de clarification, PR.
