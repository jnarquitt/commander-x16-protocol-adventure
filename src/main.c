#include <conio.h>
#include <stdint.h>
#include "protocol_dice.h"

#define MAP_W 30
#define MAP_H 18

static uint8_t px = 3u;
static uint8_t py = 3u;
static uint8_t hp = 18u;
static uint16_t steps = 0u;

static const char* map_rows[MAP_H] = {
    "##############################",
    "#............#...............#",
    "#............#.......####....#",
    "#....................#..#....#",
    "#....######..........#..#....#",
    "#....#....#..........####....#",
    "#....#....#..................#",
    "#....######....####..........#",
    "#..............#..#..........#",
    "#..............####....###...#",
    "#......................#.#...#",
    "#....####..............###...#",
    "#....#..#....................#",
    "#....####...........######...#",
    "#...................#........#",
    "#...................######...#",
    "#............................#",
    "##############################"
};

static void draw_world(void) {
    uint8_t y;
    clrscr();
    cputs("PROTOCOL ADVENTURE  v0.001\r\n");
    cputs("WASD move   Q quit\r\n");
    for (y = 0u; y < MAP_H; ++y) {
        gotoxy(0u, (uint8_t)(y + 3u));
        cputs(map_rows[y]);
    }
    gotoxy(px, (uint8_t)(py + 3u));
    cputc('@');
    gotoxy(33u, 4u); cprintf("HP %u/18", hp);
    gotoxy(33u, 5u); cprintf("STEPS %u", steps);
}

static void show_roll(const ProtocolResult* r) {
    uint8_t i;
    cputs("ROLL: ");
    for (i = 0u; i < r->count; ++i) {
        if (r->faces[i] < 4u) cputs("_ ");
        else cprintf("%u ", r->faces[i]);
    }
    cprintf(" TOTAL %u\r\n", r->total);
}

static void encounter(void) {
    ProtocolPool attack = {{8u, 8u, 6u, 6u}, 4u};
    ProtocolPool defense = {{6u, 6u, 4u, 4u}, 4u};
    ProtocolResult result;
    uint8_t enemy_hp = 10u;
    char key;
    while (enemy_hp && hp) {
        clrscr();
        cputs("RANDOM ENCOUNTER!\r\n\r\n");
        cprintf("YOU %u HP     STREET DRONE %u HP\r\n\r\n", hp, enemy_hp);
        cputs("A attack   F flee\r\n");
        key = cgetc();
        if (key == 'f' || key == 'F') {
            protocol_roll(&defense, &result);
            show_roll(&result);
            if (result.total >= 12u) { cputs("YOU ESCAPE. PRESS A KEY."); cgetc(); return; }
            cputs("ESCAPE FAILED. ");
        } else if (key == 'a' || key == 'A') {
            protocol_roll(&attack, &result);
            show_roll(&result);
            if (result.total >= 12u) {
                uint8_t damage = result.total >= 16u ? 6u : 4u;
                enemy_hp = damage >= enemy_hp ? 0u : (uint8_t)(enemy_hp - damage);
                cputs("HIT! ");
            } else cputs("MISS. ");
        } else continue;
        if (enemy_hp) {
            protocol_roll(&defense, &result);
            if (result.total < 12u) {
                hp = hp <= 3u ? 0u : (uint8_t)(hp - 3u);
                cputs("THE DRONE HITS.\r\n");
            } else cputs("YOU EVADE.\r\n");
        }
        cputs("PRESS A KEY."); cgetc();
    }
    clrscr();
    cputs(hp ? "ENCOUNTER WON.\r\n" : "YOU HAVE FALLEN.\r\n");
    cputs("PRESS A KEY."); cgetc();
}

static uint8_t walkable(uint8_t x, uint8_t y) {
    return map_rows[y][x] != '#';
}

int main(void) {
    char key;
    uint8_t nx, ny;
    protocol_seed(0x1985u);
    while (hp) {
        draw_world();
        key = cgetc();
        nx = px; ny = py;
        if (key == 'q' || key == 'Q') break;
        if (key == 'w' || key == 'W') --ny;
        else if (key == 's' || key == 'S') ++ny;
        else if (key == 'a' || key == 'A') --nx;
        else if (key == 'd' || key == 'D') ++nx;
        else continue;
        if (walkable(nx, ny)) {
            px = nx; py = ny; ++steps;
            if ((steps % 7u) == 0u) encounter();
        }
    }
    clrscr(); cputs("THANKS FOR PLAYING.\r\n");
    return 0;
}
