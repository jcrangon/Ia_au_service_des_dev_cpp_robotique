#!/usr/bin/env python3
"""Lightweight offline evidence collector, not a safety certifier."""
from pathlib import Path
import re, sys
root=Path(__file__).resolve().parents[4]
req=root/'docs/requirements.md'
if not req.exists():
 print('ERROR missing docs/requirements.md',file=sys.stderr);sys.exit(2)
found=re.findall(r'(?m)^REQ-\d+',req.read_text(encoding='utf8'))
for item in found: print(f'{item}: contract available; verify implementation and tests')
print('NOTE: no claim of compliance; inspect code and execute tests')
