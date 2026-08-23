# v0.120 Art Pipeline and Direction

## Direction

The game uses grave historical science fiction rather than glossy fantasy. Character portraits combine ash-dark neutrals, copper firelight, blue-pencil accents, restrained Protocol red, and signal-cyan rim light.

The five-character direction sheet establishes Mara, Eli, Nia, Miriam Quill, and Silas Venn. Its designs are original to this project and avoid modern tactical armor, steampunk shorthand, chibi proportions, and imitations of an existing commercial game.

## Source and generated assets

- Source: `assets/source/portraits/ash-ledger-character-direction-v0.120.png`
- Converter: `tools/build_portraits.py`
- Runtime source: `src/generated_portraits.c`
- Runtime declaration: `include/generated_portraits.h`
- Human QA sheet: `assets/generated/portrait-preview-sheet-v0.120.png`
- Individual enlarged previews: `assets/generated/portrait-*-v0.120.png`

Run:

```bash
./x16 assets
./x16 test
```

## Conversion contract

- Split the source sheet into five stable panels.
- Crop each head-and-shoulders composition consistently.
- Resize to 32×32.
- Quantize to the project’s 16-color Commander X16-safe palette.
- Pack two 4-bpp pixels per byte.
- Produce exactly 512 bytes per portrait and 2,560 bytes total.
- Generate enlarged nearest-neighbor previews for visual inspection.

Generated C and preview assets are checked into source control so normal game builds do not depend on Pillow or the original art service.

## Runtime ownership

Converted portraits are uploaded to VRAM `$14000-$149FF` during startup. Dialogue uses sprite slot 5 at 32×32. Field actors remain in `$13100-$134FF`; player animation remains in `$13000-$130FF`.

## Next visual gate

Before final art production, verify each portrait at native emulator scale and on composite output. Nia’s facial readability, Quill’s blue pencil, Venn’s distinct silhouette, and skin-tone separation are specific review points.

