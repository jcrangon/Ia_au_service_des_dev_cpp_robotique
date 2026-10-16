#!/usr/bin/env bash
# ============================================================================
# Comparaison reproductible de deux implémentations indépendantes.
# Usage : ./compare.sh /chemin/worktree-A /chemin/worktree-B
# Tous les tests utilisent le MÊME banc d'acceptation, indépendant des agents.
# Le script termine en erreur si au moins une solution ne passe pas.
# ============================================================================
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 PROJET_A PROJET_B" >&2; exit 2
fi
REPORTS="$HERE/results"
mkdir -p "$REPORTS"
ANY_FAIL=0

for label in A B; do
  if [ "$label" = A ]; then candidate="$1"; else candidate="$2"; fi
  project="$(realpath "$candidate")" || exit 2
  echo "========== CANDIDAT $label =========="
  echo "Dossier : $project"
  if [ ! -f "$project/CMakeLists.txt" ]; then
    echo "CMakeLists.txt absent pour $label" >&2
    ANY_FAIL=1; continue
  fi

  # 1. Recompiler la solution avec un répertoire dédié et le même compilateur.
  if ! cmake -S "$project" -B "$REPORTS/build-$label" -G Ninja \
      -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug \
      > "$REPORTS/$label-configure.log" 2>&1; then
    echo "CONFIGURATION ÉCHOUÉE ($label)"; ANY_FAIL=1; continue
  fi
  if ! cmake --build "$REPORTS/build-$label" --parallel 2 \
      > "$REPORTS/$label-build.log" 2>&1; then
    echo "COMPILATION ÉCHOUÉE ($label)"; ANY_FAIL=1; continue
  fi

  # 2. Exécuter les tests internes sans interpréter un échec comme un succès.
  if ! ctest --test-dir "$REPORTS/build-$label" --output-on-failure \
      > "$REPORTS/$label-ctest.log" 2>&1; then
    echo "CTEST ÉCHOUÉ ($label)"; ANY_FAIL=1
  else
    echo "CTEST PASSÉ ($label)"
  fi

  # 3. Construire le banc externe contre la même implémentation candidate.
  # On ne dépend pas du nom/librairie statique produit par le build CMake.
  if ! clang++ -std=c++20 -Wall -Wextra -Wpedantic \
       -I "$project/include" \
       "$HERE/acceptance_classify.cpp" \
       "$project/src/monitor.cpp" \
       -o "$REPORTS/acceptance-$label" \
       > "$REPORTS/$label-external-build.log" 2>&1; then
    echo "BANC EXTERNE : COMPILATION ÉCHOUÉE ($label)"
    ANY_FAIL=1; continue
  fi
  if "$REPORTS/acceptance-$label" > "$REPORTS/$label-acceptance.log" 2>&1; then
    echo "BANC EXTERNE PASSÉ ($label)"
  else
    echo "BANC EXTERNE ÉCHOUÉ ($label)"
    ANY_FAIL=1
  fi
  grep 'TOTAL' "$REPORTS/$label-acceptance.log" || true
  echo "Rapport : $REPORTS/$label-acceptance.log"
done
exit "$ANY_FAIL"
