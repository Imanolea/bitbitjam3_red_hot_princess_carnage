// Red hot princess carnage | BitBitJam 3
// Light Games 2016
#include <gb/gb.h>
#include <gb/hardware.h>

// Ficheros de datos
// Gráficos
#include "data/graphics/sprite_tileset.h"
#include "data/graphics/sprite_bkg_tileset.h"
#include "data/graphics/bkg_tileset.h"
// Mapas
#include "data/maps/level0_map.h"
#include "data/maps/gui_map.h"

// GUI
#define GUI_POINTS_X     16U
#define GUI_HEARTHS_X    1U
// Tiles
#define DIGIT0_TILE         196U
#define EMPTYHEARTH_TILE    193U
#define HALFHEARTH_TILE     194U
#define FULLHEARTH_TILE     195U
// Valores del sistema
#define MAX_SP              40U
#define STD_ORIENTATION     0U
#define INV_ORIENTATION     S_FLIPX
// Animaciones
#define PRIN_IDLE           0U
#define PRIN_CROUCH         1U
#define PRIN_JUMP           2U
// Valores del juego
#define PRIN_GROUND_Y       88U
#define PRIN_HIT_DUR        8U
// Flags
#define ANIMATION_CHANGE    1U

// Tablas de datos

// Graficas
BYTE frame_list[] = {
    1,0,0,  2,8,6,  3,8,-2,  4,16,5,  5,16,-3,  6,24,5,  7,24,-3,  8,24,-11,  9,32,6, 10,32,-2,  11,32,-10,  127,  // 0 - Princesa en guardia
    1,10,0,  2,18,6,  3,18,-2,  12,26,5,  13,26,-3,  14,32,-11,  15,34,5,  16,34,-3,  127, // 34 - Princesa agachada
    1,0,0,  2,8,6,  3,8,-2,  17,16,6,  18,16,-2,  19,24,6,  20,24,-2,  21,32,-2,  127, // 59 - Princesa despegando
    1,0,0,  22,8,2,  23,8,-6,  24,16,2,  29,16,-6,  25,24,1,  26,24,-7,  27,32,1,  28,32,-7,  30,40,-2,  127, // 84 - Princesa en el aire
    1,0,0,  2,8,16,  3,8,-2,  4,16,5,  5,16,-3,  6,24,5,  7,24,-3,  8,24,-11,  9,32,6, 10,32,-2,  11,32,-10,  127,  // 115 - Princesa en guardia golpe
    1,10,0,  2,18,16,  3,18,-2,  12,26,5,  13,26,-3,  14,32,-11,  15,34,5,  16,34,-3,  127, // 149 - Princesa agachada golpe
    1,0,0,  22,8,16,  23,8,-6,  24,16,2,  29,16,-6,  25,24,1,  26,24,-7,  27,32,1,  28,32,-7,  30,40,-2,  127, // 174 - Princesa en el aire golpe
};

UWORD frame_table[] = {
    0, // 0 - Princesa en guardia
    34, // 1 - Princesa agachada
    59, // 2 - Princesa despegando
    84, // 3 - Princesa en el aire
    115, // 4 - Princesa en guardia golpe
    149, // 5 - Princesa agachada golpe
    174, // 6 - Princesa en el aire golpe
};

UBYTE animation_list[] = {
    0, 254, 255, // 0 - Princesa en guardia
    1, 254, 255, // 3 - Princesa agachada
    2,   3,   3,  35, 255, // 6 - Princesa saltando
};

UWORD animation_table[] = {
    0, // 0 - Princesa en guardia
    3, // 1 - Princesa agachada
    6, // 2 - Princesa saltando
};

// Lógica
BYTE prin_jump_table[] = {
    0, -3, -3, -2, -7, -2, -2, -1, -2, -1, -2, -1, -1, -1, -1, -1, 0, -1, 0, -1, 0, 0, 0, 1, 0, 1, 0, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 3, 2, 8, 127
};

// Estructuras
typedef struct {
    UBYTE null;
    UBYTE x;
    UBYTE y;
    UBYTE frame;
    UBYTE orientation;
    UBYTE animation_no;
    UBYTE animation_f;
    UBYTE animation_i;
    UBYTE animation_t;
    UBYTE state;
} Character;

Character prin;
UBYTE prin_health;
UBYTE prin_jump_i;
UBYTE prin_hit_t;
UBYTE sprite_no_i;
UBYTE pre_joypad;
UBYTE bkg_x;
UBYTE bkg_y;
UBYTE gui_points;
// Semillas
UBYTE logic_counter;
// Listas
UBYTE gui_points_tiles[3];
UBYTE gui_hearths_tiles[4];

// Declaración de funciones
void init();
void init_var();
void init_sprite();
void init_bkg();
void init_gui();
void init_interrupts();
void logic();
void logic_prin();
void logic_prin_control();
void logic_prin_move();
void upd();
void upd_characters();
void upd_prin();
void upd_prin_hit();
void upd_character(Character *character);
void upd_character_animation(Character *character);
void upd_character_sprite(Character *character);
void upd_bkg();
void upd_gui();
void upd_gui_points();
void upd_gui_hearths();
void draw_bkg();
void draw_gui();

// Ensamblador
WORD div8(UBYTE numerator, UBYTE denominator);
WORD mul8(UBYTE factor_1, UBYTE factor_2);
WORD toneg8(UBYTE value, UBYTE flag);
WORD leftshift(WORD value, UBYTE num);
WORD rightshift(WORD value, UBYTE num);
WORD assign(UWORD value);
WORD comp(UBYTE value_1, UBYTE value_2);

void main() {
    init();
    while (1) {
        logic();
        upd();
        wait_vbl_done();
    }
}

void init() { 
    disable_interrupts();
    DISPLAY_OFF;
    init_var();
    init_sprite();
    init_bkg();
    init_gui();
    init_interrupts();
    DISPLAY_ON;
    enable_interrupts();
}

void init_var() {
    prin.x = 85;
    prin.y = PRIN_GROUND_Y;
    prin.frame = 0;
    prin.orientation = STD_ORIENTATION;
    prin.animation_no = 0;
    prin.animation_f = 0;
    prin.animation_i = 0;
    prin.animation_t = 1;
    prin.state = PRIN_IDLE;
    prin_health = 8;
    prin_jump_i = 0;
    prin_hit_t = 0;
    sprite_no_i = 0;
    pre_joypad = 0;
    bkg_x = 0;
    bkg_y = 0;
    gui_points = 0;
}

void init_bkg() {
    SWITCH_ROM_MBC1(bkg_tilesetBank);
    set_bkg_data(0, 128, bkg_tileset);
    SWITCH_ROM_MBC1(level0_mapBank);
    set_bkg_tiles(0, 0, 32, 32, level0_map);
    move_bkg(bkg_x, bkg_y);
    SHOW_BKG;
}

void init_gui() {
    SWITCH_ROM_MBC1(sprite_bkg_tilesetBank);
    set_win_data(128, 256, sprite_bkg_tileset);
    SWITCH_ROM_MBC1(gui_mapBank);
    set_win_tiles(0, 0, gui_mapWidth, gui_mapHeight, gui_map);
    move_win(7, 136);
    upd_gui();
    SHOW_WIN;
}

void init_sprite() {
    SWITCH_ROM_MBC1(sprite_tilesetBank);
    set_sprite_data(0, 128, sprite_tileset);
    SPRITES_8x8;
    upd_character_sprite(&prin);
    SHOW_SPRITES;
}

void init_interrupts() {
    add_VBL(draw_gui);
    add_VBL(draw_bkg);
}

void logic() {
    logic_prin();
    logic_counter++;
}

void logic_prin() {
    logic_prin_control();
    logic_prin_move();
}

void logic_prin_control() {
    UBYTE j_up, j_right, j_down, j_left;
    UBYTE j_a, j_b;
    UBYTE cur_joypad;
    UBYTE prin_animation_no;
    UBYTE prin_animation_f;
    UBYTE prin_state;
    cur_joypad = joypad();
    j_up = cur_joypad & J_UP;
    j_right = cur_joypad & J_RIGHT;
    j_down = cur_joypad & J_DOWN;
    j_left = cur_joypad & J_LEFT;
    j_a = cur_joypad & J_A;
    j_b = cur_joypad & J_B;
    prin_animation_no = assign(prin.animation_no);
    prin_animation_f = 0;
    prin_state = prin.state;
    if (j_left) {
        prin.orientation = INV_ORIENTATION;
    }
    if (j_right) {
        prin.orientation = STD_ORIENTATION;
    }
    if (prin.state != PRIN_JUMP) {
        if (j_down) {
            prin_animation_no = 1;
            prin_state = PRIN_CROUCH;
        } else {
            prin_animation_no = 0;
            prin_state = PRIN_IDLE;
        }
        if (j_up && !(pre_joypad & J_UP)) {
            prin_animation_no = 2;
            prin_state = PRIN_JUMP;
        }
    }
    if (prin_hit_t == 0) {
        if (j_a && !(pre_joypad & J_A)) {
            prin_hit_t = PRIN_HIT_DUR;
        }
        if (j_b && !(pre_joypad & J_B)) {
            prin_hit_t = PRIN_HIT_DUR;
        }
    }
    if (!comp(prin.animation_no, prin_animation_no)) {
        prin_animation_f = ANIMATION_CHANGE;
        prin.animation_i = 0;
        prin.animation_t = 1;
    }
    prin.animation_no = prin_animation_no;
    prin.animation_f = prin_animation_f;
    prin.state = prin_state;
    pre_joypad = cur_joypad;
}

void logic_prin_move() {
    BYTE mov_y;
    if (prin.state != PRIN_JUMP) {
        return;
    }
    mov_y = prin_jump_table[prin_jump_i++];
    if (mov_y == 127) {
        prin_jump_i = 0;
        prin.animation_no = 0;
        prin.animation_f = ANIMATION_CHANGE;
        prin.animation_i = 0;
        prin.animation_t = 1;
        prin.state = PRIN_IDLE;
        return;
    }
    prin.y += mov_y;
}

void upd() {
    upd_characters();
    upd_bkg();
    upd_gui();
    gui_points = prin.animation_no;
}

void upd_characters() {
    sprite_no_i = 0;
    upd_prin();
    while (sprite_no_i < MAX_SP - 1) {
        set_sprite_tile(sprite_no_i++, 0);
    }
}

void upd_prin() {
    upd_character_animation(&prin);
    upd_prin_hit();
    upd_character_sprite(&prin);    
}

void upd_prin_hit() {
    UBYTE *animation_info;
    if (prin_hit_t == 0) {
        return;
    }
    if (prin.state == PRIN_IDLE) {
        prin.frame = 4;
    } else if (prin.state == PRIN_CROUCH) {
        prin.frame = 5;
    } else if (prin.state == PRIN_JUMP) {
        prin.frame = 6;
    }
    prin_hit_t--;
    if (prin_hit_t == 0) {
        animation_info = &animation_list;
        animation_info += animation_table[prin.animation_no];
        animation_info += prin.animation_i - 2;
        prin.frame = *animation_info;
        gui_points = prin.frame;
    }
}

void upd_character(Character *character) {
    upd_character_animation(character);
    upd_character_sprite(character);    
}

void upd_character_animation(Character *character) {
    UBYTE *animation_info;
    UBYTE *animation_start;
    if (character->animation_t-- != 0 && character->animation_f != ANIMATION_CHANGE) {
        return;
    }    
    animation_info = &animation_list;
    animation_info += animation_table[character->animation_no];
    animation_start = animation_info;
    animation_info += character->animation_i;
    if (*animation_info == 255) {
        character->animation_i = 0;
        animation_info = animation_start;
    }
    character->frame = *animation_info++;
    character->animation_t = *animation_info;
    character->animation_i += 2;
}

void upd_character_sprite(Character *character) {
    UBYTE *frame_info;
    UBYTE f_orientation;
    frame_info = &frame_list;
    frame_info += frame_table[character->frame];
    if (character->orientation == STD_ORIENTATION) {
        f_orientation = 0;
    } else {
        f_orientation = 1;
    }
    while (*frame_info != 127) {
        set_sprite_tile(sprite_no_i, *frame_info++);
        move_sprite(sprite_no_i, toneg8(*frame_info++, f_orientation) + character->x, *frame_info++ + character->y);
        set_sprite_prop(sprite_no_i, character->orientation);
        sprite_no_i++;
    }
}

void upd_bkg() {

}

void upd_gui() {
    upd_gui_points();
    upd_gui_hearths();
}

void upd_gui_points() {
    UWORD div_out;
    UBYTE div_res, div_rem;
    UBYTE gui_points_aux;
    BYTE i;
    gui_points_aux = gui_points;
    for(i = 2; i != -1; i--) {
        div_out = div8(gui_points_aux, 10);
        div_res = div_out;
        div_rem = div_out >> 8;
        gui_points_tiles[i] = DIGIT0_TILE + div_rem;
        gui_points_aux = div_res;
    }
}

void upd_gui_hearths() {
    BYTE prin_health_aux;
    BYTE i;
    prin_health_aux = prin_health;
    for(i = 0; i != 4; i++) {
        if (prin_health_aux > 1) {
           gui_hearths_tiles[i] = FULLHEARTH_TILE; 
        } else if (prin_health_aux != 1) {
           gui_hearths_tiles[i] = EMPTYHEARTH_TILE;
        } else {
           gui_hearths_tiles[i] = HALFHEARTH_TILE;
        }
        prin_health_aux -= 2;
    }
}

void draw_gui() {
    set_win_tiles(GUI_POINTS_X, 0, 3, 1, gui_points_tiles);
    set_win_tiles(GUI_HEARTHS_X, 0, 4, 1, gui_hearths_tiles); 
}

void draw_bkg() {
    move_bkg(bkg_x, bkg_y);
}