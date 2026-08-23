# Project Status

## Current build

- Version: **0.010 — VERA Vertical Slice / Pre-Alpha**
- State: playable vertical slice
- Program: `build/PROTOCOL.PRG`
- Public page: https://jnarquitt.github.io/commander-x16-protocol-adventure/

## Working now

- 40×30 VERA-backed tile display
- animated hardware player sprite
- keyboard and joystick movement
- collision and map interactions
- Protocol Dice skill checks
- threat-driven random encounters
- combat screen with Attack, Guard, Medkit, and Flee
- state persistence across map/combat transitions

## Next milestone

**v0.020 — Content Systems**

- external map and encounter data
- reusable dialogue system
- multiple enemies and regions
- inventory and item data
- save/load design and first implementation
- sound effects and music foundation
- host-side asset conversion and validation

## Current risks

- emulator and ROM versions can change startup behavior;
- VRAM allocations must remain centralized;
- GitHub Pages can briefly serve a cached earlier PRG;
- game-state growth will eventually require banked RAM planning.

