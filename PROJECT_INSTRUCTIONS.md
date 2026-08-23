# Commander X16 Project Instructions

These instructions govern this repository and future Commander X16 projects derived from it.

## Working relationship

- The user is the creative director and playtester, not the build engineer.
- Take reasonable implementation decisions without stopping for routine approval.
- Explain outcomes in plain language and keep instructions short.
- When a direct file or GitHub action cannot be completed automatically, provide one complete, copyable Bash block.
- Prefer Bash commands over prose-only terminal directions.
- Never use placeholders such as `/path/to/file` when the path can be detected.
- Commands must be safe to paste from the user's home directory.
- A Bash block must either complete the operation or stop with a useful error.
- After a command, state the expected visible result.

## Development rules

- Target the current Commander X16 emulator and current `cc65` CX16 target.
- Keep the last known-good PRG runnable while developing the next milestone.
- Use VERA hardware intentionally; document VRAM addresses and memory ownership.
- Preserve Protocol Dice canon: roll the skill pool, results 1–3 are blank, and total results 4+.
- Separate rules logic, platform code, content data, and generated assets.
- Do not hand-edit anything under `build/`.
- Record player-visible changes in `CHANGELOG.md`.
- Update `PROJECT_STATUS.md` whenever a milestone changes state.
- Record durable decisions in `DECISIONS.md`.

## Required verification

Before calling a version complete:

- run `./x16 doctor`;
- run `./x16 build`;
- run `./x16 test`;
- boot the generated PRG in the emulator;
- test keyboard and joystick controls;
- test map-to-combat and combat-to-map transitions;
- confirm HP, inventory, objectives, and encounter state persist;
- confirm GitHub Actions succeeds after publishing;
- confirm the public PRG is the intended build.

## Release communication

Every handoff must include what changed, what was verified, the current version, one public project link when published, and one copyable Bash block if the user must do anything locally.

