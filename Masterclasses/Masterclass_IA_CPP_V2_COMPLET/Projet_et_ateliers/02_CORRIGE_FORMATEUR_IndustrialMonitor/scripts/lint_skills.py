#!/usr/bin/env python3
"""Lint basique des skills : syntaxe, nom et fichiers référencés.

Ce vérificateur n'évalue PAS si le skill est utile, sûr ou réellement exécuté.
Il échoue sur les erreurs structurelles et avertit sur certains risques.
"""
from pathlib import Path
import re
import sys
import yaml

ROOT = Path(__file__).resolve().parent.parent / '.github' / 'skills'
errors = []
warnings = []

for folder in sorted(p for p in ROOT.iterdir() if p.is_dir()):
    skill_file = folder / 'SKILL.md'
    if not skill_file.exists():
        errors.append(f'{folder.name}: SKILL.md absent')
        continue
    content = skill_file.read_text(encoding='utf-8')
    if not content.startswith('---\n'):
        errors.append(f'{folder.name}: frontmatter absent')
        continue
    parts = content.split('\n---\n', 1)
    if len(parts) != 2:
        errors.append(f'{folder.name}: frontmatter non fermé')
        continue
    try:
        metadata = yaml.safe_load(parts[0][4:])
    except yaml.YAMLError as exc:
        errors.append(f'{folder.name}: YAML invalide: {exc}')
        continue
    if not isinstance(metadata, dict):
        errors.append(f'{folder.name}: métadonnées non dictionnaire')
        continue
    if metadata.get('name') != folder.name:
        errors.append(f'{folder.name}: nom YAML différent du dossier')
    desc = metadata.get('description')
    if not isinstance(desc, str) or len(desc.strip()) < 40:
        warnings.append(f'{folder.name}: description peu discriminante')
    # Références explicites par chemin relatif à l'intérieur du skill.
    for ref in set(re.findall(r'(?<![\w/])(?:references|scripts)/[\w./-]+',parts[1])):
        if not (folder / ref).is_file():
            errors.append(f'{folder.name}: référence manquante {ref}')
    if 'allowed-tools:' in parts[0] and 'bash' in parts[0]:
        warnings.append(f'{folder.name}: vérifier les permissions shell')
    print(f'[INSPECTÉ] {folder.name}')

for item in warnings:
    print('[AVERTISSEMENT]', item)
for item in errors:
    print('[ERREUR]', item)
print(f'Bilan: {len(errors)} erreurs, {len(warnings)} avertissements')
sys.exit(1 if errors else 0)
