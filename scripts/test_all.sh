#!/usr/bin/env bash
# Run the original Lab 1 tests from main, then the current Lab 2 tests.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
: "${ECO_FRAMEWORK:?Set ECO_FRAMEWORK to an absolute path to Eco.Framework first}"

if ! git -C "$ROOT" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  echo "ERROR: run this script from a Git checkout of lab_EcoInterface" >&2
  exit 2
fi
if ! git -C "$ROOT" show-ref --verify --quiet refs/remotes/origin/main; then
  echo "ERROR: origin/main not found. Run git fetch origin first." >&2
  exit 2
fi

TMP_PARENT="$(mktemp -d "${TMPDIR:-/tmp}/eco-labs.XXXXXXXX")"
LAB1_WORKTREE="$TMP_PARENT/lab1"
cleanup() {
  git -C "$ROOT" worktree remove --force "$LAB1_WORKTREE" >/dev/null 2>&1 || true
  rmdir "$TMP_PARENT" >/dev/null 2>&1 || true
}
trap cleanup EXIT

printf '\n========== LAB 1 / 12 Max-Min checks ==========\n'
git -C "$ROOT" worktree add --quiet --detach "$LAB1_WORKTREE" origin/main
make -C "$LAB1_WORKTREE/AssemblyFiles/Mac/clang_arm64" test
printf '[PASS] Lab 1: original 12 checks passed\n'

printf '\n========== LAB 2 / A, C, D configurations ==========\n'
make -C "$ROOT/AssemblyFiles/Mac/clang_arm64" test
printf '\n==================================================\n'
printf 'PASS: Lab 1 (12 checks) + Lab 2 (A/C/D)\n'
printf '==================================================\n'
