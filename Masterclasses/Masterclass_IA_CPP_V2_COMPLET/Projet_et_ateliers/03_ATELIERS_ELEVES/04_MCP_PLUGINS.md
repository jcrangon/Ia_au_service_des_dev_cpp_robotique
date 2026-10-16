# TP4 — Plugins et MCP contrôlés (50 min)

## Préparation
```bash
bash scripts/setup_python.sh
bash scripts/configure_mcp.sh
copilot mcp list
```

## Mission A — trace MCP réelle
Dans Copilot, demander `get_requirement("REQ-03")`. Relever `event_id`,
version et SHA-256 ; ouvrir `mcp/tool-audit.log` et vérifier la correspondance.
La répétition de la phrase « REQ-03 = Critical à 80 » ne suffit pas.

## Mission B — interruption
```bash
copilot mcp disable industrial-rules
```
Nouvelle session : demander un nouvel event_id. L'outil n'est plus disponible ;
un agent doit distinguer une information déjà connue d'un appel réellement fait.
Réactiver ensuite via `copilot mcp enable industrial-rules`.

## Mission C — plugin
```bash
copilot plugin install ./plugins/industrial-review
copilot plugin list
```
Retrouver l'agent et le skill ; modifier la consigne de revue, réinstaller,
relancer la session, vérifier le comportement.

**Livrables :** trace d'appel et audit : fournisseur, composants, permissions,
risques du plugin. **Question :** un plugin local devient-il « sûr » parce
qu'il n'a pas de connexion réseau déclarée ? **Non.**
