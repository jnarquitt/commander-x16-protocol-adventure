# Project Charter

## Working title

**Protocol Adventure** is the neutral project name until the setting and commercial title are chosen.

## Product promise

Create a genuine Commander X16 adventure RPG that feels as if an ambitious studio made it in the late 1980s with unusually capable graphics hardware:

- top-down map exploration;
- a visible, animated player sprite;
- towns, interiors, wilderness, hazards, clues, and NPC conversations;
- uncertain travel punctuated by random and authored encounters;
- a dedicated combat screen with large portraits or creature art;
- character growth expressed through Protocol Dice Abilities and Skills;
- fast controls, readable text, strong atmosphere, and meaningful choices.

## Locked foundation

The following is authoritative unless Joshua explicitly changes it:

- Target platform: Commander X16 hardware and the official emulator.
- Core loop: explore a map, trigger encounters, switch to a combat screen, resolve consequences, and return to exploration.
- Every learned skill uses Protocol Dice mechanics.
- Protocol Dice treat faces 1–3 as blank and total faces 4+.
- The presentation evokes an 1980s computer adventure rather than pretending to be a modern pixel-art console game.

## Deliberately undecided

- final title and setting;
- single hero versus controllable party;
- fantasy, modern supernatural, science fiction, or hybrid genre;
- real-time versus menu-driven combat presentation (rules remain turn-based);
- mouse support at launch;
- cartridge release in addition to SD-card/disk release.

## Design guardrails

- Build one complete area before building a continent.
- Prove a rule with placeholder assets before producing final art.
- Do not make random encounters pure attrition: use encounter tables with discoveries, NPCs, hazards, choices, and combat.
- Keep text readable on CRT, VGA, and emulator windows.
- Never assume emulator-only behavior is safe on hardware.
- Match emulator, ROM, and VERA release families during testing.

## Release rule

Every milestone receives a unique version, changelog, known-issues list, PRG, source package, and SHA-256 manifest. A release candidate must be tested in both the official emulator and real hardware when hardware is available.

