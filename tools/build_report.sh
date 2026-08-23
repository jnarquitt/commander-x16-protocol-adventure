#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PRG="$ROOT/build/PROTOCOL.PRG"

[[ -f "$PRG" ]] || { printf 'No PRG. Run ./x16 build first.\n' >&2; exit 1; }

printf 'PROTOCOL SEVEN X16 BUILD REPORT\n'
printf 'PRG bytes: %s\n' "$(wc -c < "$PRG")"
printf 'PRG SHA-256: %s\n' "$(sha256sum "$PRG" | awk '{print $1}')"
printf 'Load header: %s\n' "$(od -An -tx1 -N2 "$PRG" | tr -d ' \n')"
printf '\nVRAM OWNERSHIP\n'
printf '$13000-$130FF  player animation frames\n'
printf '$13100-$134FF  actor and portrait sprites\n'
printf '$1B000-$1EBFF  KERNAL text screen region\n'
printf '$1F000-$1F7FF  KERNAL/custom charset\n'
printf '$1F9C0-$1F9FF  VERA PSG voices\n'
printf '$1FC00-$1FFFF  sprite attributes\n'
printf '\nSAVE SCHEMA\n'
printf 'P7ASH.SAV version 1, device 8, checksum protected\n'

