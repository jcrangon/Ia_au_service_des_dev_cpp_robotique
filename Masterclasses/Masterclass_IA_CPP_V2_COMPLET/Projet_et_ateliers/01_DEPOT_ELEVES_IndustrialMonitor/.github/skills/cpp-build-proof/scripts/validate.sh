#!/usr/bin/env bash
# Script auditable exécuté par le skill cpp-build-proof.
# Ne modifie pas le code source ; seuls les artefacts build/ sont créés.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)"
echo "[INFO] Repository: $ROOT"
echo '[1/3] Configuration Clang C++20'
cmake -S "$ROOT" -B "$ROOT/build" -G Ninja \
  -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
echo '[2/3] Compilation'
cmake --build "$ROOT/build" --parallel 2
echo '[3/3] CTest'
ctest --test-dir "$ROOT/build" --output-on-failure
echo '[SUCCESS] Étapes ci-dessus exécutées avec code retour 0.'
