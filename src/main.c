#include <cbm.h>
#include <conio.h>
#include <cx16.h>
#include <joystick.h>
#include <stdint.h>
#include "protocol_dice.h"

#define MAP_W 32u
#define MAP_H 20u
#define MAP_OX 1u
#define MAP_OY 2u
#define SPRITE_DATA 0x13000UL
#define SPRITE_ATTR 0x1FC00UL
#define ACT_NONE 0u
#define ACT_UP 1u
#define ACT_DOWN 2u
#define ACT_LEFT 3u
#define ACT_RIGHT 4u
#define ACT_USE 5u
#define ACT_QUIT 6u

static uint8_t px = 3u, py = 3u, hp = 18u, medkits = 1u;
static uint8_t relic_found = 0u, gate_open = 0u, walk_frame = 0u, threat = 0u;
static uint16_t steps = 0u;

static const char* map_rows[MAP_H] = {
    "################################",
    "#...........TTT................#",
    "#....####...TTT.....~~~~.......#",
    "#....#..#...........~~~~.......#",
    "#....#..#.....?.....~~~~.......#",
    "#....####...........~~~~.......#",
    "#..............................#",
    "#..TTT.....######........TTT...#",
    "#..TTT.....#....#........TTT...#",
    "#..........#....D..............#",
    "#..........######....~~~~~~~...#",
    "#...................~~~~~~~....#",
    "#....####...........~~~~~~~....#",
    "#....#..#......................#",
    "#....####......TTTT............#",
    "#..............TTTT....#####...#",
    "#......................#...#...#",
    "#...........+..........#####...#",
    "#..............................#",
    "################################"
};

static void delay_frames(uint8_t frames) { while (frames--) waitvsync(); }

static uint8_t player_pixel(uint8_t x, uint8_t y, uint8_t frame) {
    if (y < 2u || y > 14u || x < 3u || x > 12u) return 0u;
    if (y < 6u) {
        if (x < 5u || x > 10u) return 0u;
        if (y == 3u && (x == 6u || x == 9u)) return 0u;
        return y < 4u ? 8u : 7u;
    }
    if (y < 11u) {
        if (x < 4u || x > 11u) return 0u;
        if (x == 4u || x == 11u) return 1u;
        return 2u;
    }
    if (y < 13u) return (x > 4u && x < 11u) ? 12u : 0u;
    if (frame == 0u) {
        if ((x >= 4u && x <= 6u) || (x >= 9u && x <= 11u)) return 15u;
    } else if ((x >= 3u && x <= 5u) || (x >= 10u && x <= 12u)) return 15u;
    return 0u;
}

static void upload_player_sprite(void) {
    uint8_t frame, y, x, left, right;
    unsigned long address = SPRITE_DATA;
    for (frame = 0u; frame < 2u; ++frame) {
        for (y = 0u; y < 16u; ++y) {
            for (x = 0u; x < 16u; x += 2u) {
                left = player_pixel(x, y, frame);
                right = player_pixel((uint8_t)(x + 1u), y, frame);
                vpoke((uint8_t)((left << 4) | right), address++);
            }
        }
    }
}

static void sprite_image(uint8_t frame) {
    unsigned long image = SPRITE_DATA + ((unsigned long)frame * 128UL);
    vpoke((uint8_t)((image >> 5) & 0xFFu), SPRITE_ATTR);
    vpoke((uint8_t)((image >> 13) & 0x0Fu), SPRITE_ATTR + 1UL);
}

static void sprite_position(void) {
    uint16_t sx = (uint16_t)(MAP_OX + px) * 8u;
    uint16_t sy = (uint16_t)(MAP_OY + py) * 8u;
    vpoke((uint8_t)(sx & 0xFFu), SPRITE_ATTR + 2UL);
    vpoke((uint8_t)((sx >> 8) & 0x03u), SPRITE_ATTR + 3UL);
    vpoke((uint8_t)(sy & 0xFFu), SPRITE_ATTR + 4UL);
    vpoke((uint8_t)((sy >> 8) & 0x03u), SPRITE_ATTR + 5UL);
}

static void sprite_show(uint8_t visible) {
    vpoke(visible ? 0x0Cu : 0u, SPRITE_ATTR + 6UL);
    vpoke(0x50u, SPRITE_ATTR + 7UL);
    vera_sprites_enable(visible);
}

static void draw_cell(uint8_t x, uint8_t y, char tile) {
    gotoxy((uint8_t)(MAP_OX + x), (uint8_t)(MAP_OY + y)); revers(0);
    if (tile == '#') { textcolor(COLOR_GRAY2); bgcolor(COLOR_BROWN); cputc('#'); }
    else if (tile == '~') { textcolor(COLOR_CYAN); bgcolor(COLOR_BLUE); cputc('~'); }
    else if (tile == 'T') { textcolor(COLOR_LIGHTGREEN); bgcolor(COLOR_GREEN); cputc('*'); }
    else if (tile == '?') { textcolor(COLOR_YELLOW); bgcolor(COLOR_BROWN); cputc('?'); }
    else if (tile == 'D') { textcolor(COLOR_YELLOW); bgcolor(COLOR_RED); cputc(gate_open ? '/' : '+'); }
    else if (tile == '+') { textcolor(COLOR_YELLOW); bgcolor(COLOR_PURPLE); cputc(relic_found ? '.' : '+'); }
    else { textcolor(COLOR_GREEN); bgcolor(COLOR_BLACK); cputc('.'); }
}

static void draw_hud(void) {
    uint8_t y;
    textcolor(COLOR_WHITE); bgcolor(COLOR_BLACK);
    for (y = 23u; y < 30u; ++y) { gotoxy(0u, y); cclear(40u); }
    gotoxy(0u, 23u); textcolor(COLOR_YELLOW); cputs(" PROTOCOL ADVENTURE // V0.010 ");
    gotoxy(0u, 24u); textcolor(COLOR_WHITE); cprintf(" HP:%u/18  MED:%u  STEPS:%u", hp, medkits, steps);
    gotoxy(0u, 25u); textcolor(COLOR_LIGHTGREEN); cputs(" WASD/JOYSTICK MOVE   E/A USE   Q QUIT");
    gotoxy(0u, 27u); textcolor(COLOR_GRAY3);
    if (!relic_found) cputs(" SIGNAL: ARTIFACT BEYOND THE AMBER GATE.");
    else cputs(" SIGNAL: ARTIFACT SECURED. RETURN ALIVE.");
}

static void draw_world(void) {
    uint8_t x, y;
    videomode(VIDEOMODE_40x30); bordercolor(COLOR_BLACK); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_YELLOW); gotoxy(1u, 0u); cputs("SECTOR 7: THE AMBER WILDS");
    for (y = 0u; y < MAP_H; ++y) for (x = 0u; x < MAP_W; ++x) draw_cell(x, y, map_rows[y][x]);
    draw_hud(); sprite_position(); sprite_image(walk_frame); sprite_show(1u);
}

static void show_pool(const ProtocolPool* pool, const ProtocolResult* result, uint8_t target) {
    uint8_t i;
    textcolor(COLOR_YELLOW); cputs("POOL ");
    for (i = 0u; i < result->count; ++i) {
        textcolor(result->faces[i] < 4u ? COLOR_GRAY2 : COLOR_WHITE);
        cprintf("D%u:[%u] ", pool->sides[i], result->faces[i]);
    }
    textcolor(COLOR_YELLOW); cprintf("\r\nTOTAL %u  TARGET %u  ", result->total, target);
    if (result->total >= target) { textcolor(COLOR_LIGHTGREEN); cputs("SUCCESS"); }
    else { textcolor(COLOR_LIGHTRED); cputs("FAILURE"); }
}

static void draw_enemy(uint8_t enemy_hp, uint8_t guard) {
    uint8_t y;
    sprite_show(0u); bordercolor(COLOR_RED); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_LIGHTRED); gotoxy(2u, 1u); cputs("RANDOM ENCOUNTER // SCRAP STALKER");
    textcolor(COLOR_ORANGE);
    gotoxy(8u, 4u); cputs("       /\\____/\\");
    gotoxy(8u, 5u); cputs("  ____/  O  O  \\____");
    gotoxy(8u, 6u); cputs(" /  _     /\\     _  \\");
    gotoxy(8u, 7u); cputs("|__/ \\___/  \\___/ \\__|");
    gotoxy(8u, 8u); cputs("    /___/\\__/\\___\\");
    gotoxy(8u, 9u); cputs("      /_/  \\_\\");
    textcolor(COLOR_WHITE); gotoxy(2u, 12u); cprintf("VECTOR HP %u/18", hp);
    gotoxy(24u, 12u); cprintf("STALKER HP %u/14", enemy_hp);
    gotoxy(2u, 14u); textcolor(COLOR_YELLOW); cputs("[A] ATTACK [G] GUARD [H] MED [F] FLEE");
    gotoxy(2u, 16u); textcolor(COLOR_GRAY3); if (guard) cputs("GUARD ACTIVE: DEFENSE GAINS A D6.");
    for (y = 19u; y < 29u; ++y) { gotoxy(1u, y); bgcolor(COLOR_BLACK); cclear(38u); }
}

static uint8_t combat(void) {
    ProtocolPool attack = {{8u, 8u, 6u, 6u}, 4u};
    ProtocolPool defense = {{6u, 6u, 4u, 4u, 6u}, 4u};
    ProtocolResult result;
    uint8_t enemy_hp = 14u, guard = 0u, damage;
    char key;
    while (enemy_hp && hp) {
        defense.count = guard ? 5u : 4u; draw_enemy(enemy_hp, guard); key = cgetc(); gotoxy(2u, 19u);
        if (key == 'h' || key == 'H') {
            if (medkits) { --medkits; hp = hp > 10u ? 18u : (uint8_t)(hp + 8u); textcolor(COLOR_LIGHTGREEN); cputs("MEDKIT APPLIED. "); }
            else { textcolor(COLOR_LIGHTRED); cputs("NO MEDKITS REMAIN. "); continue; }
        } else if (key == 'g' || key == 'G') { guard = 1u; textcolor(COLOR_LIGHTGREEN); cputs("YOU TAKE COVER. "); }
        else if (key == 'f' || key == 'F') {
            protocol_roll(&defense, &result); show_pool(&defense, &result, 12u);
            if (result.total >= 12u) { delay_frames(90u); return 1u; }
        } else if (key == 'a' || key == 'A') {
            protocol_roll(&attack, &result); show_pool(&attack, &result, 12u);
            if (result.total >= 12u) { damage = result.total >= 20u ? 8u : (result.total >= 16u ? 6u : 4u); enemy_hp = damage >= enemy_hp ? 0u : (uint8_t)(enemy_hp - damage); }
        } else continue;
        if (enemy_hp) {
            gotoxy(2u, 22u); textcolor(COLOR_ORANGE); cputs("STALKER ATTACK: "); protocol_roll(&defense, &result);
            if (result.total < 12u) { damage = guard ? 2u : 4u; hp = damage >= hp ? 0u : (uint8_t)(hp - damage); textcolor(COLOR_LIGHTRED); cprintf("HIT FOR %u.", damage); }
            else { textcolor(COLOR_LIGHTGREEN); cputs("EVADED."); }
        }
        guard = 0u; delay_frames(100u);
    }
    if (hp) { gotoxy(2u, 25u); textcolor(COLOR_LIGHTGREEN); cputs("THREAT NEUTRALIZED. PRESS ANY KEY."); cgetc(); return 1u; }
    return 0u;
}

static void skill_event(char tile) {
    ProtocolPool pool = {{8u, 8u, 6u, 6u}, 4u};
    ProtocolResult result;
    sprite_show(0u); bgcolor(COLOR_BLACK); bordercolor(COLOR_PURPLE); clrscr();
    textcolor(COLOR_YELLOW); gotoxy(2u, 2u); cputs("PROTOCOL SKILL TEST");
    textcolor(COLOR_WHITE); gotoxy(2u, 5u); cputs(tile == '?' ? "WIS / WIS / INT + INVESTIGATION D6" : "INT / INT / WIS + SYSTEMS D6");
    protocol_roll(&pool, &result); gotoxy(2u, 8u); show_pool(&pool, &result, 12u); gotoxy(2u, 13u);
    if (result.total >= 12u) {
        textcolor(COLOR_LIGHTGREEN);
        if (tile == '?') { cputs("CACHE FOUND: +1 MEDKIT."); ++medkits; }
        else { cputs("ACCESS GRANTED: AMBER GATE OPEN."); gate_open = 1u; }
    } else { textcolor(COLOR_LIGHTRED); cputs("SIGNAL COLLAPSED. TRY AGAIN LATER."); }
    gotoxy(2u, 17u); textcolor(COLOR_GRAY3); cputs("PRESS ANY KEY TO RETURN."); cgetc();
}

static uint8_t input_action(void) {
    uint8_t joy = joy_read(JOY_1); char key = 0;
    if (kbhit()) key = cgetc();
    if (key == 'w' || key == 'W' || JOY_UP(joy)) return ACT_UP;
    if (key == 's' || key == 'S' || JOY_DOWN(joy)) return ACT_DOWN;
    if (key == 'a' || key == 'A' || JOY_LEFT(joy)) return ACT_LEFT;
    if (key == 'd' || key == 'D' || JOY_RIGHT(joy)) return ACT_RIGHT;
    if (key == 'e' || key == 'E' || JOY_BTN_A(joy)) return ACT_USE;
    if (key == 'q' || key == 'Q') return ACT_QUIT;
    return ACT_NONE;
}

static uint8_t walkable(uint8_t x, uint8_t y) {
    char tile;
    if (x >= MAP_W || y >= MAP_H) return 0u; tile = map_rows[y][x];
    if (tile == '#' || tile == '~' || tile == 'T') return 0u;
    if (tile == 'D' && !gate_open) return 0u;
    return 1u;
}

static void use_tile(void) {
    char tile = map_rows[py][px];
    if (tile == '?' && medkits < 3u) skill_event('?');
    else if (tile == 'D' && !gate_open) skill_event('D');
    else if (tile == '+' && gate_open && !relic_found) {
        relic_found = 1u; sprite_show(0u); clrscr(); bordercolor(COLOR_YELLOW);
        textcolor(COLOR_YELLOW); gotoxy(4u, 8u); cputs("THE AMBER ARTIFACT IS YOURS.");
        textcolor(COLOR_WHITE); gotoxy(4u, 11u); cputs("THE WILDS HAVE NOT FINISHED WITH YOU.");
        gotoxy(4u, 15u); cputs("PRESS ANY KEY."); cgetc();
    }
}

static uint8_t title_screen(void) {
    char key;
    sprite_show(0u); videomode(VIDEOMODE_40x30); bordercolor(COLOR_RED); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_YELLOW); gotoxy(4u, 4u); cputs("P R O T O C O L");
    textcolor(COLOR_LIGHTRED); gotoxy(8u, 7u); cputs("A D V E N T U R E");
    textcolor(COLOR_WHITE); gotoxy(6u, 11u); cputs("COMMANDER X16 VERA SLICE");
    textcolor(COLOR_GRAY3); gotoxy(4u, 15u); cputs("PROTOCOL DICE: 1-3 BLANK, 4+ TOTAL");
    textcolor(COLOR_LIGHTGREEN); gotoxy(8u, 20u); cputs("PRESS ANY KEY TO DEPLOY");
    key = cgetc(); return key != 'q' && key != 'Q';
}

int main(void) {
    uint8_t action, nx, ny, joy_ok;
    protocol_seed(0x1985u); joy_ok = joy_install(joy_static_stddrv) == JOY_ERR_OK; upload_player_sprite();
    if (!title_screen()) return 0; draw_world();
    while (hp) {
        action = input_action();
        if (action == ACT_QUIT) break;
        if (action == ACT_USE) { use_tile(); draw_world(); delay_frames(12u); continue; }
        nx = px; ny = py;
        if (action == ACT_UP && ny) --ny; else if (action == ACT_DOWN) ++ny;
        else if (action == ACT_LEFT && nx) --nx; else if (action == ACT_RIGHT) ++nx;
        else { waitvsync(); continue; }
        if (nx < MAP_W && ny < MAP_H && map_rows[ny][nx] == 'D' && !gate_open) {
            skill_event('D'); draw_world(); delay_frames(12u); continue;
        }
        if (walkable(nx, ny)) {
            px = nx; py = ny; ++steps; walk_frame ^= 1u; sprite_position(); sprite_image(walk_frame); draw_hud();
            threat = (uint8_t)(threat + 2u + protocol_random(3u));
            if (threat >= 24u && map_rows[py][px] == '.') { threat = protocol_random(5u); if (!combat()) break; draw_world(); }
        }
        delay_frames(7u);
    }
    sprite_show(0u); bgcolor(COLOR_BLACK); clrscr(); gotoxy(5u, 10u);
    if (hp) { textcolor(COLOR_YELLOW); cputs("MISSION SUSPENDED."); }
    else { textcolor(COLOR_LIGHTRED); cputs("VECTOR LOST IN THE AMBER WILDS."); }
    gotoxy(5u, 14u); textcolor(COLOR_WHITE); cputs("RESET TO DEPLOY AGAIN.");
    if (joy_ok) joy_uninstall(); return 0;
}
