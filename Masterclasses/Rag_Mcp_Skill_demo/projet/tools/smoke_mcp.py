"""Integration smoke test using MCP stdio transport; requires mcp SDK."""
import asyncio
import sys
from pathlib import Path
from mcp import ClientSession, StdioServerParameters
from mcp.client.stdio import stdio_client

async def main():
    program = Path(__file__).resolve().with_name("mcp_demo.py")
    params = StdioServerParameters(command=sys.executable, args=[str(program)])
    async with stdio_client(params) as (read, write):
        async with ClientSession(read, write) as session:
            await session.initialize()
            tools = await session.list_tools()
            names = [t.name for t in tools.tools]
            print("TOOLS", names)
            assert "search_code" in names and "index_health" in names
            result = await session.call_tool("search_code", {"query":"can_move", "top_k":3})
            print("RESULT", result)
            assert not result.isError

if __name__ == "__main__":
    asyncio.run(main())
