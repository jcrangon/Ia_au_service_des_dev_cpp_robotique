# IM-011 — Température inférieure au zéro absolu

**Diffuser uniquement à 16 h 25.**

Un capteur peut remonter une température finie, mais physiquement impossible :
T < -273,15 °C. Une telle mesure doit produire Invalid. T == -273,15 est
acceptée au regard de la contrainte REQ-11 uniquement. Les exigences déjà
approuvées continuent de s'appliquer.

## Questions techniques
- Réutiliser ou modifier valid_reading ?
- Une mesure invalide pendant la récupération réinitialise-t-elle le compteur ?
- Faut-il mettre à jour le JSON MCP et le Markdown de la spécification ?
- Quels tests `nextafter` reproduisent la frontière, sans epsilon inventé ?
- La nouvelle règle doit-elle être traitée avant ou après `std::isfinite` ?

## Évidence exigée
Code, test minimum, CTest, nouvelle source MCP, hash, PR, analyse de risque.
