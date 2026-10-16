#!/usr/bin/env bash
# Diagnostic non destructif de la station Linux ou WSL2.
# Ce script ne tente pas d'installer des outils ni d'ouvrir de compte GitHub.
set -u
ERRORS=0
for tool in git gh clang++ cmake ninja python3 jq copilot; do
  if command -v "$tool" >/dev/null 2>&1; then
    printf '[OK] %-10s %s\n' "$tool" "$(command -v "$tool")"
  else
    printf '[ABSENT] %s\n' "$tool"
    ERRORS=$((ERRORS + 1))
  fi
done
printf '\n[SYSTÈME] '; uname -srm
if grep -qi microsoft /proc/sys/kernel/osrelease 2>/dev/null; then
  echo '[INFO] Ubuntu sous WSL2 détecté.'
else
  echo '[INFO] Linux natif ou machine virtuelle.'
fi
if command -v gh >/dev/null 2>&1; then
  gh auth status >/dev/null 2>&1 && echo '[OK] gh authentifié' || \
    echo '[INFO] gh auth login nécessaire.'
fi
if [ -f CMakeLists.txt ]; then echo '[OK] Racine de projet probable.'; fi
printf '[RÉSULTAT] Outils manquants : %d\n' "$ERRORS"
exit "$ERRORS"
