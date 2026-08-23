# Changelog

## v0.010 — 2026-08-23

- Replaced the text proof with a 40×30 VERA tilemap exploration screen.
- Added a two-frame, 16×16, 4-bpp hardware sprite generated directly into VRAM.
- Added keyboard and standard Commander X16 joystick controls.
- Added colored terrain, collision, an investigation cache, a locked gate, and an artifact objective.
- Added region threat accumulation and a dedicated Scrap Stalker combat screen.
- Added Attack, Guard, Medkit, and Flee commands with visible Protocol Dice pools.
- Preserved HP, inventory, objective state, and encounter state across screen transitions.
- Added current-cc65 automated builds for compatibility with modern X16 ROM releases.

## v0.001 — 2026-08-22

- Created the Commander X16 project charter and design bible.
- Adapted Protocol Dice into deterministic video-game rules.
- Chose C with cc65 plus optional ca65 modules as the baseline toolchain.
- Defined VERA, memory banking, input, audio, asset, save, and test architecture.
- Added a text-mode exploration/combat prototype.
- Established the v0.010 VERA vertical-slice acceptance criteria.
