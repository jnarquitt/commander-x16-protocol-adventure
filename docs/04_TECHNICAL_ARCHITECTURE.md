# Technical Architecture

## State machine

The game is a state machine. Only one top-level state owns input and rendering at a time:

- Boot
- Title
- New Game / Continue
- Exploration
- Dialogue
- Encounter Transition
- Combat
- Inventory / Character
- Save / Load
- Game Over

Transitions pass small identifiers, never large copied structures.

## Module boundaries

| Module | Responsibility |
|---|---|
| `game` | Top-level state and transitions |
| `input` | Keyboard/joystick to abstract actions |
| `protocol_dice` | RNG, dice pools, targets, margins |
| `world` | Map chunks, collision, triggers, camera |
| `encounter` | Weighted selection, suppression, persistent state |
| `combat` | Initiative, commands, effects, victory/defeat |
| `dialogue` | Text tokens, choices, conditions, consequences |
| `content` | Stable IDs and banked data access |
| `vera` | All direct VERA register access |
| `audio` | Music state, PSG/YM effects, PCM policy |
| `save` | Versioned serialization and validation |
| `debug` | Assertions, overlay, deterministic replay hooks |

The dice engine must not know about screens, sprites, or story. Combat asks it to roll a pool and receives a result record.

## Memory plan for the vertical slice

This is a planning allocation, not a hardware register map:

| Storage | Intended contents |
|---|---|
| Low RAM | executable, C runtime, hot state, small buffers |
| Banked RAM banks 1–3 | world chunks and collision |
| Banked RAM banks 4–5 | dialogue and string tokens |
| Banked RAM bank 6 | encounter and enemy data |
| Banked RAM bank 7 | save/load staging and decompression |
| VRAM low region | Layer 0 tiles and tilemap |
| VRAM middle region | Layer 1/UI tiles and tilemap |
| VRAM high region | sprite frames and palettes |

Bank 0 is treated as system-sensitive until current documentation and runtime behavior prove an allocation safe.

## Content pipeline

Source assets remain editable PNG, Tiled JSON/TMX, MIDI/tracker source, and UTF-8 text. Host tools convert them into bounded binary resources. Every converter must output size, destination bank/VRAM address, checksum, and source filename.

Runtime does not parse PNG, JSON, TMX, or UTF-8.

## Encounter algorithm

1. A successful player step increases the regional threat accumulator.
2. Safe tiles and story states modify the accumulator.
3. When the threshold test fires, filter encounter entries by flags and cooldown.
4. Select by weight using an unbiased bounded random value.
5. Record the chosen stable encounter ID and update its cooldown/history.
6. Transition through a loading mask into the encounter state.

## Save compatibility

The save schema is its own version. Code version and save version are not interchangeable. New releases must load older supported saves through explicit migration code or reject them with a readable message.

## Failure policy

In development builds, invalid IDs, bad bank bounds, oversized assets, and impossible dice must fail loudly. In release builds, recover to the title screen where possible and display a short error code that maps to the manual.

