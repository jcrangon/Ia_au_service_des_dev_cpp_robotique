# Contract of the external MCP tool
Expected tool name: search_code (name depends on server installation).
Expected arguments: query (string), top_k (integer).
Expected evidence fields: path, start_line, end_line, snippet, score.
Scores are retrieval estimates, never probabilities of correctness.
Only consider the tool operational when its response was actually observed.
