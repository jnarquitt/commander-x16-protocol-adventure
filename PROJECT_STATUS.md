# Protocol Seven: The Ash Ledger — Project Status

## Current build

- Version: **0.120 — Art Direction and World Detail / Pre-Alpha**
- State: playable narrative RPG slice with converted portraits, animated environments, party followers, music, and manual checkpoints
- Program: `build/PROTOCOL.PRG`
- Public page: https://jnarquitt.github.io/commander-x16-protocol-adventure/

## Working now

- Protocol Seven and *The Ash Ledger* campaign identity
- Mara, Eli, and Nia party with canonical HP-by-CON values
- 1906 San Francisco exploration map and hardware player sprite
- rescue, Insurance Annex, Telegraph Exchange, Quill, Rourke, Venn, and extraction interactions
- fail-forward Protocol Dice skill scenes
- Carrier, Exposure, Fire Clock, clue, and optional-history tracking
- 3 AP battle turns with attack, guard, nanogel, and opposed influence
- physical and social victory paths against Silas Venn
- Historical Verification and seven-stroke campaign hook
- deterministic seed, investigation-key, confrontation-jump, and state-overlay controls
- custom VERA terrain glyphs and independently drawn actor sprites
- speaker portraits and scene fades
- native VERA PSG feedback cues
- joystick-native title and combat menus
- versioned, checksum-protected device-8 checkpoint saves
- automated PRG, VRAM, and save-schema build report
- preserved five-character portrait direction sheet and deterministic conversion pipeline
- 32×32 runtime portraits for Mara, Eli, Nia, Quill, and Venn
- Eli and Nia field sprites following the player
- animated fire and water glyphs
- skippable typewriter dialogue presentation
- nonblocking two-voice VERA PSG field theme
- manual checkpoint control on keyboard C or joystick button B

## Next milestone

**v0.130 — Cinematic Scene and Map Expansion**

- full-resolution 16×16 map tiles and dedicated VERA tile layer
- portrait expressions and conversation camera cues
- interior maps for Insurance Annex and Telegraph Exchange
- multi-scene map transitions and authored event triggers
- smoke, falling masonry, aftershock, and crowd effects
- expanded score with battle and verification themes
- three-slot checkpoint interface

## Current risks

- emulator and ROM versions can change startup behavior;
- VRAM allocations must remain centralized;
- GitHub Pages can briefly serve a cached earlier PRG;
- game-state growth will eventually require banked RAM planning.
