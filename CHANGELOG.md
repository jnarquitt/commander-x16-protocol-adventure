# Changelog

## v0.100 — The Falling Hour / Pre-Alpha

- Rebooted the project as *Protocol Seven: The Ash Ledger*, a story-driven SNES-style RPG.
- Adapted the opening 1906 San Francisco operation from *The Ash Ledger* r003.
- Added Mara Venn, Eli Mercer, and Nia Okafor as a persistent party using current HP-by-CON rules.
- Added rescue, investigation, Quill, Rourke, Venn, extraction, and Historical Verification scenes.
- Added Carrier, Exposure, Fire Clock, clue, nanogel, and remembered-history state.
- Added fail-forward skill scenes with visible Protocol Dice.
- Added a 3 AP opposed conflict with attack, guard, nanogel, and influence actions.
- Added physical and social victory paths and the seven-stroke campaign hook.
- Added developer state, deterministic seed, clue grant, and confrontation-jump controls.

## 0.020 — Content Systems / Pre-Alpha

- Separated map, region, enemy, and item definitions from the main game loop.
- Added the Static Marsh as a second named region.
- Added Signal Wraith and Iron Sentinel encounters alongside the Scrap Stalker.
- Added region-specific random encounter selection and enemy statistics.
- Added a reusable dialogue presentation and a Lost Relay interaction.
- Added `V` developer state, `R` forced encounter, and `N` deterministic seed reset controls.
- Updated the HUD and title identity to v0.020.

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
