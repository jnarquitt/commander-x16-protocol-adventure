# v0.110 Presentation, Audio, and Save Architecture

## Visual foundation

- The field uses a custom 8×8 glyph set for pavement, brick, rubble, fire, and water.
- Sprite slot 0 is the animated player.
- Sprite slots 1–4 are field actors: trapped child, Miriam Quill, Elias Rourke, and Silas Venn.
- Sprite slot 5 becomes the active 32×32 speaker portrait during dialogue.
- Sprite slots 6 and 7 display Eli and Nia following Mara on the field.
- Field actors are generated deterministically; five principal portraits are converted from the approved direction sheet and embedded so the PRG remains self-contained.

## Dialogue presentation

- Dialogue remains over the visible field when possible.
- The active speaker receives a colored portrait and nameplate.
- Keyboard Enter or joystick button A advances text.
- Text remains short enough for the 40-column presentation and is not dependent on color alone.

## Controller menus

- Title: Up/Down selects New Operation or Continue; button A confirms.
- Battle: Up/Down selects Attack, Reason, Guard, Nanogel, or End Turn; button A confirms.
- Number keys 1–3 remain available for rapid Vector selection.
- Direct keyboard command letters remain available for testing and accessibility.

## Audio

The sound module reserves VERA PSG voice 0 for confirm, success, failure, alarm, and movement cues. Voices 1 and 2 provide a nonblocking looping field theme.

A later music driver must expand this ownership model, support global attenuation, and never write another subsystem's voice without coordination.

## Checkpoint saves

- Filename: `P7ASH.SAV`
- Device: 8
- Schema version: 1
- Identity: `P7AS`
- Integrity: rotating XOR checksum
- State: position, party HP, nanogel, clues, rescue, Quill, Carrier, Exposure, Fire Clock, Little Giant, and Venn
- Checkpoints: Carrier identification and completed Venn confrontation

The title menu can continue a valid checkpoint. Missing, truncated, wrong-version, wrong-magic, and checksum-invalid saves fail closed and begin a new operation only after informing the player.

## VRAM ownership

Run `./x16 report` for the active allocation summary. No new visual or audio system may claim an address without updating that report and this document.
