#include <conio.h>
#include <cx16.h>
#include <joystick.h>
#include <stdint.h>
#include "protocol_dice.h"
#include "game_content.h"
#include "x16_audio.h"
#include "savegame.h"

#define MAP_OX 1u
#define MAP_OY 1u
#define SPRITE_DATA 0x13000UL
#define ACTOR_DATA 0x13100UL
#define SPRITE_ATTR 0x1FC00UL
#define ACTOR_STYLES 8u
#define ACT_NONE 0u
#define ACT_UP 1u
#define ACT_DOWN 2u
#define ACT_LEFT 3u
#define ACT_RIGHT 4u
#define ACT_USE 5u
#define ACT_QUIT 6u
#define ACT_DEBUG 7u
#define ACT_FORCE 8u
#define ACT_SEED 9u
#define ACT_KEYS 10u

static uint8_t px = 2u, py = 15u, walk_frame = 0u;
static uint8_t party_hp[PARTY_SIZE] = {24u,18u,18u};
static uint8_t nanogel = 2u, rescued = 0u, carbon = 0u, telegram = 0u;
static uint8_t quill_met = 0u, carrier = 0u, exposure = 0u, fire_clock = 0u;
static uint8_t little_giant = 0u, venn_done = 0u, mission_done = 0u;
static uint16_t steps = 0u;

static void delay_frames(uint8_t frames) { while (frames--) waitvsync(); }

static char wait_confirm(void) {
    uint8_t joy; char key;
    while (1) {
        if (kbhit()) { key = cgetc(); audio_confirm(); return key; }
        joy = joy_read(JOY_1);
        if (JOY_BTN_A(joy)) {
            audio_confirm();
            while (JOY_BTN_A(joy_read(JOY_1))) waitvsync();
            return 13;
        }
        waitvsync();
    }
}

static void fade_out(void) {
    bordercolor(COLOR_GRAY2); delay_frames(2u);
    bordercolor(COLOR_GRAY1); delay_frames(2u);
    bordercolor(COLOR_BLACK); bgcolor(COLOR_BLACK); delay_frames(3u);
}

static void save_pack(SaveData* save) {
    save->magic[0] = 'P'; save->magic[1] = '7'; save->magic[2] = 'A'; save->magic[3] = 'S';
    save->version = SAVE_VERSION; save->px = px; save->py = py;
    save->party_hp[0] = party_hp[0]; save->party_hp[1] = party_hp[1]; save->party_hp[2] = party_hp[2];
    save->nanogel = nanogel; save->rescued = rescued; save->carbon = carbon; save->telegram = telegram;
    save->quill_met = quill_met; save->carrier = carrier; save->exposure = exposure; save->fire_clock = fire_clock;
    save->little_giant = little_giant; save->venn_done = venn_done; save->checksum = 0u;
}

static void save_unpack(const SaveData* save) {
    px = save->px; py = save->py;
    party_hp[0] = save->party_hp[0]; party_hp[1] = save->party_hp[1]; party_hp[2] = save->party_hp[2];
    nanogel = save->nanogel; rescued = save->rescued; carbon = save->carbon; telegram = save->telegram;
    quill_met = save->quill_met; carrier = save->carrier; exposure = save->exposure; fire_clock = save->fire_clock;
    little_giant = save->little_giant; venn_done = save->venn_done;
}

static uint8_t checkpoint_save(void) {
    SaveData save; save_pack(&save); return savegame_write(&save);
}

static uint8_t checkpoint_load(void) {
    SaveData save;
    if (!savegame_read(&save)) return 0u;
    save_unpack(&save); return 1u;
}

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
    if (frame == 0u) return ((x >= 4u && x <= 6u) || (x >= 9u && x <= 11u)) ? 15u : 0u;
    return ((x >= 3u && x <= 5u) || (x >= 10u && x <= 12u)) ? 15u : 0u;
}

static void upload_player_sprite(void) {
    uint8_t frame, y, x, left, right;
    unsigned long address = SPRITE_DATA;
    for (frame = 0u; frame < 2u; ++frame) for (y = 0u; y < 16u; ++y) {
        for (x = 0u; x < 16u; x += 2u) {
            left = player_pixel(x, y, frame); right = player_pixel((uint8_t)(x + 1u), y, frame);
            vpoke((uint8_t)((left << 4) | right), address++);
        }
    }
}

static uint8_t actor_pixel(uint8_t x, uint8_t y, uint8_t style) {
    static const uint8_t accent[ACTOR_STYLES] = {12u,6u,7u,2u,8u,3u,1u,5u};
    if (y < 2u || y > 14u || x < 3u || x > 12u) return 0u;
    if (y < 7u) {
        if (x < 5u || x > 10u) return 0u;
        if (y == 4u && (x == 6u || x == 9u)) return 1u;
        if (y == 6u && x >= 7u && x <= 8u) return 8u;
        return y < 3u ? accent[style] : 7u;
    }
    if (y < 12u) return (x >= 4u && x <= 11u) ? accent[style] : 0u;
    if (y < 15u && ((x >= 4u && x <= 6u) || (x >= 9u && x <= 11u))) return 15u;
    return 0u;
}

static void upload_actor_sprites(void) {
    uint8_t style, y, x, left, right;
    unsigned long address = ACTOR_DATA;
    for (style = 0u; style < ACTOR_STYLES; ++style) for (y = 0u; y < 16u; ++y) {
        for (x = 0u; x < 16u; x += 2u) {
            left = actor_pixel(x, y, style); right = actor_pixel((uint8_t)(x + 1u), y, style);
            vpoke((uint8_t)((left << 4) | right), address++);
        }
    }
}

static void upload_glyph(uint8_t code, const uint8_t* rows) {
    uint8_t y; unsigned long address = 0x1F000UL + ((unsigned long)code * 8UL);
    for (y = 0u; y < 8u; ++y) vpoke(rows[y], address++);
}

static void upload_custom_tiles(void) {
    static const uint8_t street[8] = {0x00u,0x00u,0x10u,0x00u,0x00u,0x02u,0x00u,0x00u};
    static const uint8_t brick[8] = {0xFFu,0x81u,0xFFu,0x18u,0xFFu,0x81u,0xFFu,0x18u};
    static const uint8_t rubble[8] = {0x81u,0x24u,0x5Au,0x18u,0x42u,0xBDu,0x24u,0x81u};
    static const uint8_t flame[8] = {0x10u,0x38u,0x28u,0x6Cu,0x54u,0xFEu,0x7Cu,0x38u};
    static const uint8_t water[8] = {0x00u,0x66u,0x99u,0x00u,0x33u,0xCCu,0x00u,0x66u};
    upload_glyph((uint8_t)'.', street); upload_glyph((uint8_t)'#', brick);
    upload_glyph((uint8_t)'%', rubble); upload_glyph((uint8_t)'*', flame); upload_glyph((uint8_t)'~', water);
}

static void actor_sprite(uint8_t slot, uint8_t style, uint8_t x, uint8_t y, uint8_t visible) {
    unsigned long attr = SPRITE_ATTR + ((unsigned long)slot * 8UL);
    unsigned long image = ACTOR_DATA + ((unsigned long)style * 128UL);
    uint16_t sx = (uint16_t)(MAP_OX + x) * 8u;
    uint16_t sy = (uint16_t)(MAP_OY + y) * 8u;
    vpoke((uint8_t)((image >> 5) & 0xFFu), attr);
    vpoke((uint8_t)((image >> 13) & 0x0Fu), attr + 1UL);
    vpoke((uint8_t)(sx & 0xFFu), attr + 2UL); vpoke((uint8_t)(sx >> 8), attr + 3UL);
    vpoke((uint8_t)(sy & 0xFFu), attr + 4UL); vpoke((uint8_t)(sy >> 8), attr + 5UL);
    vpoke(visible ? 0x0Cu : 0u, attr + 6UL); vpoke(0x50u, attr + 7UL);
}

static void field_actors(void) {
    actor_sprite(1u, 7u, 3u, 2u, !rescued);
    actor_sprite(2u, 3u, 27u, 9u, 1u);
    actor_sprite(3u, 4u, 27u, 6u, 1u);
    actor_sprite(4u, 5u, 27u, 13u, !venn_done);
    actor_sprite(5u, 0u, 0u, 0u, 0u);
    vera_sprites_enable(1u);
}

static uint8_t speaker_style(const char* name) {
    if (name[0] == 'M' && name[1] == 'A') return 0u;
    if (name[0] == 'E' && name[1] == 'L' && name[2] == 'I') return 1u;
    if (name[0] == 'N') return 2u;
    if (name[0] == 'M' && name[1] == 'I') return 3u;
    if (name[0] == 'E') return 4u;
    if (name[0] == 'S') return 5u;
    if (name[0] == 'A') return 6u;
    return 7u;
}

static void portrait_show(const char* name) {
    unsigned long attr = SPRITE_ATTR + 40UL;
    unsigned long image = ACTOR_DATA + ((unsigned long)speaker_style(name) * 128UL);
    vpoke((uint8_t)((image >> 5) & 0xFFu), attr); vpoke((uint8_t)((image >> 13) & 0x0Fu), attr + 1UL);
    vpoke(8u, attr + 2UL); vpoke(0u, attr + 3UL); vpoke(160u, attr + 4UL); vpoke(0u, attr + 5UL);
    vpoke(0x0Cu, attr + 6UL); vpoke(0x50u, attr + 7UL); vera_sprites_enable(1u);
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
    uint8_t slot;
    if (!visible) {
        for (slot = 0u; slot < 6u; ++slot) vpoke(0u, SPRITE_ATTR + ((unsigned long)slot * 8UL) + 6UL);
        vera_sprites_enable(0u); return;
    }
    vpoke(visible ? 0x0Cu : 0u, SPRITE_ATTR + 6UL);
    vpoke(0x50u, SPRITE_ATTR + 7UL);
    vera_sprites_enable(1u);
}

static uint8_t actor_gone(char tile) {
    if (tile == 'C') return rescued;
    if (tile == 'L') return carbon;
    if (tile == 'T') return telegram;
    if (tile == 'V') return venn_done;
    return 0u;
}

static void draw_cell(uint8_t x, uint8_t y, char tile) {
    gotoxy((uint8_t)(MAP_OX + x), (uint8_t)(MAP_OY + y)); revers(0);
    if (actor_gone(tile)) tile = '.';
    if (tile == '#') { textcolor(COLOR_GRAY2); bgcolor(COLOR_GRAY1); cputc('#'); }
    else if (tile == 'F') { textcolor(COLOR_YELLOW); bgcolor(COLOR_RED); cputc('*'); }
    else if (tile == '~') { textcolor(COLOR_CYAN); bgcolor(COLOR_BLUE); cputc('~'); }
    else if (tile == 'X') { textcolor(COLOR_GRAY3); bgcolor(COLOR_BROWN); cputc('%'); }
    else if (tile == 'C' || tile == 'Q' || tile == 'V' || tile == 'R') { textcolor(COLOR_GRAY3); bgcolor(COLOR_BLACK); cputc('.'); }
    else if (tile == 'L') { textcolor(COLOR_BLUE); bgcolor(COLOR_WHITE); cputc('L'); }
    else if (tile == 'T') { textcolor(COLOR_BLACK); bgcolor(COLOR_YELLOW); cputc('T'); }
    else if (tile == 'E') { textcolor(COLOR_YELLOW); bgcolor(COLOR_BLUE); cputc('E'); }
    else { textcolor(COLOR_GRAY3); bgcolor(COLOR_BLACK); cputc('.'); }
}

static void clear_rows(uint8_t first, uint8_t last) {
    uint8_t y;
    bgcolor(COLOR_BLACK);
    for (y = first; y <= last; ++y) { gotoxy(0u, y); cclear(40u); }
}

static void draw_hud(void) {
    clear_rows(20u, 29u);
    textcolor(COLOR_YELLOW); gotoxy(0u, 20u); cputs(" PROTOCOL 7: THE ASH LEDGER // V0.110");
    textcolor(COLOR_WHITE); gotoxy(0u, 21u); cprintf("M:%u E:%u N:%u  GEL:%u  FIRE:%u", party_hp[0], party_hp[1], party_hp[2], nanogel, fire_clock);
    textcolor(COLOR_CYAN); gotoxy(0u, 22u); cprintf("CARRIER:%s  EXP:%s", carrier_name(carrier), exposure_name(exposure));
    textcolor(COLOR_LIGHTGREEN); gotoxy(0u, 24u); cputs("WASD/JOY MOVE  E/A INTERACT  Q QUIT");
    textcolor(COLOR_GRAY3); gotoxy(0u, 26u);
    if (!carbon || !telegram) cputs("OBJECTIVE: FIND QUILL'S LOST RECORDS.");
    else if (!quill_met) cputs("OBJECTIVE: FIND THE BLUE-PENCIL WOMAN.");
    else if (!venn_done) cputs("OBJECTIVE: PROTECT THE CARRIER.");
    else cputs("OBJECTIVE: REACH FERRY EXTRACTION E.");
    gotoxy(0u, 28u); cputs("DEV: V STATE  R CONFRONT  N SEED  K KEYS");
}

static void draw_world(void) {
    uint8_t x, y;
    videomode(VIDEOMODE_40x30); upload_custom_tiles(); bordercolor(COLOR_ORANGE); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_YELLOW); gotoxy(1u, 0u); cputs("SAN FRANCISCO // THE FALLING HOUR");
    for (y = 0u; y < MAP_H; ++y) for (x = 0u; x < MAP_W; ++x) draw_cell(x, y, ash_map[y][x]);
    draw_hud(); sprite_position(); sprite_image(walk_frame); sprite_show(1u); field_actors();
}

static void dialogue(const char* name, const char* one, const char* two, const char* three) {
    sprite_show(0u); clear_rows(19u, 29u);
    textcolor(COLOR_YELLOW); gotoxy(1u, 19u); cputs("+--------------------------------------+");
    portrait_show(name); gotoxy(4u, 20u); revers(1); cputs(name); revers(0);
    textcolor(COLOR_WHITE); gotoxy(2u, 22u); cputs(one);
    gotoxy(2u, 24u); cputs(two);
    gotoxy(2u, 26u); cputs(three);
    textcolor(COLOR_GRAY3); gotoxy(8u, 28u); cputs("PRESS A / ENTER"); wait_confirm();
}

static void pool_from(const uint8_t* dice, ProtocolPool* pool) {
    uint8_t i; pool->count = 4u;
    for (i = 0u; i < 4u; ++i) pool->sides[i] = dice[i];
}

static uint8_t skill_scene(uint8_t who, const uint8_t* dice, const char* skill, uint8_t target) {
    ProtocolPool pool; ProtocolResult result; uint8_t i;
    pool_from(dice, &pool); protocol_roll(&pool, &result);
    sprite_show(0u); bgcolor(COLOR_BLACK); bordercolor(party_defs[who].color); clrscr();
    textcolor(party_defs[who].color); gotoxy(2u, 3u); cprintf("%s // %s", party_defs[who].name, skill);
    textcolor(COLOR_YELLOW); gotoxy(2u, 7u); cputs("PROTOCOL DICE"); gotoxy(2u, 9u);
    for (i = 0u; i < result.count; ++i) {
        textcolor(result.faces[i] < 4u ? COLOR_GRAY2 : COLOR_WHITE);
        cprintf("D%u[%u] ", pool.sides[i], result.faces[i]);
    }
    textcolor(COLOR_YELLOW); gotoxy(2u, 12u); cprintf("TOTAL %u   DIFFICULTY %u", result.total, target);
    gotoxy(2u, 15u);
    if (result.total >= target) { textcolor(COLOR_LIGHTGREEN); cputs("SUCCESS"); audio_success(); }
    else { textcolor(COLOR_LIGHTRED); cputs("FAILURE -- THE STORY MOVES FORWARD"); audio_failure(); }
    textcolor(COLOR_GRAY3); gotoxy(2u, 19u); cputs("1-3 ARE BLANK. EVERY 4+ SCORES.");
    gotoxy(2u, 23u); cputs("PRESS A / ENTER."); wait_confirm();
    return result.total >= target;
}

static uint8_t interactive(char tile) {
    return tile == 'C' || tile == 'L' || tile == 'T' || tile == 'Q' || tile == 'V' || tile == 'R' || tile == 'E';
}

static char nearby_tile(void) {
    char tile = ash_map[py][px];
    if (interactive(tile)) return tile;
    if (py && interactive(ash_map[py - 1u][px])) return ash_map[py - 1u][px];
    if (py + 1u < MAP_H && interactive(ash_map[py + 1u][px])) return ash_map[py + 1u][px];
    if (px && interactive(ash_map[py][px - 1u])) return ash_map[py][px - 1u];
    if (px + 1u < MAP_W && interactive(ash_map[py][px + 1u])) return ash_map[py][px + 1u];
    return '.';
}

static uint8_t party_alive(void) { return party_hp[0] || party_hp[1] || party_hp[2]; }

static void draw_battle(uint8_t venn_hp, uint8_t doubt, uint8_t ap, uint8_t active, uint8_t command) {
    uint8_t y;
    sprite_show(0u); bordercolor(COLOR_RED); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_LIGHTRED); gotoxy(2u, 1u); cputs("COVENANT INTERVENTION // SILAS VENN");
    textcolor(COLOR_GRAY3); gotoxy(11u, 4u); cputs("   ____   ");
    gotoxy(11u, 5u); cputs("  / __ \\  "); gotoxy(11u, 6u); cputs(" | /  \\ | ");
    gotoxy(11u, 7u); cputs(" | \\__/ | "); gotoxy(11u, 8u); cputs("  \\____/  ");
    textcolor(COLOR_WHITE); gotoxy(2u, 11u); cprintf("VENN HP:%u/26  DOUBT:%u/12  AP:%u", venn_hp, doubt, ap);
    for (y = 0u; y < PARTY_SIZE; ++y) {
        gotoxy(2u, (uint8_t)(13u + y)); textcolor(y == active ? party_defs[y].color : COLOR_GRAY3);
        cprintf("%u %s %-11s HP %u/%u", y + 1u, party_defs[y].name, party_defs[y].role, party_hp[y], party_defs[y].max_hp);
    }
    clear_rows(17u, 28u); textcolor(COLOR_YELLOW);
    gotoxy(2u, 17u); cprintf("%c ATTACK", command == 0u ? '>' : ' ');
    gotoxy(2u, 18u); cprintf("%c REASON", command == 1u ? '>' : ' ');
    gotoxy(2u, 19u); cprintf("%c GUARD", command == 2u ? '>' : ' ');
    gotoxy(2u, 20u); cprintf("%c NANOGEL", command == 3u ? '>' : ' ');
    gotoxy(2u, 21u); cprintf("%c END TURN", command == 4u ? '>' : ' ');
    textcolor(COLOR_GRAY3); gotoxy(19u, 18u); cputs("UP/DOWN + BUTTON A");
    gotoxy(19u, 20u); cputs("1/2/3 SELECT VECTOR");
}

static char battle_input(uint8_t* command) {
    static const char keys[5] = {'a','t','g','h','e'};
    uint8_t joy; char key;
    while (1) {
        if (kbhit()) return cgetc();
        joy = joy_read(JOY_1);
        if (JOY_UP(joy)) {
            *command = *command ? (uint8_t)(*command - 1u) : 4u;
            while (JOY_UP(joy_read(JOY_1))) waitvsync(); return 0;
        }
        if (JOY_DOWN(joy)) {
            *command = *command < 4u ? (uint8_t)(*command + 1u) : 0u;
            while (JOY_DOWN(joy_read(JOY_1))) waitvsync(); return 0;
        }
        if (JOY_BTN_A(joy)) {
            key = keys[*command]; audio_confirm();
            while (JOY_BTN_A(joy_read(JOY_1))) waitvsync(); return key;
        }
        waitvsync();
    }
}

static void battle_message(const char* label, uint8_t left, uint8_t right, uint8_t amount) {
    textcolor(COLOR_YELLOW); gotoxy(2u, 23u); cputs(label);
    textcolor(COLOR_WHITE); gotoxy(2u, 25u); cprintf("VECTOR %u  //  VENN %u", left, right);
    gotoxy(2u, 27u); cprintf("MARGIN %u -- PRESS A", amount); wait_confirm();
}

static uint8_t confrontation(void) {
    ProtocolPool a_pool, v_pool; ProtocolResult a_roll, v_roll;
    uint8_t venn_hp = 26u, doubt = 0u, ap = 3u, active = 0u, guard = 0u, command = 0u;
    uint8_t i, target, amount; char key;
    const uint8_t venn_def[4] = {8u,8u,8u,8u};
    const uint8_t venn_attack[4] = {8u,8u,8u,8u};
    while (venn_hp && doubt < 12u && party_alive()) {
        if (!party_hp[active]) for (i = 0u; i < PARTY_SIZE; ++i) if (party_hp[i]) { active = i; break; }
        draw_battle(venn_hp, doubt, ap, active, command); key = battle_input(&command);
        if (!key) continue;
        if (key >= '1' && key <= '3') { i = (uint8_t)(key - '1'); if (party_hp[i]) active = i; continue; }
        if (key == 'e' || key == 'E') ap = 0u;
        else if ((key == 'a' || key == 'A') && ap) {
            pool_from(party_defs[active].attack, &a_pool); pool_from(venn_def, &v_pool);
            a_pool.sides[a_pool.count++] = 4u;
            protocol_roll(&a_pool, &a_roll); protocol_roll(&v_pool, &v_roll);
            amount = a_roll.total > v_roll.total ? (uint8_t)(a_roll.total - v_roll.total) : 0u;
            venn_hp = amount >= venn_hp ? 0u : (uint8_t)(venn_hp - amount); --ap;
            if (amount) audio_success(); else audio_failure(); battle_message("OPPOSED ATTACK", a_roll.total, v_roll.total, amount);
        } else if ((key == 't' || key == 'T') && ap) {
            pool_from(party_defs[active].influence, &a_pool); pool_from(venn_def, &v_pool);
            protocol_roll(&a_pool, &a_roll); protocol_roll(&v_pool, &v_roll);
            amount = a_roll.total > v_roll.total ? (uint8_t)(a_roll.total - v_roll.total) : 0u;
            doubt = (uint8_t)(doubt + amount); --ap;
            if (amount) audio_success(); else audio_failure(); battle_message("OPPOSED INFLUENCE", a_roll.total, v_roll.total, amount);
        } else if ((key == 'g' || key == 'G') && ap) { guard = 1u; --ap; }
        else if ((key == 'h' || key == 'H') && ap && nanogel) {
            --nanogel; party_hp[active] = party_hp[active] > (uint8_t)(party_defs[active].max_hp - 6u) ? party_defs[active].max_hp : (uint8_t)(party_hp[active] + 6u);
            if (exposure < 5u) ++exposure; --ap;
        } else continue;
        if (!ap && venn_hp && doubt < 12u) {
            do { target = protocol_random(PARTY_SIZE); } while (!party_hp[target]);
            pool_from(venn_attack, &v_pool); v_pool.sides[v_pool.count++] = 6u; pool_from(party_defs[target].defense, &a_pool);
            if (guard && target == active && a_pool.count < PROTOCOL_MAX_DICE) a_pool.sides[a_pool.count++] = 6u;
            protocol_roll(&v_pool, &v_roll); protocol_roll(&a_pool, &a_roll);
            amount = v_roll.total > a_roll.total ? (uint8_t)(v_roll.total - a_roll.total) : 0u;
            party_hp[target] = amount >= party_hp[target] ? 0u : (uint8_t)(party_hp[target] - amount);
            draw_battle(venn_hp, doubt, 0u, active, command); if (amount) audio_alarm(); battle_message("VENN'S COUNTERATTACK", a_roll.total, v_roll.total, amount);
            ap = 3u; guard = 0u;
        }
    }
    if (!party_alive()) return 0u;
    if (doubt >= 12u) dialogue("SILAS VENN", "YOU MAY BE RIGHT ABOUT THE METHOD.", "BUT ASK WHO PROFITS FROM ITS FUTURE.", "HE LOWERS HIS REVOLVER AND WITHDRAWS.");
    else dialogue("MARA", "VENN IS DOWN. THE STREET IS QUIET.", "THE LEDGER STILL HAS TO SURVIVE US.", "THE FERRY WINDOW IS CLOSING.");
    venn_done = 1u; return 1u;
}

static void interaction(void) {
    char tile = nearby_tile(); uint8_t success;
    if (tile == 'C' && !rescued) {
        dialogue("MARA", "A CHILD IS TRAPPED UNDER THE AWNING.", "THE NEXT AFTERSHOCK COULD DROP THE WALL.", "WE MOVE NOW.");
        success = skill_scene(0u, party_defs[0].attack, "ATHLETICS", 12u); rescued = 1u;
        if (!success && fire_clock < 4u) ++fire_clock;
        dialogue("ELIAS ROURKE", "YOU PULLED THEM OUT. I SAW IT.", "IF YOU NEED A ROUTE THROUGH THIS CITY,", "FIND ME NEAR THE RELIEF POST.");
    } else if (tile == 'L' && !carbon) {
        dialogue("ELI", "PACIFIC ASSURANCE ANNEX.", "THE RECORDS ROOM IS READY TO FALL.", "I CAN REACH THE BLUE CARBON SHEETS.");
        success = skill_scene(1u, party_defs[1].defense, "SECURITY", 12u); carbon = 1u;
        if (!success && fire_clock < 4u) ++fire_clock;
        dialogue("NIA", "THE COMPARISONS ARE REPRODUCIBLE.", "BRACING. FOUNDATION. FIREBREAKS.", "THESE NOTES ARE THE TARGET.");
    } else if (tile == 'T' && !telegram) {
        dialogue("NIA", "HALF-SENT TELEGRAM FROM OAKLAND:", "QUILL -- SEND DUPLICATE MEASUREMENTS.", "PROFESSOR ADA HALDEN IS A SECOND ROUTE.");
        success = skill_scene(2u, party_defs[2].influence, "CULTURE", 12u); telegram = 1u;
        if (!success && exposure < 5u) ++exposure;
    } else if (tile == 'Q') {
        if (!carbon && !telegram) dialogue("MIRIAM QUILL", "IF YOU'RE HERE TO TELL ME TO LEAVE,", "TAKE A NUMBER. MY WORK IS NOT COPIED.", "FIND THE RECORDS OR HALDEN'S MESSAGE.");
        else {
            quill_met = 1u; carrier = (carbon && telegram) ? 2u : 1u;
            dialogue("MIRIAM QUILL", "MEMORY IS USELESS UNLESS ANOTHER", "ENGINEER CAN REPEAT THE COMPARISON.", "SAVE THE METHOD. NOT MY REPUTATION.");
            dialogue("ALETHEIA", "CONFIRMATION: THE OBSERVATIONS ARE", "THE HISTORICAL CARRIER.", "ESTABLISH AN INDEPENDENT COPY.");
            if (checkpoint_save()) dialogue("ALETHEIA", "CHECKPOINT RECORDED.", "THE IDENTIFIED CARRIER IS PRESERVED.", "THE COVENANT IS NOW MOVING.");
            else dialogue("ALETHEIA", "CHECKPOINT COULD NOT BE WRITTEN.", "CONTINUE THE OPERATION.", "VERIFY DEVICE 8 BEFORE RESETTING.");
        }
    } else if (tile == 'R') {
        if (rescued && quill_met && !little_giant) {
            little_giant = 1u; dialogue("ELIAS ROURKE", "20TH AND CHURCH? I'LL SEND A RUNNER.", "ONE SMALL HYDRANT AGAINST THE FIRE.", "LET'S HOPE QUILL'S PRESSURE NOTE IS RIGHT.");
        } else if (!rescued) dialogue("ELIAS ROURKE", "RELIEF CREWS FIRST. QUESTIONS LATER.", "THERE ARE PEOPLE TRAPPED NORTH OF HERE.", "HELP THEM AND WE'LL TALK.");
        else dialogue("ELIAS ROURKE", "KEEP MOVING. THE ROUTES ARE CLOSING.", "THE FERRY ARCADE IS STILL OPEN.", "FOR NOW.");
    } else if (tile == 'V' && !venn_done) {
        if (!quill_met) dialogue("STRANGER", "A MAN IN A DARK WORK COAT WATCHES YOU.", "HE TOUCHES TWO FINGERS TO HIS COLLAR.", "THEN DISAPPEARS INTO THE SMOKE.");
        else {
            dialogue("SILAS VENN", "SAVE QUILL. I WILL HELP YOU.", "BUT BURN THE LEDGER. ASK YOUR MACHINE", "WHY IT NEEDS THIS PARTICULAR FUTURE.");
            if (confrontation()) {
                if (checkpoint_save()) dialogue("ALETHEIA", "CHECKPOINT RECORDED.", "PARTY AND MISSION STATE ARE PRESERVED.", "CONTINUE TO FERRY EXTRACTION.");
                else dialogue("ALETHEIA", "CHECKPOINT COULD NOT BE WRITTEN.", "CONTINUE THE OPERATION.", "VERIFY DEVICE 8 BEFORE RESETTING.");
            }
        }
    } else if (tile == 'E') {
        if (venn_done && carrier >= 2u) mission_done = 1u;
        else if (carrier < 2u) dialogue("ALETHEIA", "MISSION CONDITION NOT SATISFIED.", "THE CARRIER MUST BE ESTABLISHED.", "FIND TWO INDEPENDENT RECORD ROUTES.");
        else dialogue("ALETHEIA", "COVENANT INTERFERENCE REMAINS ACTIVE.", "THE CARRIER WILL NOT REACH THE FERRY.", "RESOLVE THE MAN IN THE DARK COAT.");
    } else dialogue("NIA", "ASH. BRICK. BROKEN GLASS.", "NOTHING HERE IDENTIFIES THE CARRIER.", "WE KEEP LOOKING.");
}

static void debug_state(void) {
    sprite_show(0u); bordercolor(COLOR_CYAN); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_CYAN); gotoxy(2u, 2u); cputs("DEVELOPER STATE // NON-CANONICAL");
    textcolor(COLOR_WHITE); gotoxy(2u, 5u); cprintf("POS %u,%u  STEPS %u", px, py, steps);
    gotoxy(2u, 7u); cprintf("RESCUE %u  CARBON %u  TELEGRAM %u", rescued, carbon, telegram);
    gotoxy(2u, 9u); cprintf("QUILL %u  VENN %u  HYDRANT %u", quill_met, venn_done, little_giant);
    gotoxy(2u, 11u); cprintf("CARRIER %s", carrier_name(carrier));
    gotoxy(2u, 13u); cprintf("EXPOSURE %s  FIRE %u", exposure_name(exposure), fire_clock);
    gotoxy(2u, 16u); cprintf("HP MARA %u  ELI %u  NIA %u", party_hp[0], party_hp[1], party_hp[2]);
    textcolor(COLOR_GRAY3); gotoxy(2u, 21u); cputs("PRESS A / ENTER."); wait_confirm();
}

static uint8_t input_action(void) {
    uint8_t joy = joy_read(JOY_1); char key = 0;
    if (kbhit()) key = cgetc();
    if (key == 'w' || key == 'W' || JOY_UP(joy)) return ACT_UP;
    if (key == 's' || key == 'S' || JOY_DOWN(joy)) return ACT_DOWN;
    if (key == 'a' || key == 'A' || JOY_LEFT(joy)) return ACT_LEFT;
    if (key == 'd' || key == 'D' || JOY_RIGHT(joy)) return ACT_RIGHT;
    if (key == 'e' || key == 'E' || JOY_BTN_A(joy)) return ACT_USE;
    if (key == 'v' || key == 'V') return ACT_DEBUG;
    if (key == 'r' || key == 'R') return ACT_FORCE;
    if (key == 'n' || key == 'N') return ACT_SEED;
    if (key == 'k' || key == 'K') return ACT_KEYS;
    if (key == 'q' || key == 'Q') return ACT_QUIT;
    return ACT_NONE;
}

static uint8_t walkable(uint8_t x, uint8_t y) {
    char tile;
    if (x >= MAP_W || y >= MAP_H) return 0u;
    tile = ash_map[y][x];
    if (tile == '#' || tile == 'F' || tile == '~' || tile == 'X') return 0u;
    if ((tile == 'C' || tile == 'L' || tile == 'T' || tile == 'V') && !actor_gone(tile)) return 0u;
    if (tile == 'Q' || tile == 'R') return 0u;
    return 1u;
}

static uint8_t title_screen(void) {
    uint8_t selected = 0u, joy; char key;
    sprite_show(0u); videomode(VIDEOMODE_40x30); bordercolor(COLOR_RED); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_LIGHTRED); gotoxy(6u, 3u); cputs("P R O T O C O L   S E V E N");
    textcolor(COLOR_YELLOW); gotoxy(10u, 7u); cputs("THE ASH LEDGER");
    textcolor(COLOR_GRAY3); gotoxy(8u, 10u); cputs("A COMMANDER X16 RPG");
    textcolor(COLOR_BLUE); gotoxy(3u, 14u); cputs("SAN FRANCISCO // APRIL 1906");
    textcolor(COLOR_WHITE); gotoxy(3u, 17u); cputs("WHAT SURVIVES BECOMES HISTORY.");
    textcolor(COLOR_GRAY2); gotoxy(13u, 27u); cputs("V0.110 PRE-ALPHA");
    while (1) {
        textcolor(selected == 0u ? COLOR_LIGHTGREEN : COLOR_GRAY3); gotoxy(10u, 21u); cputs(selected == 0u ? "> NEW OPERATION " : "  NEW OPERATION ");
        textcolor(selected == 1u ? COLOR_LIGHTGREEN : COLOR_GRAY3); gotoxy(10u, 23u); cputs(selected == 1u ? "> CONTINUE      " : "  CONTINUE      ");
        textcolor(COLOR_GRAY2); gotoxy(7u, 25u); cputs("UP/DOWN + A  OR N/L");
        if (kbhit()) {
            key = cgetc();
            if (key == 'q' || key == 'Q') return 0u;
            if (key == 'n' || key == 'N') { audio_confirm(); fade_out(); return 1u; }
            if (key == 'l' || key == 'L') { audio_confirm(); fade_out(); return 2u; }
        }
        joy = joy_read(JOY_1);
        if (JOY_UP(joy) || JOY_DOWN(joy)) {
            selected ^= 1u;
            while (JOY_UP(joy_read(JOY_1)) || JOY_DOWN(joy_read(JOY_1))) waitvsync();
        } else if (JOY_BTN_A(joy)) {
            audio_confirm(); while (JOY_BTN_A(joy_read(JOY_1))) waitvsync(); fade_out(); return (uint8_t)(selected + 1u);
        }
        waitvsync();
    }
}

static void briefing(void) {
    bgcolor(COLOR_BLACK); bordercolor(COLOR_CYAN); clrscr();
    textcolor(COLOR_CYAN); gotoxy(2u, 2u); cputs("ALETHEIA // OPERATION P7-AP01-M01");
    textcolor(COLOR_YELLOW); gotoxy(2u, 5u); cputs("THE ELECTRICAL ARGOSY // OCT 1905");
    textcolor(COLOR_WHITE); gotoxy(2u, 8u); cputs("NO RECORD OF THIS PUBLICATION EXISTS.");
    gotoxy(2u, 10u); cputs("IT DESCRIBES A CITY NOT YET BURNED.");
    gotoxy(2u, 13u); cputs("THE DISASTER IS NOT YOUR TARGET.");
    textcolor(COLOR_LIGHTRED); gotoxy(2u, 16u); cputs("DETERMINE WHAT MUST SURVIVE IT.");
    textcolor(COLOR_GRAY3); gotoxy(2u, 22u); cputs("CARRIER REQUIRED: ESTABLISHED");
    gotoxy(2u, 24u); cputs("EXPOSURE TARGET: TRACE OR BETTER");
    gotoxy(9u, 27u); cputs("PRESS A / ENTER"); wait_confirm(); fade_out();
    clrscr(); textcolor(COLOR_ORANGE); gotoxy(2u, 4u); cputs("THE GROUND MOVES BEFORE YOU LAND.");
    textcolor(COLOR_WHITE); gotoxy(2u, 8u); cputs("BRICK FALLS. BELLS RING WITHOUT RHYTHM.");
    gotoxy(2u, 11u); cputs("THE SKY TURNS THE COLOR OF COPPER.");
    gotoxy(2u, 15u); cputs("A CHILD CRIES BENEATH AN AWNING.");
    textcolor(COLOR_CYAN); gotoxy(2u, 20u); cputs("NO ONE IS LOOKING AT YOU YET.");
    textcolor(COLOR_GRAY3); gotoxy(9u, 25u); cputs("PRESS A / ENTER"); wait_confirm(); fade_out();
}

static void verification(void) {
    sprite_show(0u); bordercolor(COLOR_CYAN); bgcolor(COLOR_BLACK); clrscr();
    textcolor(COLOR_CYAN); gotoxy(2u, 2u); cputs("HISTORICAL VERIFICATION");
    textcolor(COLOR_WHITE); gotoxy(2u, 5u); cputs("QUILL'S NAME APPEARS ONLY TWICE.");
    gotoxy(2u, 7u); cputs("HER METHOD APPEARS HUNDREDS OF TIMES.");
    textcolor(COLOR_YELLOW); gotoxy(2u, 10u); cprintf("CARRIER: %s", carrier_name(carrier));
    gotoxy(2u, 12u); cprintf("EXPOSURE: %s", exposure_name(exposure));
    if (little_giant) { textcolor(COLOR_ORANGE); gotoxy(2u, 15u); cputs("A SMALL HYDRANT IS PAINTED GOLD."); }
    textcolor(COLOR_WHITE); gotoxy(2u, 18u); cputs("PAGE 21 IS NO LONGER MISSING.");
    textcolor(COLOR_LIGHTRED); gotoxy(2u, 21u); cputs("SEVEN PENCIL STROKES MARK THE MARGIN.");
    textcolor(COLOR_CYAN); gotoxy(2u, 24u); cputs("ALETHEIA: I DID NOT PLACE THAT THERE.");
    textcolor(COLOR_GRAY3); gotoxy(8u, 28u); cputs("END OF V0.110"); wait_confirm();
}

int main(void) {
    uint8_t action, nx, ny, joy_ok, start_mode;
    protocol_seed(0x1906u); joy_ok = joy_install(joy_static_stddrv) == JOY_ERR_OK; audio_init(); upload_player_sprite(); upload_actor_sprites();
    start_mode = title_screen(); if (!start_mode) return 0;
    if (start_mode == 2u) {
        if (!checkpoint_load()) { dialogue("ALETHEIA", "NO VALID CHECKPOINT WAS FOUND.", "A NEW OPERATION WILL BEGIN.", "DEVICE 8 REMAINS AVAILABLE FOR SAVES."); briefing(); }
    } else briefing();
    draw_world();
    while (party_alive() && !mission_done) {
        action = input_action();
        if (action == ACT_QUIT) break;
        if (action == ACT_USE) { interaction(); draw_world(); delay_frames(10u); continue; }
        if (action == ACT_DEBUG) { debug_state(); draw_world(); continue; }
        if (action == ACT_SEED) { protocol_seed(0x1906u); dialogue("DEVELOPER CONTROL", "PROTOCOL DICE SEED RESET TO 1906.", "TEST ROLLS ARE NOW REPRODUCIBLE.", "THIS DOES NOT CHANGE MISSION CANON."); draw_world(); continue; }
        if (action == ACT_KEYS) { carbon = telegram = quill_met = 1u; carrier = 2u; dialogue("DEVELOPER CONTROL", "INVESTIGATION KEYS GRANTED.", "CARRIER SET TO ESTABLISHED.", "THIS DOES NOT CHANGE MISSION CANON."); draw_world(); continue; }
        if (action == ACT_FORCE) {
            carbon = telegram = quill_met = 1u; carrier = 2u;
            if (!venn_done && !confrontation()) break; draw_world(); continue;
        }
        nx = px; ny = py;
        if (action == ACT_UP && ny) --ny; else if (action == ACT_DOWN) ++ny;
        else if (action == ACT_LEFT && nx) --nx; else if (action == ACT_RIGHT) ++nx;
        else { waitvsync(); continue; }
        if (walkable(nx, ny)) {
            px = nx; py = ny; ++steps; walk_frame ^= 1u;
            sprite_position(); sprite_image(walk_frame); draw_hud(); audio_step(walk_frame);
        }
        delay_frames(6u);
    }
    if (mission_done) verification();
    else {
        sprite_show(0u); bgcolor(COLOR_BLACK); clrscr(); gotoxy(5u, 11u);
        textcolor(party_alive() ? COLOR_YELLOW : COLOR_LIGHTRED);
        cputs(party_alive() ? "OPERATION SUSPENDED." : "THE VECTOR TEAM IS LOST.");
        gotoxy(5u, 15u); textcolor(COLOR_WHITE); cputs("RESET TO INSERT AGAIN.");
    }
    audio_stop(); if (joy_ok) joy_uninstall(); return 0;
}
