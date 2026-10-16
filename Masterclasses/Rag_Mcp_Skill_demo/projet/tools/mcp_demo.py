"""Minimal runnable MCP tool; requires `python -m pip install mcp`."""
from pathlib import Path
from mcp.server.fastmcp import FastMCP

app = FastMCP('industrial-training')
ROOT = Path(__file__).resolve().parents[1]

@app.tool()
def search_code(query: str, top_k: int = 5) -> list[dict]:
    """Search source lines in the training repository (lexical stand-in, NOT vector RAG)."""
    if not query.strip():
        return []
    results=[]
    for path in sorted((ROOT/'src').glob('*.cpp')):
        for line_no, line in enumerate(path.read_text(encoding='utf8').splitlines(), 1):
            if query.lower() in line.lower():
                results.append({'path':str(path.relative_to(ROOT)),'start_line':line_no,
                                'end_line':line_no,'snippet':line,'score':None})
    return results[:max(1,min(top_k,20))]

@app.tool()
def index_health() -> dict:
    """Describe the demonstrator index; never claim pgvector is installed."""
    return {'backend':'local-lexical-demo','ready':(ROOT/'src').is_dir(),'vector_rag':False}

if __name__ == '__main__':
    app.run(transport='stdio')
