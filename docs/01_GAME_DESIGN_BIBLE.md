# Game Design Bible

## Experience

The player should feel like they found a lost, unusually sophisticated 1980s adventure game. Exploration is brisk and tactile. The world is mysterious rather than enormous. A few memorable locations, inhabitants, and enemies matter more than hundreds of interchangeable rooms.

## Core loop

1. Enter an area and read its immediate visual language.
2. Move, inspect, speak, take, use, or attempt a skill.
3. Spend resources or accept risk to reach new information and routes.
4. Trigger a placed event or a region-specific random encounter.
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

Use a hub-and-spoke region for the vertical slice:

- one safe hub;
- two outdoor routes;
- one small interior or dungeon;
- one gated route opened by a clue, item, or skill;
- three encounter families;
- one named antagonist or creature;
- one decision that changes a later encounter.

## Encounters

Each region owns a weighted deck rather than a flat monster table. Example composition:

- 35% hostile combat;
- 20% threatening situation that may become combat;
- 15% environmental hazard;
- 15% discovery or resource;
- 10% traveler or conversation;
- 5% rare authored event.

After an event appears, its weight can drop, disappear, or change. A pity counter prevents long stretches without events and suppresses immediate repeats.

## Character model

The player has six Ability dice: Strength, Dexterity, Constitution, Intelligence, Wisdom, and Charisma. A character sheet stores die sides, current HP, skill ranks/dice, equipment, conditions, quest flags, and progression.

The default starting Ability array is d8, d8, d6, d6, d4, d4. Every character begins with 18 HP unless this project later establishes its own documented override.

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

