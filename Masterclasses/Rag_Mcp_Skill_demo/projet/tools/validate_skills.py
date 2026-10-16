#!/usr/bin/env python3
from pathlib import Path
import re, sys
root=Path(__file__).resolve().parents[1]/'.github/skills'
fail=0
for path in sorted(root.glob('*/SKILL.md')):
    source=path.read_text(encoding='utf8')
    match=re.match(r'\A---\n(.*?)\n---\n',source,re.S)
    ok=bool(match and re.search(r'(?m)^name: '+re.escape(path.parent.name)+r'$',match.group(1))
            and re.search(r'(?m)^description: .+',match.group(1)))
    print(('PASS' if ok else 'FAIL'),path.relative_to(root))
    fail += not ok
sys.exit(1 if fail else 0)
