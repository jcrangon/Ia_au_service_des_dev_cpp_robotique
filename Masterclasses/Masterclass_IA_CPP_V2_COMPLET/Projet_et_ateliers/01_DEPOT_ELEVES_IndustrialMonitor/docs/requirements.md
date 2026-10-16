# IndustrialMonitor — cahier des charges approuvé (version 1.0.0)

## Périmètre
Composant pédagogique de supervision thermique. Il **n'agit pas** sur un moteur,
un frein, un relais ou une fonction de sûreté réelle.

## Modèle de données
`Reading` : température en °C (`double`), âge en millisecondes (`int`),
capteur valide (`bool`). `Limits` : seuils Warning/Critical et seuils de
retour, plus l'âge maximum autorisé.

## Exigences de classification instantanée
- **REQ-01** : mesure valide et T < `warning_enter` ⇒ Normal.
- **REQ-02** : mesure valide, `warning_enter` ≤ T < `critical_enter` ⇒ Warning.
- **REQ-03** : mesure valide et T ≥ `critical_enter` ⇒ Critical.
- **REQ-04** : NaN et ±infini ⇒ Invalid.
- **REQ-05** : capteur signalé en panne ⇒ Invalid.
- **REQ-06** : âge < 0 ou > `max_age_ms` ⇒ Invalid ; égalité au maximum autorisée.
- **REQ-07** : seuils finis et strictement ordonnés :
  `warning_exit < warning_enter < critical_exit < critical_enter` ;
  `max_age_ms > 0`. Une configuration invalide ⇒ `classify` retourne Invalid ;
  le constructeur de `Monitor` doit la rejeter par exception.

## Machine à états (surveillance, jamais action sur robot)
- **REQ-08** : hystérésis :
  - Normal → Critical si T ≥ critical_enter, sinon Warning si T ≥ warning_enter ;
  - Warning → Critical si T ≥ critical_enter, sinon Normal si T ≤ warning_exit ;
  - Critical → Warning si T ≤ critical_exit ;
  - autrement, conserver l'état courant ;
  - Critical ne saute **jamais** directement à Normal en une mesure valide.
- **REQ-09** : une entrée invalide déclenche immédiatement Invalid. Après Invalid,
  deux mesures **valides consécutives** sont exigées ; la première conserve
  Invalid, la deuxième reclasse selon la température courante. Une nouvelle
  entrée invalide remet le compteur de récupération à zéro.
- **REQ-10** : `classify` est pure et sans état interne ; `Monitor::ingest` peut
  modifier uniquement l'état de son instance.

## Questions ouvertes d'ingénierie
- L'état initial de `Monitor` est Normal par convention pédagogique. L'état
  initial réel doit être déterminé avec les ingénieurs sûreté.
- La fonction n'est pas thread-safe si une même instance est utilisée en
  concurrence : une politique de synchronisation reste à définir.
- Les horodatages et numéros de séquence ne sont pas encore traités.
- En cas de `Invalid`, c'est au **consommateur** de définir un comportement sûr.

## Contrôle
Les résultats d'IA doivent être confrontés à des tests indépendants ;
compilation seule, consensus entre agents ou score pondéré ne sont pas des preuves.
