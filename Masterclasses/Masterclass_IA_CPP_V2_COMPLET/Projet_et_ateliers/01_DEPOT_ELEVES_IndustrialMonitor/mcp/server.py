"""
Serveur MCP de règles industrielles — exemple pédagogique (transport stdio).

Pourquoi un MCP ?
    Un agent IA peut *consulter* les exigences officielles au moyen d'un outil,
    plutôt que de prétendre les connaître. Chaque réponse comprend une trace
    (event_id) et une empreinte SHA-256 du fichier lu.

Propriétés / limites :
    - Aucune dépendance à un modèle LLM ; le serveur sert des données locales.
    - On ne fournit AUCUN outil de modification du code, de shell ou de GitHub.
    - Un journal local prouve qu'une fonction a été appelée, pas que l'agent
      a correctement interprété son résultat ni qu'il a respecté les risques.
    - Le contenu du fichier JSON n'est pas signé : hash != preuve d'approbation.
    - Les logs sont en local et peuvent être modifiés : ce n'est pas un audit WORM.

Installation : `bash scripts/setup_python.sh`
Enregistrement : `bash scripts/configure_mcp.sh`
"""
from __future__ import annotations

import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path
from uuid import uuid4

from mcp.server.fastmcp import FastMCP

# Nom publié au client Copilot ; le transport stdio ne nécessite pas de port.
mcp = FastMCP("industrial-rules")
ROOT = Path(__file__).resolve().parent.parent
SPEC_PATH = ROOT / "docs" / "requirements.json"
POLICY_PATH = ROOT / "docs" / "criticality.md"
AUDIT_PATH = ROOT / "mcp" / "tool-audit.log"


def record(tool: str, argument: str) -> str:
    """Créer une trace unique d'appel sans écrire sur stdout (réservé à MCP)."""
    event_id = uuid4().hex
    when = datetime.now(timezone.utc).isoformat()
    # L'entrée est contrôlée avant sérialisation pour éviter les retours ligne
    # injectés dans le journal. Ce journal reste exclusivement pédagogique.
    safe_arg = argument.replace("\n", " ").replace("\r", " ")[:120]
    with AUDIT_PATH.open("a", encoding="utf-8") as stream:
        stream.write(f"{when}|{event_id}|{tool}|{safe_arg}\n")
    return event_id


@mcp.tool()
def get_requirement(requirement_id: str) -> dict:
    """Lire une exigence approuvée et retourner version, hash et trace d'appel."""
    event_id = record("get_requirement", requirement_id)
    try:
        data = SPEC_PATH.read_bytes()
        spec = json.loads(data)
    except (OSError, ValueError) as exc:
        return {"status": "ERROR", "event_id": event_id, "detail": str(exc)}

    # Un document versionné peut diverger entre deux branches : on renvoie
    # donc une empreinte et la version, jamais uniquement un texte isolé.
    text = spec.get("requirements", {}).get(requirement_id)
    return {
        "status": "OK" if text is not None else "UNKNOWN",
        "event_id": event_id,
        "requirement_id": requirement_id,
        "text": text,
        "spec_version": spec.get("version"),
        "sha256": hashlib.sha256(data).hexdigest(),
    }


@mcp.tool()
def get_criticality_rule(level: str) -> dict:
    """Consulter la politique de criticité, sans attribuer d'approbation."""
    normalized = level.strip().upper()
    event_id = record("get_criticality_rule", normalized)
    try:
        content = POLICY_PATH.read_bytes()
    except OSError as exc:
        return {"status": "ERROR", "event_id": event_id, "detail": str(exc)}

    if normalized not in {"GREEN", "ORANGE", "RED"}:
        return {"status": "UNKNOWN", "event_id": event_id, "level": normalized}
    return {
        "status": "OK",
        "event_id": event_id,
        "level": normalized,
        "policy": content.decode("utf-8"),
        "sha256": hashlib.sha256(content).hexdigest(),
    }


if __name__ == "__main__":
    # MCP stdio : ne surtout pas utiliser print() sur stdout, ce qui casserait
    # le protocole ; FastMCP gère directement les flux standards.
    mcp.run()
