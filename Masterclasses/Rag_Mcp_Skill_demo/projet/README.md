# Skills & MCP — industrial C++ training repository
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure # EXPECTED FAILURE: demonstration bug
python3 tools/validate_skills.py
python3 .github/skills/cpp-risk-audit/scripts/req_check.py
```
The MCP demo is intentionally lexical, not pgvector RAG. Replace `search_code` with the approved RAG MCP from the previous course to exercise actual vector retrieval.
```bash
python3 -m venv .venv
. .venv/bin/activate
python -m pip install 'mcp>=1,<3'
python tools/mcp_demo.py # stdio: no human-readable output expected
```
Register MCP with `copilot mcp add industrial-training -- /ABS/PATH/TO/.venv/bin/python /ABS/PATH/TO/tools/mcp_demo.py`.
Do not confuse shell commands with commands typed inside interactive Copilot.
