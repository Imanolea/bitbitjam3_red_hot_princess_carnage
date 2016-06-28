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

UBYTE hero_health;
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
void upd();
void upd_characters();
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
    hero_health = 8;
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
    SPRITES_8x16;
    SHOW_SPRITES;
}

void init_interrupts() {
    add_VBL(draw_gui);
    add_VBL(draw_bkg);
}

void logic() {
    pre_joypad = joypad();
    logic_counter++;
}

void upd() {
    upd_bkg();
    upd_gui();
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
    BYTE hero_health_aux;
    BYTE i;
    hero_health_aux = hero_health;
    for(i = 0; i != 4; i++) {
        if (hero_health_aux > 1) {
           gui_hearths_tiles[i] = FULLHEARTH_TILE; 
        } else if (hero_health_aux != 1) {
           gui_hearths_tiles[i] = EMPTYHEARTH_TILE;
        } else {
           gui_hearths_tiles[i] = HALFHEARTH_TILE;
        }
        hero_health_aux -= 2;
    }
}

void draw_gui() {
    set_win_tiles(GUI_POINTS_x, 0, 3, 1, gui_points_tiles);
    set_win_tiles(GUI_HEARTHS_x, 0, 4, 1, gui_hearths_tiles); 
}

void draw_bkg() {
    move_bkg(bkg_x, bkg_y);
}