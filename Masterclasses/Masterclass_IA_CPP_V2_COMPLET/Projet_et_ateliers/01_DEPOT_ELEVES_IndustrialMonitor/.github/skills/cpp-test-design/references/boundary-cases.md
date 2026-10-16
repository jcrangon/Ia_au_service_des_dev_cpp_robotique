# Référentiel de tests adversariaux (chargement à la demande)

## Floating-point
À un seuil T : `std::nextafter(T,-INFINITY)`, T,
`std::nextafter(T,INFINITY)` ; ne pas choisir un epsilon arbitraire.
`NaN` ne se compare ni <, ni >, ni ==, même à lui-même.

## Temps
Âge -1, 0, max, max+1 ; établir ce que signifie « périmé ».

## Séquences
Pour chaque état, tester entrée, maintien, sortie, rebond et reprise.
Une validation simple de `classify` ne vérifie pas `Monitor::ingest`.

## Précautions
- Dériver l'attendu du cahier des charges.
- Ne pas auto-corriger un test pour correspondre au code.
- Signaler les hypothèses ouvertes.
