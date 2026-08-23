# Commander X16 Programming Guide

## Baseline platform

The Commander X16 is an 8-bit system centered on a 65C02 CPU and VERA video/audio hardware. VERA provides 128 KiB VRAM, two tile/bitmap layers, a 256-entry palette selected from 4096 colors, and up to 128 sprites. The machine uses banked system RAM, so code must explicitly distinguish always-visible low RAM, the banked RAM window, ROM, I/O, and VRAM.

## Toolchain decision

Use **cc65**:

- `cc65` compiles C;
- `ca65` assembles 65C02 source;
- `ld65` links segments;
- `cl65` drives the usual compile/link flow;
- the `cx16` target supplies runtime support and platform headers.

C owns game state, content flow, menus, and most rules. ca65 owns measured hot paths, interrupt handlers, bulk VRAM copies, decompression, and other routines where assembly provides a demonstrated benefit.

## Install and verify

Install a current cc65 build and a matching official Commander X16 emulator/ROM pair. Verify:

```sh
cl65 --version
/path/to/x16emu -version
make
make run EMULATOR=/path/to/x16emu
```

The official emulator requires a compatible `rom.bin`; mismatching emulator and ROM generations can fail. Keep exact versions in release notes.

## Build anatomy

The starter command is:

```sh
cl65 -t cx16 -Oirs -I include -o build/PROTOCOL.PRG src/main.c src/protocol_dice.c
```

As the game grows, introduce a custom linker configuration and explicit segments only after a memory map has been measured. Do not copy an old online memory map without checking the current X16 reference and cc65 CX16 runtime documentation.

## Memory rules

- Low RAM contains the executable, C runtime, stack, heap, and hot state.
- Banked RAM is the home of map chunks, dialogue, encounter definitions, decompressed assets, and save buffers.
- VRAM holds tile graphics, tilemaps, sprite graphics, palettes, and display data.
- ROM/KERNAL calls are preferred for stable file, keyboard, joystick, mouse, and system services where practical.
- Bank changes are global state. Wrap them in functions, keep critical sections short, and restore the prior bank.
- Never leave a C pointer aimed into a banked window after switching banks.

cc65's default CX16 runtime documents usable ranges and a downward-growing C stack. Re-check those details whenever the toolchain changes.

## VERA essentials

VERA is accessed through registers beginning at `$9F20`. Its address ports support automatic increment, which should be used for sequential transfers. Build a small graphics abstraction instead of scattering register writes throughout gameplay code.

Required graphics modules:

- `vera_init` — mode, scale, layers, sprites, border;
- `vram_copy` — bounded sequential transfers;
- `palette_load` — project palette banks;
- `tilemap_draw` — map metadata to VERA tile entries;
- `sprite_set` — frame, position, depth, palette, collision mask;
- `fade` — encounter transition without altering game state.

For the vertical slice, use 16×16 tiles and one 16×16 or 32×32 hero sprite. Allocate VRAM in one documented table before importing assets.

## Main loop

Use a fixed update rhythm synchronized to VSYNC:

```text
poll input -> update game state -> update camera/animation ->
wait for safe display interval -> copy changed display data -> repeat
```

Do not redraw the full map every frame. Update the camera only on movement and upload only dirty UI or map regions.

## Input

Define abstract actions: Up, Down, Left, Right, Confirm, Cancel, Menu, and Context. Keyboard and joystick adapters populate the same action structure. Edge-trigger menus; separately track held input for movement repeat.

## Files and saves

Use KERNAL/DOS file APIs rather than assuming host filesystem behavior. Save files need:

- magic bytes and format version;
- checksum;
- character state;
- current map and coordinates;
- quest and encounter flags;
- inventory;
- RNG state;
- optional compatibility/migration bytes.

Write a temporary file, verify it, then replace the main save when the available DOS workflow safely permits it. Never serialize compiler-dependent structs directly.

## Debugging

- Compile debug builds with symbols and fixed RNG seeds.
- Use emulator breakpoints and memory inspection for bank/VRAM corruption.
- Add an on-screen developer panel: build version, map, coordinates, active RAM bank, RNG seed, free-memory estimate, and last encounter ID.
- Treat warnings about illegal VERA register access as real bugs.
- Test without zero-initialized RAM; recent emulator behavior intentionally exposes programs that rely on accidental zeroes.

## Performance practice

Measure before rewriting C in assembly. Typical candidates for optimization are decompression, rectangle/tile copies, collision queries, sprite sorting, and audio streaming. Rules, quests, and menus usually benefit more from clarity than hand assembly.

## Hardware QA

- matching released ROM and firmware family;
- keyboard and at least one supported gamepad;
- SD-card loading and saving;
- cold boot and repeated load;
- CRT-safe border and overscan assumptions;
- no emulator host-filesystem dependency;
- stable behavior with minimum supported banked RAM;
- audio levels and timing on hardware.

