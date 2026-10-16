#!/usr/bin/env bash
# ============================================================================
# Compilation et tests — procédure reproductible et explicite.
# L'option -e arrête immédiatement sur erreur, -u sur variable absente,
# pipefail sur erreur d'une commande dans un pipeline.
# ============================================================================
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${ROOT}/build"

# La configuration explicite le compilateur : ne pas dépendre du PATH du cours.
echo '[1/3] CMake configure (Clang + Ninja)'
cmake -S "$ROOT" -B "$BUILD" -G Ninja \
  -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug

echo '[2/3] Build'
cmake --build "$BUILD" --parallel 2

echo '[3/3] Tests CTest'
ctest --test-dir "$BUILD" --output-on-failure

echo '[OK] Les commandes lancées ci-dessus ont retourné 0.'
