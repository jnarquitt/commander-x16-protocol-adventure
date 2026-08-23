#!/usr/bin/env python3
from pathlib import Path
from PIL import Image, ImageOps

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/source/portraits/ash-ledger-character-direction-v0.120.png"
OUT_DIR = ROOT / "assets/generated"
C_OUT = ROOT / "src/generated_portraits.c"
H_OUT = ROOT / "include/generated_portraits.h"
NAMES = ("mara", "eli", "nia", "quill", "venn")

PALETTE = (
    (0, 0, 0), (255, 255, 255), (136, 0, 0), (170, 255, 238),
    (204, 68, 204), (0, 204, 85), (0, 0, 170), (238, 238, 119),
    (221, 136, 85), (102, 68, 0), (255, 119, 119), (51, 51, 51),
    (119, 119, 119), (170, 255, 102), (0, 136, 255), (187, 187, 187),
)

def nearest(pixel):
    r, g, b = pixel[:3]
    return min(range(16), key=lambda i: sum((a-bb) ** 2 for a, bb in zip((r,g,b), PALETTE[i])))

def main():
    if not SOURCE.exists():
        raise SystemExit(f"Missing portrait direction sheet: {SOURCE}")
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    image = Image.open(SOURCE).convert("RGB")
    width, height = image.size
    panel = width / 5
    packed = bytearray()
    previews = []
    for index, name in enumerate(NAMES):
        left = int(index * panel + panel * 0.08)
        right = int((index + 1) * panel - panel * 0.08)
        top = int(height * 0.12)
        bottom = int(height * 0.68)
        portrait = ImageOps.fit(image.crop((left, top, right, bottom)), (32, 32), Image.Resampling.LANCZOS)
        pixels = portrait.get_flattened_data() if hasattr(portrait, "get_flattened_data") else portrait.getdata()
        indexes = [nearest(pixel) for pixel in pixels]
        for y in range(32):
            for x in range(0, 32, 2):
                packed.append((indexes[y * 32 + x] << 4) | indexes[y * 32 + x + 1])
        preview = Image.new("RGB", (32, 32))
        preview.putdata([PALETTE[i] for i in indexes])
        preview.resize((192, 192), Image.Resampling.NEAREST).save(OUT_DIR / f"portrait-{name}-v0.120.png")
        previews.append(preview.resize((192, 192), Image.Resampling.NEAREST))

    sheet = Image.new("RGB", (192 * 5, 192), "black")
    for index, preview in enumerate(previews): sheet.paste(preview, (index * 192, 0))
    sheet.save(OUT_DIR / "portrait-preview-sheet-v0.120.png")

    H_OUT.write_text(
        "#ifndef GENERATED_PORTRAITS_H\n#define GENERATED_PORTRAITS_H\n\n"
        "#include <stdint.h>\n#define PORTRAIT_COUNT 5u\n#define PORTRAIT_BYTES 512u\n"
        "extern const uint8_t portrait_data[PORTRAIT_COUNT * PORTRAIT_BYTES];\n\n#endif\n",
        encoding="utf-8",
    )
    lines = ['#include <stdint.h>', '#include "generated_portraits.h"', '',
             'const uint8_t portrait_data[PORTRAIT_COUNT * PORTRAIT_BYTES] = {']
    for start in range(0, len(packed), 16):
        lines.append("    " + ",".join(f"0x{b:02X}u" for b in packed[start:start+16]) + ",")
    lines.append("};\n")
    C_OUT.write_text("\n".join(lines), encoding="utf-8")
    print(f"Built {len(NAMES)} portraits: {len(packed)} bytes")

if __name__ == "__main__":
    main()
