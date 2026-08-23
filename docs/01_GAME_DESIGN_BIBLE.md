# Game Design Bible

## Experience

The player should feel like they found an ambitious, story-rich 16-bit console RPG built for Commander X16. Exploration is brisk and tactile. Historical places, hard ethical choices, and a few memorable people matter more than hundreds of interchangeable rooms.

## Core loop

1. Enter an area and read its immediate visual language.
2. Move, inspect, speak, take, use, or attempt a skill.
3. Spend resources or accept risk to reach new information and routes.
4. Trigger an authored campaign event, optional historical echo, hazard, or conflict.
5. Resolve conversation, hazard, discovery, or combat.
6. Return to the changed exploration map.
7. Reach a safe place to recover, save, equip, and choose the next lead.

## Screen model

### Exploration

- Layer 0: terrain tilemap.
- Layer 1: roofs, foreground cover, UI frame, or contextual overlay.
- Hardware sprites: hero, NPCs, enemies, pickups, effects.
- Recommended initial logical field: 40×30 tiles at 16×16 pixels, displayed at 640×480.
- Camera follows the player on maps larger than one screen.

### Encounter transition

Use a short palette fade, wipe, or tile dissolve. The transition masks asset loading and makes encounters feel dramatic.

### Combat

- Large enemy art or assembled sprites on the upper field.
- Player and enemy status in a persistent readable panel.
- Commands: Attack, Skill, Item, Guard, Flee, and context-specific options.
- Every choice previews the relevant pool and target difficulty before confirmation when space allows.
- Roll animation shows each die face, blanks 1–3 visually, totals the remainder, and announces margin and effect.

## World structure

Build each Protocol Seven operation as a chapter with:

- an Aletheia briefing and insertion;
- an explorable historical location;
- multiple investigation routes;
- a named Fulcrum and Historical Carrier;
- Covenant intervention;
- competing Carrier and Exposure pressures;
- extraction and Historical Verification;
- at least one optional choice that returns later as remembered history.

## Encounters

Campaign chapters favor authored encounters. Optional regional events may use weighted decks, but must include people, evidence, hazards, temporal consequences, and discoveries—not merely combat. Repeated battles may never replace the mission procedure.

## Character model

The player has six Ability dice: Strength, Dexterity, Constitution, Intelligence, Wisdom, and Charisma. A character sheet stores die sides, current HP, skill ranks/dice, equipment, conditions, quest flags, and progression.

The starting Ability array is d8, d8, d6, d6, d4, d4. HP equals the character's CON die maximum multiplied by three: d4 12, d6 18, d8 24, d10 30, d12 36.

## Progression

- Skills improve by story awards and deliberate training, not repetitive grinding.
- Equipment changes pools or effects, but never replaces Skills.
- Advancement should unlock new approaches to old places.
- An upgrade is interesting when it changes a decision, not only a number.

## Accessibility

- Provide remappable keyboard and joystick actions.
- Avoid information conveyed only by color or rapid flashing.
- Make combat waits dismissible.
- Offer normal and large-text dialogue layouts if memory permits.
- Keep essential instructions on screen; do not require the printed manual to operate the interface.
