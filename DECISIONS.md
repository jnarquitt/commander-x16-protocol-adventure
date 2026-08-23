# Decision Log

Durable decisions belong here. Add new entries; do not silently rewrite old decisions.

## 2026-08-22 — Toolchain

Use the current upstream cc65 toolchain instead of an older distribution package. The older toolchain produced a PRG that reached a blank blue screen with the current emulator/ROM combination.

## 2026-08-22 — Display and player

Use the VERA tile display for the world and a real VERA hardware sprite for the player. Maintain an explicit VRAM ownership map as graphics systems grow.

## 2026-08-22 — Input

Support both WASD and a standard Commander X16 joystick. Future input changes must preserve keyboard-only testing.

## 2026-08-22 — Protocol Dice

All skills and combat resolution use Protocol Dice. Results 1–3 are blank; results 4 and higher contribute their face value to the total.

## 2026-08-22 — User operations

Use Bash as the default handoff interface. When automatic GitHub operations are unavailable, provide a complete copyable Bash command that detects paths where practical and stops safely on errors.

