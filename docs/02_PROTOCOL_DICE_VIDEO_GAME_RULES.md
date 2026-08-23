# Protocol Dice Video-Game Rules

## Canonical roll

A standard Skill pool contains three associated Ability dice and one Skill die. Equipment, help, conditions, or a special feature may add dice up to the implementation cap of eight.

For every die:

- generate an unbiased face from 1 through its number of sides;
- treat 1, 2, and 3 as blank;
- add results of 4 or higher at face value;
- compare the total with a Difficulty or an opponent's total.

Supported die sizes are d4, d6, d8, d10, and d12. Untrained Skills use d4.

## Difficulties

| Difficulty | Target |
|---|---:|
| Easy under pressure | 8 |
| Moderate | 12 |
| Hard | 16 |
| Very hard | 20 |
| Extreme | 24+ |

Do not roll when failure is uninteresting and no pressure exists.

## Margins

| Amount above target | Result |
|---|---|
| 0–3 | Narrow |
| 4–7 | Solid |
| 8–11 | Powerful |
| 12+ | Overwhelming |

On an opposed tie, the defender or existing situation wins.

## Digital presentation

The computer must preserve the feeling of rolling a pool:

- display die types before commitment;
- show individual results, not only the total;
- replace 1–3 with a blank-face graphic after the roll;
- total visible results;
- show the target, success/failure, margin, and mechanical consequence;
- allow the player to accelerate or skip repeated animation.

## Combat adaptation

The game preserves 3 AP turns. A normal Attack costs 1 AP and may be repeated by paying its cost. Movement costs 1 AP for up to the scene's equivalent of 30 feet. One normal Reaction is available between turns under the current banked/borrowed AP rule.

- **Attack:** roll the weapon Skill pool against the target Defense.
- **Skill:** roll a context-specific Skill, often to create an advantage rather than deal damage.
- **Item:** use a carried item; the item defines whether a roll occurs.
- **Guard:** gain a temporary defense die until the next turn.
- **End Turn:** surrender remaining actions and proceed to the opposition.

Damage equals the winning margin of the opposed attack and defense pools. Armor contributes its listed gear die to an appropriate defense pool and does not also reduce damage unless a specific item says otherwise.

One primary relevant gear die may contribute to a roll unless an explicit rule permits stacking. VAMs consume BAR capacity rather than energy; loading into free capacity is free, while field replacement or unloading costs 1 AP for the reconfiguration.

## Randomness contract

- Use rejection sampling so every face is equally likely.
- Keep game RNG state in save data.
- Permit a fixed developer seed for reproducible bugs.
- Gameplay must not depend on animation timing.
- Random encounter selection and dice rolling may share a generator initially, but the production build should use separate streams so presentation changes do not alter combat results.

## Data representation

- Store a die as its side count: 4, 6, 8, 10, or 12.
- Store pools as a count followed by side-count bytes.
- Store totals in 8 bits; the eight-d12 maximum is 96.
- Store Skill identifiers and definitions separately from character values.
- Use stable numeric IDs in save data; never save C pointers or array addresses.

## First playtest questions

- Does seeing every die make outcomes understandable?
- Are DC 8/12/16/20 readable through play rather than only in documentation?
- Does a failed roll create a new situation quickly?
- Do encounter frequency and healing produce tension without grind?
- Does combat end before its visual presentation becomes repetitive?
