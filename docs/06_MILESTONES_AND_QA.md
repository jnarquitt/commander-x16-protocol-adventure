# Milestones and QA

## Historical prototypes

| Version | Result |
|---|---|
| v0.001 | Toolchain, documentation, and text proof |
| v0.010 | VERA map, hardware sprite, and first combat loop |
| v0.020 | Reusable content and developer controls |

These builds proved the platform. Their Amber Wilds content is not Protocol Seven story canon.

## Protocol Seven RPG milestones

| Version | Goal |
|---|---|
| v0.100 | Playable *Ash Ledger* opening chapter vertical slice |
| v0.110 | Custom tiles, party/NPC sprites, portraits, transitions, first audio, checkpoint save |
| v0.200 | Complete Mission 01 adaptation with all major Carrier and Exposure routes |
| v0.300 | Full RPG systems and Mission 02 content pipeline proof |
| v0.500 | Complete *Continuance Files* campaign alpha |
| v0.800 | Content-complete beta and real-hardware campaign |
| v1.000 | Release master |

## v0.100 acceptance criteria

- boots from PRG in the official emulator;
- title and briefing identify Protocol Seven, The Ash Ledger, and 1906 San Francisco;
- Mara, Eli, and Nia retain separate HP;
- exploration supports keyboard and joystick;
- rescue and two investigation routes are playable;
- failures move the story forward with Fire Clock or Exposure consequences;
- Quill identifies the observations as the Carrier;
- Venn can be defeated physically or persuaded through opposed Protocol Dice;
- battle uses 3 AP and margin damage;
- Carrier and Exposure govern extraction and verification;
- the Little Giant choice and seven-stroke mark appear in verification;
- developer state, fixed seed, clue grant, and confrontation jump work;
- source compiles with current cc65.

## Test layers

- **Host rules tests:** seeded dice rolls, valid bounds, blank faces, deterministic replay.
- **Content validation:** map dimensions, reachable critical interactions, valid IDs, bounded text.
- **Emulator smoke test:** boot, briefing, input, investigation, dialogue, battle, extraction, verification.
- **Regression route:** fixed seed plus developer keys reaches both physical and social Venn outcomes.
- **Hardware test:** cold boot, controller, SD I/O, video boundaries, audio, and long session.
- **Playtest:** story comprehension, navigation, text size, ethical clarity, battle duration, and emotional impact.

## Definition of done

A feature is done only when its data format is documented, failure behavior is defined, frozen Protocol Seven rules remain intact, memory cost is known, emulator smoke testing passes, and player-visible behavior has been playtested or explicitly marked untested.

