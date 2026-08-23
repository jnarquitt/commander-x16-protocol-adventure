# Content and Data Standard

## Stable IDs

Maps, areas, encounters, actors, items, skills, dialogue nodes, quests, sound effects, music cues, and flags receive numeric IDs. IDs are never silently reused after publication.

Human-readable symbolic names live in source and generated symbol files. Save data contains numeric IDs.

## Map authoring

Each map chunk defines:

- Layer 0 tile entries;
- optional Layer 1 entries;
- collision bits;
- spawn points;
- exits;
- interaction triggers;
- placed encounters;
- regional encounter table;
- music and palette IDs;
- bank and compressed size.

Collision and interaction data are logical data, not inferred from tile graphics at runtime.

## Dialogue

Dialogue nodes contain a speaker, text token stream, optional portrait, conditions, choices, and consequences. Text wraps during conversion, not during combat-critical frames. The validator rejects lines that exceed the chosen dialogue window.

## Skills

Each Skill definition includes:

- stable ID and display name;
- three associated Abilities;
- default untrained die;
- contexts in which it is offered;
- difficulty guidance;
- success and failure consequence hooks.

Never encode a Skill check as an unexplained percentage. Even passive checks use a Protocol Dice pool.

## Encounters

An encounter record contains stable ID, region, weight, eligibility flags, cooldown, presentation type, actors, opening text, escape rule, reward/consequence, and post-encounter flags.

## Asset budgets

Every asset has a declared maximum before final art begins. The build must fail when a bank, VRAM region, sprite-frame set, dialogue block, or save record exceeds its budget.

## Naming

- C symbols: `snake_case`.
- Constants and stable symbolic IDs: `UPPER_SNAKE_CASE`.
- Source assets: lowercase descriptive names.
- Generated files: uppercase 8.3-compatible release names when they will be used directly on the X16 filesystem.
- Versioned packages: `Commander_X16_Protocol_Adventure_v0.010_PRE_ALPHA.zip`.

