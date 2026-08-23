# Development Controls

This file defines the controls needed to develop this game and reuse the project structure for other Commander X16 software.

## Command controls

The repository-root `x16` script is the primary command center.

| Command | Purpose |
|---|---|
| `./x16 help` | Show all available commands |
| `./x16 doctor` | Check Git, Make, compiler, emulator, and repository state |
| `./x16 setup` | Build a current local cc65 toolchain |
| `./x16 build` | Compile `build/PROTOCOL.PRG` |
| `./x16 run` | Build and launch in the detected emulator |
| `./x16 test` | Run automated checks and compile verification |
| `./x16 clean` | Remove generated compiler output |
| `./x16 status` | Show project, Git, build, and workflow status |
| `./x16 publish "message"` | Commit and push current changes |
| `./x16 download` | Download the newest public PRG |
| `./x16 play-latest` | Download and run the newest public PRG |

## Creative controls

- `docs/00_PROJECT_CHARTER.md`: scope and authority
- `docs/01_GAME_DESIGN_BIBLE.md`: experience, tone, and game loop
- `docs/02_PROTOCOL_DICE_VIDEO_GAME_RULES.md`: mechanical canon
- `PROJECT_STATUS.md`: current state and immediate milestone
- `DECISIONS.md`: durable technical and design choices
- `BACKLOG.md`: prioritized work without changing canon
- `CHANGELOG.md`: shipped player-visible changes

## Predicted controls needed next

### Game-state inspector

A developer overlay should display map coordinates, region, threat, encounter seed, HP, inventory flags, objective flags, and free banked RAM. It should be removable from release builds.

### Deterministic encounter control

Allow a fixed random seed and a force-encounter key so combat bugs can be reproduced without walking around until an encounter appears.

### Scene jump control

Provide developer shortcuts for title, selected map, combat, victory, defeat, and dialogue scenes.

### Content validation

Host-side tools should reject invalid maps, unreachable objectives, unknown tile IDs, oversized dialogue, conflicting VRAM addresses, and encounter tables with invalid weights.

### Save inspection

Save data needs a version number, checksum, migration policy, and a host-side decoder that prints human-readable state.

### Asset pipeline

Use source PNG/TMX/text assets and repeatable conversion commands. Generated binary assets should include size and target-address reports.

### Memory budget

Generate a build report covering PRG size, conventional RAM regions, banked RAM allocations, VRAM allocations, sprite slots, and largest content blocks.

### Hardware compatibility matrix

Record tested emulator version, ROM version, video mode, keyboard, joystick, and real-hardware results for every release candidate.

### Release identity

Show version and short Git commit on the title or diagnostics screen so a screenshot immediately identifies the running build.

### Automated publishing guard

GitHub Actions must build from source, upload the PRG, deploy the project page, and fail rather than publish when compilation or validation fails.

