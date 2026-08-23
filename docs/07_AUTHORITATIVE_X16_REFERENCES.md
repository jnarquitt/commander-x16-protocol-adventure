# Authoritative Commander X16 References

Use primary sources for implementation decisions and re-check them when upgrading the emulator, ROM, VERA, or compiler.

- Commander X16 Reference Manual: https://github.com/X16Community/x16-docs
- Official emulator: https://github.com/X16Community/x16-emulator
- X16 ROM/KERNAL/DOS: https://github.com/X16Community/x16-rom
- VERA Programmer's Reference: https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2009%20-%20VERA%20Programmer%27s%20Reference.md
- cc65 Commander X16 runtime notes: https://cc65.github.io/doc/cx16.html
- cc65 documentation: https://cc65.github.io/doc/
- Commander X16 community organization: https://github.com/X16Community
- Commander X16 forum: https://cx16forum.com/forum/

## Version discipline

Record these together in every release:

- emulator version and commit/release;
- `rom.bin` release;
- VERA firmware/FPGA release for hardware;
- cc65 version;
- project version;
- save-schema version;
- asset-pipeline version.

The emulator project explicitly warns that incompatible ROM versions may not work together. Treat a matched emulator/ROM pair as one test platform.

## Subjects to verify before implementation

- current memory map and reserved zero page;
- KERNAL entry points and calling convention;
- joystick and keyboard APIs;
- VERA register semantics and address increments;
- sprite attributes, collision behavior, and limits;
- tilemap entry format and layer configuration;
- VSYNC interrupt behavior;
- RAM bank availability on minimum target hardware;
- CMDR-DOS file behavior;
- cc65 runtime stack, heap, banking helpers, and drivers.

