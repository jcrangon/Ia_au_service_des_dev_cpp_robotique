---
name: cpp-risk-audit
description: Perform a read-only industrial C++ criticality audit against docs/requirements.md and docs/criticality.md. Use for REQ traceability, safety consequences, and independent test suggestions.
---
# Independent risk audit
1. Read `docs/requirements.md` and `docs/criticality.md` before source code.
2. Run the local checks using `python3 .github/skills/cpp-risk-audit/scripts/req_check.py`.
3. Inspect source and test code, then classify each finding as PROOF, HYPOTHESIS or UNKNOWN.
4. Provide two reproducible counter-examples with input, expected, observed, test command.
5. Classify criticality by consequence. Do NOT claim formal safety certification.
6. Format: REQ | evidence | test | consequence | decision.
Read `references/matrix.md` before finalizing.
