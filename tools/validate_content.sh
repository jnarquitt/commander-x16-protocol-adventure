#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CONTENT="$ROOT/src/game_content.c"

rows="$(sed -n '/const char\* const ash_map/,/^};/p' "$CONTENT" | sed -n 's/^[[:space:]]*"\([^"]*\)"[,;]*$/\1/p')"
count="$(printf '%s\n' "$rows" | awk 'NF { n++ } END { print n+0 }')"
[[ "$count" == "18" ]] || { printf 'FAIL: map has %s rows, expected 18\n' "$count" >&2; exit 1; }

bad="$(printf '%s\n' "$rows" | awk 'length($0) != 32 { print NR ":" length($0) }')"
[[ -z "$bad" ]] || { printf 'FAIL: map rows must be 32 columns: %s\n' "$bad" >&2; exit 1; }

for marker in C L T Q R V E; do
  found="$(printf '%s' "$rows" | tr -cd "$marker" | wc -c)"
  [[ "$found" == "1" ]] || { printf 'FAIL: marker %s appears %s times\n' "$marker" "$found" >&2; exit 1; }
done

printf 'PASS: Ash Ledger map dimensions and critical markers\n'

portrait_bytes="$(grep -Eo '0x[0-9A-F]{2}u' "$ROOT/src/generated_portraits.c" | wc -l)"
[[ "$portrait_bytes" == "2560" ]] || { printf 'FAIL: generated portraits contain %s bytes, expected 2560\n' "$portrait_bytes" >&2; exit 1; }
[[ -s "$ROOT/assets/generated/portrait-preview-sheet-v0.120.png" ]] || { printf 'FAIL: portrait QA sheet is missing\n' >&2; exit 1; }
printf 'PASS: five generated 32x32 4-bpp portraits\n'
