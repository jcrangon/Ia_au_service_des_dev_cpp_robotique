#!/usr/bin/env bash
# Dépendances Python minimales pour le serveur MCP et le contrôle des skills.
# Réseau nécessaire uniquement pendant pip install.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
python3 -m venv "$ROOT/.venv"
"$ROOT/.venv/bin/python" -m pip install -r "$ROOT/mcp/requirements.txt"
echo 'Environnement prêt : .venv/bin/python'
