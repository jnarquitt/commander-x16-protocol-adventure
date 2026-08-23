# Protocol Seven: The Ash Ledger — Commander X16 RPG

A story-driven, SNES-style roleplaying game for Commander X16, set in the Protocol Seven universe and adapting *The Continuance Files* adventure path.

## Play the latest build

The project page automatically publishes the newest compiled Commander X16 program:

**https://jnarquitt.github.io/commander-x16-protocol-adventure/**

Download `PROTOCOL.PRG` there and run it with a matching official Commander X16 emulator and ROM.

## Project status

Version **0.120 — ART DIRECTION AND WORLD DETAIL / PRE-ALPHA** adds the first converted character art, environmental motion, party presentation, field music, and manual checkpoint controls.

The vertical slice includes:

- Mara, Eli, and Nia as the opening Vector party;
- a 1906 San Francisco exploration chapter;
- rescue and investigation routes through the Insurance Annex and Telegraph Exchange;
- Miriam Quill, Elias Rourke, Silas Venn, and the Blue Ledger;
- Carrier, Exposure, Fire Clock, and remembered-history choices;
- a 3 AP Protocol Dice conflict with physical and social victory paths;
- extraction, Historical Verification, and the seven-stroke mystery.
- custom terrain glyphs and independent actor sprites;
- speaker portraits, scene fades, and native VERA PSG cues;
- joystick-native title and combat menus;
- checksum-protected chapter checkpoints on device 8.
- an original five-character portrait direction sheet and deterministic X16 converter;
- 32×32 portraits for Mara, Eli, Nia, Quill, and Venn;
- Eli and Nia following Mara on the field;
- animated fire and water, skippable typewriter dialogue, and a looping two-voice theme;
- manual checkpoint recording with keyboard C or joystick button B.

## Start here

From the repository folder, run:

```bash
./x16 doctor
./x16 setup
./x16 run
```

The `x16` Bash command center detects the local toolchain and emulator, builds the game, runs checks, downloads public builds, and publishes through Git. See `docs/08_DEVELOPMENT_CONTROLS.md` for the full control system.

Then read `docs/01_GAME_DESIGN_BIBLE.md` for the game vision and `docs/02_PROTOCOL_DICE_VIDEO_GAME_RULES.md` for the digital rules.

## Authoritative order

When documents conflict, use this order:

1. Current Protocol Seven canonical rules and active locks
2. Current *Continuance Files* and *Ash Ledger* campaign sources
3. `docs/09_PROTOCOL_SEVEN_RPG_REBOOT.md`
4. `docs/10_ASH_LEDGER_CHAPTER_PLAN.md`
5. `PROJECT_INSTRUCTIONS.md`
6. Technical architecture and content standards
7. `DECISIONS.md`
8. Current source code and tests
9. `PROJECT_STATUS.md` and `BACKLOG.md`

No rules in this new adaptation are frozen yet. Changes must be recorded in `CHANGELOG.md` until the first playtest lock.

## Repository map

```text
docs/       Design, rules, X16 reference, architecture, content standards
src/        C and 65C02 source
include/    Shared headers
assets/     Source art, maps, palettes, music, and generated binary assets
tools/      Host-side conversion and validation tools
tests/      Host-side deterministic rules tests and emulator test notes
build/      Generated files; never hand-edit
release/    Versioned distributable disk images and ZIPs
```

## Scope of v0.120

This is a compressed proof of the complete opening chapter, not the finished campaign. The next milestone is **v0.130 Cinematic Scene and Map Expansion**: dedicated 16×16 VERA maps, mission interiors, portrait expressions, cinematic event effects, additional musical themes, and three checkpoint slots.
