#include <conio.h>
#include "game_content.h"

const char* const ash_map[MAP_H] = {
    "################################",
    "#.....FFF......#.....~~~~......#",
    "#..C..FFF..L...#.....~~~~..T...#",
    "#.....FFF......#.....~~~~......#",
    "#..............#...............#",
    "#..####..XXXX..#..#####........#",
    "#..#..#...........#...#....R...#",
    "#..#..#..####.....#...#........#",
    "#..####..#..#.....#####........#",
    "#........#..#..............Q...#",
    "#..XXXX..####..................#",
    "#..............................#",
    "#.....#####........#####.......#",
    "#.....#...#........#...#...V...#",
    "#.....#####........#####.......#",
    "#............................E.#",
    "#..............................#",
    "################################"
};

const VectorDef party_defs[PARTY_SIZE] = {
    {"MARA", "OPERATOR", 24u, COLOR_LIGHTRED, {8u,8u,6u,8u}, {8u,8u,6u,8u}, {4u,4u,6u,4u}},
    {"ELI", "GHOST", 18u, COLOR_CYAN, {8u,8u,6u,6u}, {8u,8u,8u,8u}, {4u,4u,8u,6u}},
    {"NIA", "INTERPRETER", 18u, COLOR_YELLOW, {6u,6u,4u,4u}, {6u,6u,8u,6u}, {8u,8u,8u,8u}}
};

const char* carrier_name(uint8_t carrier) {
    if (carrier >= 3u) return "DURABLE";
    if (carrier == 2u) return "ESTABLISHED";
    if (carrier == 1u) return "FRAGILE";
    return "UNIDENTIFIED";
}

const char* exposure_name(uint8_t exposure) {
    if (exposure >= 5u) return "SEVERE";
    if (exposure >= 4u) return "NOTICEABLE";
    if (exposure >= 3u) return "INTEREST";
    if (exposure >= 1u) return "TRACE";
    return "CLEAN";
}

