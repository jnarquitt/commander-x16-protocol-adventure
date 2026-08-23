# Project Charter

## Working title

**Protocol Seven: The Ash Ledger — Commander X16 RPG**

## Product promise

Create a polished, story-driven Commander X16 RPG with the emotional clarity and visual grammar of the finest 16-bit console adventures:

- top-down map exploration;
- a visible, animated player sprite;
- towns, interiors, wilderness, hazards, clues, and NPC conversations;
- authored investigation, rescue, dialogue, exploration, and conflict scenes;
- dedicated Protocol Dice combat with party and opponent presentation;
- character growth expressed through Protocol Dice Abilities and Skills;
- fast controls, readable text, strong atmosphere, and meaningful choices.

## Locked foundation

The following is authoritative unless Joshua explicitly changes it:

- Target platform: Commander X16 hardware and the official emulator.
- Core loop: explore a map, trigger encounters, switch to a combat screen, resolve consequences, and return to exploration.
- Every learned skill uses Protocol Dice mechanics.
- Protocol Dice treat faces 1–3 as blank and total faces 4+.
- Setting and campaign: Protocol Seven, beginning with *The Continuance Files* and *The Ash Ledger*.
- Story structure follows Briefing through Historical Verification.
- Carrier and Exposure choices must matter alongside survival.
- Presentation target: an excellent SNES-style narrative RPG adapted honestly to Commander X16 hardware.

## Deliberately undecided

- final commercial title treatment and subtitle hierarchy;
- precise party-following and party-switching field behavior;
- final encounter frequency outside authored campaign scenes;
- final portrait, tile, music, and sound asset style;
- mouse support at launch;
- cartridge release in addition to SD-card/disk release.

## Design guardrails

- Build one complete area before building a continent.
- Prove a rule with placeholder assets before producing final art.
- Treat real historical disasters with urgency and humanity, never as spectacle.
- Do not reduce missions to combat attrition; investigation, ethics, Carrier, and Exposure are equally real play.
- Keep text readable on CRT, VGA, and emulator windows.
- Never assume emulator-only behavior is safe on hardware.
- Match emulator, ROM, and VERA release families during testing.

## Release rule

Every milestone receives a unique version, changelog, known-issues list, PRG, source package, and SHA-256 manifest. A release candidate must be tested in both the official emulator and real hardware when hardware is available.
