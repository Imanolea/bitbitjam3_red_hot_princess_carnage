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
#define GUI_POINTS_x     16U
#define GUI_HEARTHS_x    1U
// Tiles
#define DIGIT0_TILE         196U
#define EMPTYHEARTH_TILE    193U
#define HALFHEARTH_TILE     194U
#define FULLHEARTH_TILE     195U
// Valores del sistema
#define STD_ORIENTATION     0U
#define INV_ORIENTATION     S_FLIPX
// Flags
#define ANIMATION_CHANGE    1U

// Tablas de datos
BYTE frame_list[] = {
    1,0,0,  2,8,6,  3,8,-2,  4,16,5,  5,16,-3,  6,24,5,  7,24,-3,  8,24,-11,  9,32,6, 10,32,-2,  11,32,-10,  255  // 0 - Princesa en guardia
};

UWORD frame_table[] = {
    0, // 0 - Princesa en guardia
};

UBYTE animation_list[] = {
    0, 254, 255, // 0 - Princesa en guardia
};

UWORD animation_table[] = {
    0, // 0 - Princesa en guardia
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
} Character;

Character prin;
UBYTE prin_health;
UBYTE i_sprite_no;
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
void upd();
void upd_characters();
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
    prin.y = 88;
    prin.frame = 0;
    prin.orientation = STD_ORIENTATION;
    prin.animation_no = 0;
    prin.animation_f = 0;
    prin.animation_i = 0;
    prin.animation_t = 1;
    prin_health = 8;
    i_sprite_no = 0;
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
    pre_joypad = joypad();
    logic_counter++;
}

void logic_prin() {
    UBYTE j_up, j_right, j_down, j_left;
    UBYTE cur_joypad;
    cur_joypad = joypad();
    j_up = cur_joypad & J_UP;
    j_right = cur_joypad & J_RIGHT;
    j_down = cur_joypad & J_DOWN;
    j_left = cur_joypad & J_LEFT;
    if (j_left) {
        prin.orientation = INV_ORIENTATION;
    }
    if (j_right) {
        prin.orientation = STD_ORIENTATION;
    }
}

void upd() {
    upd_characters();
    upd_bkg();
    upd_gui();
}

void upd_characters() {
    i_sprite_no = 0;
    upd_character(&prin);
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
    while (*frame_info != 255) {
        set_sprite_tile(i_sprite_no, *frame_info++);
        move_sprite(i_sprite_no, toneg8(*frame_info++, f_orientation) + character->x, *frame_info++ + character->y);
        set_sprite_prop(i_sprite_no, character->orientation);
        i_sprite_no++;
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
    set_win_tiles(GUI_POINTS_x, 0, 3, 1, gui_points_tiles);
    set_win_tiles(GUI_HEARTHS_x, 0, 4, 1, gui_hearths_tiles); 
}

void draw_bkg() {
    move_bkg(bkg_x, bkg_y);
}