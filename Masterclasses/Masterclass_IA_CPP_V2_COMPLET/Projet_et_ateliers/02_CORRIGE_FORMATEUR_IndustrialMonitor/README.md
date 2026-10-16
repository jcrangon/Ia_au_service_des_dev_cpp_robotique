# IndustrialMonitor — CORRIGÉ FORMATEUR

Projet C++20 pour le J3 « IA au service des ingénieurs C++ ».

- Version : **corrigée sur REQ-01 à REQ-10**.
- Plateformes : Linux natif / Ubuntu WSL2.
- Versionnement : Git / GitHub, dépôt privé conseillé.
- Code abondamment commenté dans `src/`, `include/`, `tests/`, `mcp/`,
  les scripts et configurations.
- Sans lien avec un robot réel ni sécurité fonctionnelle certifiée.

## Compiler et tester
```bash
bash scripts/build_test.sh
./build/monitor_demo
```

## Installer MCP Python (optionnel, avec réseau)
```bash
bash scripts/setup_python.sh
bash scripts/configure_mcp.sh
copilot mcp list
```

## Agents / skills
```bash
copilot
# Dans la session : /agent, /skills list, /model, /fleet, /usage
```

Les modèles souhaités dans `.github/agents/` doivent être remplacés ou leur
champ `model:` supprimé si les droits du compte ne les permettent pas. Les
scripts de formation NE configurent PAS automatiquement de modèles payants.

## Plugin local
```bash
copilot plugin install ./plugins/industrial-review
copilot plugin list
```

## Vérifier les skills
```bash
.venv/bin/python scripts/lint_skills.py
```

## Exigences et criticité
Lire `docs/requirements.md`, `docs/criticality.md`, `docs/architecture.md`.

La conformité n'est pas déduite d'un simple résultat « tests verts ».
