# Commander X16 Protocol Adventure

A 1980s-style exploration and combat game for the Commander X16, powered by the Protocol Dice skill system.

## Play the latest build

The project page automatically publishes the newest compiled Commander X16 program:

**https://jnarquitt.github.io/commander-x16-protocol-adventure/**

Download `PROTOCOL.PRG` there and run it with a matching official Commander X16 emulator and ROM.

## Project status

Version **0.010 — VERA VERTICAL SLICE / PRE-ALPHA** establishes the first graphical game loop on Commander X16 hardware.

The vertical slice now includes:

- a colored VERA tilemap and animated hardware player sprite;
- **WASD** and standard Commander X16 joystick controls;
- collision, investigation, a locked gate, and an artifact objective;
- regional threat accumulation and random combat encounters;
- Attack, Guard, Medkit, and Flee commands;
- visible Protocol Dice pools where results of 1–3 are blank and results of 4+ are totaled.

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

1. `docs/00_PROJECT_CHARTER.md`
2. `docs/02_PROTOCOL_DICE_VIDEO_GAME_RULES.md`
3. `docs/01_GAME_DESIGN_BIBLE.md`
4. `docs/04_TECHNICAL_ARCHITECTURE.md`
5. `docs/05_CONTENT_AND_DATA_STANDARD.md`
6. `PROJECT_INSTRUCTIONS.md`
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

## Scope of v0.010

This remains a pre-alpha vertical slice, not the finished game. The next required milestone is **v0.020 Content Systems**: externalized maps and dialogue, multiple encounter types, save/load, audio, inventory data, and host-side content converters.
