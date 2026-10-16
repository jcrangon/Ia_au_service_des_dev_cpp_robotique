---
name: cpp-rag-audit
description: Audit C++ industrial requirements with the approved repository RAG MCP search tools when available, then verify returned excerpts against source files. Use for impact analysis or requirement-to-symbol tracing.
---
# Evidence-backed RAG + MCP audit
1. Read `docs/requirements.md` and `docs/criticality.md` first.
2. If available, invoke the MCP `search_code` tool for each target requirement or symbol. The MCP server must be configured separately.
3. Record every tool call query, tool response identifier, source path, and line range.
4. Open the actual repository files and verify snippets; RAG is discovery, not proof.
5. If the MCP tool is missing, say `MCP_NON_DISPONIBLE` and use local `rg -n` instead. Never invent tool output.
6. Verify the code against explicit requirements; propose precise executable tests.
7. Return a table: REQ | path:line | evidence | test | uncertainty | priority.
8. Never run deployments, modify safety-related production files or approve a safety change.
Read `references/rag-contract.md` before using MCP.
