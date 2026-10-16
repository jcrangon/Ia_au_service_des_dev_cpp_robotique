#!/usr/bin/env bash
# Enregistre le serveur MCP local dans Copilot CLI.
# L'utilisateur doit avoir installé le venv et Copilot CLI au préalable.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PYTHON="$ROOT/.venv/bin/python"
SERVER="$ROOT/mcp/server.py"
if ! command -v copilot >/dev/null 2>&1; then
  echo 'ERREUR : GitHub Copilot CLI introuvable.' >&2; exit 1
fi
if [ ! -x "$PYTHON" ]; then
  echo 'ERREUR : venv absent ; voir scripts/setup_python.sh' >&2; exit 1
fi
# ATTENTION : copilot mcp add modifie la configuration utilisateur.
# Exécuter à la demande, pas depuis un script automatique de démarrage.
echo 'Ajout du serveur MCP industrial-rules à la configuration Copilot.'
copilot mcp add industrial-rules -- "$PYTHON" "$SERVER"
copilot mcp list
