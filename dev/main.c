// Akaru-Hime. Ninjas' Profecy (Red hot princess carnage) | BitBitJam 3
// Light Games 2016
#include <gb/gb.h>
#include <gb/hardware.h>
#include <rand.h>

// Ficheros de datos
// Gráficos
#include "data/graphics/title_tileset.h"
#include "data/graphics/story_tileset.h"
#include "data/graphics/sprite_tileset.h"
#include "data/graphics/sprite_bkg_tileset.h"
#include "data/graphics/bkg_tileset.h"
// Mapas
#include "data/maps/title_map.h"
#include "data/maps/story_map.h"
#include "data/maps/level0_map.h"
#include "data/maps/gui_map.h"
// Musica
//#include "_SFX.h" Fichero donde se incluyen los FX
#include "data/music/gbt_player.h"
//extern const unsigned char * song_Data[];

// GUI
#define GUI_POINTS_X     16U
#define HIGHSCORE_X      15U
#define HIGHSCORE_Y      14U
#define GUI_HEARTHS_X    1U
// Tiles
#define DIGIT0_TILE         196U
#define EMPTYHEARTH_TILE    193U
#define HALFHEARTH_TILE     194U
#define FULLHEARTH_TILE     195U
// Valores del sistema
#define MAX_SP              39U
#define STD_ORIENTATION     0U
#define INV_ORIENTATION     S_FLIPX
#define J_ANYKEY            63U
// Estados
// Princesa
#define PRIN_IDLE           1U
#define PRIN_CROUCH         2U
#define PRIN_JUMP           3U
#define PRIN_DYING          4U
#define PRIN_BESIEGED_PARALIZED_F   1U
#define PRIN_BESIEGED_RELEASED_F    2U
#define PRIN_BESIEGED_RECOVERY_TIME 32U
// Enemigos
#define EN_DISABLED         0U
#define EN_WALKING          1U
#define EN_ATTACKING        2U
#define EN_DYING            3U
// Proyectiles
#define PROJ_DISABLED       0U
#define PROJ_ENABLED        1U
// Valores del juego
#define GROUND_Y            98U
#define PRIN_HIT_DUR        8U
#define NUM_ENEMIES         2U
#define NUM_PROJECTILES     2U 
#define DAMAGE_FREQ         63U 
#define ENEMY_MOV_FREQ      7U
#define PROJ_MOV_FREQ       3U
#define PRIN_FLICK_TIME     32U
#define PRIN_FLICK_FREQ     3U
#define PRIN_LEFT_EN_X      74U
#define PRIN_RIGHT_EN_X     94U 
#define PRIN_LEFT_PROJ_X    78U
#define PRIN_RIGHT_PROJ_X   90U 
#define PRIN_X              84U
#define PRIN_RIGHT_HIT_X        120U
#define PRIN_RIGHT_SHORT_HIT_X  112U
#define PRIN_LEFT_HIT_X         48U
#define PRIN_LEFT_SHORT_HIT_X   56U
// Flags
#define ANIMATION_CHANGE    1U
// Constantes
const UBYTE en_spawn_pos[] = {0, STD_ORIENTATION, 
                              168, INV_ORIENTATION };
const UBYTE proj_spawn_pos[] = {  0,  96, STD_ORIENTATION, 0,
                                168,  96, INV_ORIENTATION, 0,
                                  0, 125, STD_ORIENTATION, 0,
                                168, 125, INV_ORIENTATION, 0,};

// Tablas de datos

// Graficas
BYTE frame_list[] = {
    0,0,0,  0,0,128, // 0 - Frame nulo
    0,0,1,  -8,0,2,  -8,-8,3,  -8,8,4,  0,-8,5,  0,8,6,  8,-8,7,  8,0,8,  8,8,9,  16,-5,10,  16,3,11,  24,-6,12,  24,2,13,  32,-8,14,  32,0,15,  0,0,128,   // 6 - Princesa en guardia
    6,0,1,  -2,0,2,  -2,-8,3,  -2,8,4,  6,-8,5,  6,8,6,  14,-8,7,  14,0,8,  14,8,9,  22,-5,26,  22,3,27,  30,-5,28,  30,3,29,  38,-8,30,  38,0,31,  0,0,128, // 54 - Princesa agachada
    0,0,1,  -8,0,2,  -8,-8,-3,  -8,8,4,  0,-8,16,  0,8,6,  8,-8,17,  8,0,18,  8,8,9,  16,-5,19,  16,3,20,  24,-8,21,  24,0,22,  32,-8,23,  32,0,24,  40,0,25,  0,0,128, // 102 - Princesa en el aire
    0,0,1,  -8,0,2,  -8,-8,3,  0,-8,5,  8,-8,7,  8,0,32,  8,8,33,  11,16,34,  11,24,35,  16,-8,63,  16,0,64,  16,8,65,  24,-6,12,  24,2,13,  32,-8,14,  32,0,15,  0,0,128,  // 153 - Princesa en guardia golpe
    6,0,1,  -2,0,2,  -2,-8,3,  6,-8,5,  14,-8,7,  14,0,32,  14,8,36,  14,16,37,  6,16,38,  6,8,39,  22,-5,26,  22,3,40,  22,11,41,  30,-5,28,  30,3,29,  38,-8,30,  38,0,31,  0,0,128, // 204 - Princesa agachada golpe
    0,0,1,  -8,0,2,  -8,-8,3,  0,-8,16,  8,-8,17,  8,0,42,  16,-5,43,  16,3,44,  24,-8,21,  24,0,22,  32,-8,23,  32,0,24,  40,0,25,  24,7,46,  24,15,47,  32,15,48,  0,0,128, // 258 - Princesa en el aire golpe
    0,0,50,  0,8,49,  8,0,52,  8,8,51,  16,0,54,  16,8,53,  24,0,58,  24,8,57,  32,0,60,  32,8,59,  0,0,128,  0,0,128,  // 309 - Enemigo andando 1
    0,0,50,  0,8,49,  8,0,79,  8,8,80,  16,0,56,  16,8,55,  24,0,67,  24,8,66,  32,0,69,  32,8,68,  0,0,128,  // 345 - Enemigo andando 2 
    0,0,50,  0,8,49,  8,0,52,  8,8,51,  16,0,71,  16,8,70,  24,0,73,  24,8,72,  32,0,76,  32,8,74,  0,0,128,  // 378 - Enemigo andando 3
    0,0,50,  0,8,49,  8,0,52,  8,8,77,  16,0,81,  16,8,78,  24,0,75,  24,8,72,  32,0,76,  32,8,74,  0,0,128,  // 411 - Enemigo atacando    
    0,-2,82,  0,6,83,  8,0,84,  8,8,85,  16,0,86,  16,8,87,  24,1,88,  24,9,89,  24,17,90,  32,8,91,  0,0,128,  // 444 - Enemigo muriendo 
};

UWORD frame_table[] = {
    0, // 0 - Frame nulo
    6, // 1 - Princesa en guardia
   54, // 2 - Princesa agachada
  102, // 3 - Princesa en el aire
  153, // 4 - Princesa en guardia golpe
  204, // 5 - Princesa agachada golpe
  258, // 6 - Princesa en el aire golpe
  309, // 7 - Enemigo andando 1
  345, // 8 - Enemigo andando 2
  378, // 9 - Enemigo andando 3
  411, // 10 - Enemigo atacando 
  444, // 11 - Enemigo muriendo
};

UBYTE animation_list[] = {
    0, 254, 255, // 0 - Animación nula
    1, 254, 255, // 3 - Princesa en guardia
    2, 254, 255, // 6 - Princesa agachada
    3,   3,   3,  35, 255, // 9 - Princesa saltando
    8,  11,   9,  11,   7,  11,   9,  11, 255, // 14 - Enemigo corriendo
   10, 254, 255, // 23 - Enemigo parado
   11, 254, 255, // 26 - Enemigo muriendo
};

UWORD animation_table[] = {
    0, // 0 - Animación nula
    3, // 1 - Princesa en guardia
    6, // 2 - Princesa agachada
    9, // 3 - Princesa saltando
   14, // 4 - Enemigo corriendo
   23, // 5 - Enemigo parado
   26, // 6 - Enemigo muriendo
};

UBYTE proj_animation_list[] = {
    61, 62, // Animación de shuriken
};

// Lógica
BYTE prin_jump_table[] = {
    0, -3, -3, -2, -7, -2, -2, -1, -2, -1, -2, -1, -1, -1, -1, -1, 0, -1, 0, -1, 0, 0, 0, 1, 0, 1, 0, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 3, 2, 8, 128,
};

BYTE en_dying_table[] = {
	-3, -3, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, -1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 128,
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

typedef struct {
    UBYTE null;
    UBYTE x;
    UBYTE y;
    UBYTE tile;
    UBYTE orientation;
    UBYTE animation_i;
    UBYTE animation_t;
    UBYTE state;
} Projectile;

Character prin;
Character enemies[NUM_ENEMIES];
Projectile shurikens[NUM_PROJECTILES];
UBYTE game_f;
UBYTE high_score;
UBYTE prin_health;
UBYTE prin_jump_i;
UBYTE prin_left_besieged_f;
UBYTE prin_right_besieged_f;
UBYTE prin_besieged_f;
UBYTE prin_besieged_t;
UBYTE prin_besieged_c;
UBYTE prin_flick_t;
UBYTE prin_hit_t;
UBYTE enemies_dying_i[NUM_ENEMIES];
UWORD sprite_no_i;
UBYTE pre_joypad;
UBYTE bkg_x;
UBYTE bkg_y;
UBYTE gui_points;
UBYTE logic_enemy_counter_c;
UBYTE logic_enemy_counter;
UBYTE logic_proj_counter_c;
UBYTE logic_proj_counter;
// Semillas
UBYTE logic_counter;
// Listas
UBYTE gui_points_tiles[3];
UBYTE gui_hearths_tiles[5];


UBYTE sfx_on = 0;
UBYTE twoNotes = 0;

// Declaración de funciones
void game();
void init_title();
void init_story();
void init_game();
void init_game_var();
void init_game_sprite();
void init_title_bkg();
void init_game_bkg();
void init_game_gui();
void init_game_interrupts();
void title_screen();
void story();
void init_music();
void logic();
void logic_game();
void logic_enemies();
void logic_enemy_attack(Character *character);
void logic_projectiles();
void logic_projectile_collision(Projectile *projectile);
void prin_damage();
void logic_prin();
void logic_prin_control();
void logic_prin_hit();
void logic_prin_move();
void logic_prin_fight();
void kill_enemy(Character *characer, UBYTE enemy_i);
void upd();
void upd_sprites();
void upd_characters();
void upd_prin();
void upd_prin_hit();
void upd_prin_sprite();
void upd_character(Character *character);
void upd_character_animation(Character *character);
void upd_character_sprite(Character *character);
void upd_projectiles();
void upd_gui();
void upd_gui_points();
void upd_score();
void upd_gui_hearths();
void draw_bkg();
void draw_gui();
void draw_score(UBYTE x, UBYTE y);


//Funciones para los FX
typedef enum{
	SWORD_SOUND, 	// Sword Sound
	KILL_SOUND,		// Matar enemigo Sound
	PUNCH_SOUND, 	// Golpe Princesa Sound
	GAMEOVER_SOUND,	// Game Over Sound
	START_SOUND, 	// Start Sound
	JUMP_SOUND		// Jump Sound
} fx;
void initFX();
UBYTE twoNotesSound(UBYTE sfx_two_notes_on);
void playSound(fx fxsound);
void playSword();
void playEnemyKilled();
void playPunch();
void playGameOver();
void playStart();
void playJump();

// Ensamblador
WORD div8(UBYTE numerator, UBYTE denominator);
WORD mul8(UBYTE factor_1, UBYTE factor_2);
WORD toneg8(UBYTE value, UBYTE flag);
WORD leftshift(WORD value, UBYTE num);
WORD rightshift(WORD value, UBYTE num);
WORD assign(UWORD value);
WORD comp(UBYTE value_1, UBYTE value_2);
WORD random(UBYTE seed_1, UBYTE seed_2, UBYTE seed_3);
WORD set_sprite_a(UBYTE sprite_no_i, UWORD sprite_info, UBYTE y, UBYTE x);
WORD set_sprite_b(UBYTE sprite_no_i, UWORD sprite_info, UBYTE y, UBYTE x);

void main() {
    high_score = 0;
    init_game_interrupts();
    game();
}

void game() {
    while (1) {
        title_screen();
        initrand(LY_REG);
        story();
        init_game();
        while (game_f) {
            logic();
            upd();
            wait_vbl_done();
            //gbt_update();
        }
    }
}

void init_title() { 
    disable_interrupts();
    DISPLAY_OFF;
    HIDE_SPRITES;
    initFX();
    //init_music();
    move_win(7, 144);
    init_title_bkg();
    upd_score();
    draw_score(HIGHSCORE_X, HIGHSCORE_Y - 2);
    if (high_score < gui_points) {
        high_score = assign(gui_points);
    }
    gui_points = assign(high_score);
    upd_score();
    draw_score(HIGHSCORE_X, HIGHSCORE_Y);
    //draw_highscore(HIGHSCORE_X, HIGHSCORE_Y);
    DISPLAY_ON;
    enable_interrupts();
}

void init_game() {
    disable_interrupts();
    DISPLAY_OFF;
    init_game_var();
    init_game_sprite();
    init_game_bkg();
    init_game_gui();
    DISPLAY_ON;
    enable_interrupts();
}

void init_game_var() {
    UBYTE i;
    game_f = 1;
    prin.x = 84;
    prin.y = GROUND_Y;
    prin.frame = 0;
    prin.orientation = STD_ORIENTATION;
    prin.animation_no = 0;
    prin.animation_f = 0;
    prin.animation_i = 0;
    prin.animation_t = 1;
    prin.state = PRIN_IDLE;
    for (i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].x = 0;
        enemies[i].y = 0;
        enemies[i].frame = 0;
        enemies[i].orientation = STD_ORIENTATION;
        enemies[i].animation_no = 0;
        enemies[i].animation_f = 0;
        enemies[i].animation_i = 0;
        enemies[i].animation_t = i + 1;
        enemies[i].state = EN_DISABLED;
        enemies_dying_i[i] = 0;
    }
    for (i = 0; i < NUM_PROJECTILES; i++) {
        shurikens[i].x = 0;
        shurikens[i].y = 0;
        shurikens[i].tile = 0;
        shurikens[i].orientation = STD_ORIENTATION;
        shurikens[i].animation_i = 0;
        shurikens[i].animation_t = 1;
        shurikens[i].state = PROJ_DISABLED;
    }
    prin_health = 10;
    prin_jump_i = 0;
    prin_left_besieged_f = 0;
    prin_right_besieged_f = 0;
    prin_besieged_f = 0;
    prin_besieged_t = 0;
    prin_besieged_c = 0;
    prin_flick_t = 0;
    prin_hit_t = 0;
    sprite_no_i = 0;
    pre_joypad = joypad();
    bkg_x = 0;
    bkg_y = 0;
    gui_points = 0;
    logic_enemy_counter_c = 175;
    logic_enemy_counter = rand();
    logic_proj_counter_c = 255;
    logic_proj_counter = rand();
}

void init_title_bkg() {
    SWITCH_ROM_MBC1(title_tilesetBank);
    set_bkg_data(0, 256, title_tileset);
    SWITCH_ROM_MBC1(level0_mapBank);
    set_bkg_tiles(0, 0, 20, 18, title_map);
    move_bkg(0, 0);
    SHOW_BKG;
}

void init_story_bkg() {
    SWITCH_ROM_MBC1(story_tilesetBank);
    set_bkg_data(0, 256, story_tileset);
    SWITCH_ROM_MBC1(story_mapBank);
    set_bkg_tiles(0, 0, 32, 32, story_map);
    move_bkg(0, 0);
    SHOW_BKG;
}

void init_game_bkg() {
    SWITCH_ROM_MBC1(bkg_tilesetBank);
    set_bkg_data(0, 128, bkg_tileset);
    SWITCH_ROM_MBC1(level0_mapBank);
    set_bkg_tiles(0, 0, 32, 32, level0_map);
    move_bkg(bkg_x, bkg_y);
    SHOW_BKG;
}

void init_music() {
	//gbt_play(song_Data, 2, 7);
	//gbt_loop(0);
	//set_interrupts(VBL_IFLAG);
}

void init_game_gui() {
    SWITCH_ROM_MBC1(sprite_bkg_tilesetBank);
    set_win_data(128, 256, sprite_bkg_tileset);
    SWITCH_ROM_MBC1(gui_mapBank);
    set_win_tiles(0, 0, gui_mapWidth, gui_mapHeight, gui_map);
    move_win(7, 136);
    upd_gui();
    SHOW_WIN;
}

void init_game_sprite() {
    SWITCH_ROM_MBC1(sprite_tilesetBank);
    set_sprite_data(0, 128, sprite_tileset);
    SPRITES_8x8;
    upd_character_sprite(&prin);
    SHOW_SPRITES;
}

void init_game_interrupts() {
    disable_interrupts();
    DISPLAY_OFF;
    add_VBL(draw_gui);
    add_VBL(draw_bkg);
    DISPLAY_ON;
    enable_interrupts();
}

void title_screen() {
    UBYTE j_start;
    UBYTE cur_joypad;
    init_title();
    while (1) {
        cur_joypad = joypad();
        j_start = cur_joypad & J_START;
        if (j_start && !(pre_joypad & J_START)) {
            break;
        }
        pre_joypad = cur_joypad;
        wait_vbl_done();
    }
    playStart();
}

void story() {
    UBYTE j_start;
    UBYTE cur_joypad;
    UBYTE scroll_t;
    UBYTE bkg_y;
    init_story();
    scroll_t = 0;
    bkg_y = 248;
    while (1) {
        cur_joypad = joypad();
        j_start = cur_joypad & J_START;
        if (j_start && !(pre_joypad & J_START)) {
            break;
        }
        pre_joypad = cur_joypad;
        if ((scroll_t & 7) == 0) {
            if (bkg_y != 128) {
                bkg_y++;
            }
        }
        wait_vbl_done();
        move_bkg(0, bkg_y);
        scroll_t++;
    }
    playStart();
}

void init_story() {
    disable_interrupts();
    DISPLAY_OFF;
    HIDE_SPRITES;
    init_story_bkg();
    DISPLAY_ON;
    enable_interrupts();  
}

void logic() {
    logic_game();
    logic_enemies();
    logic_projectiles();
    logic_prin();
    logic_counter++;
}

void logic_game() {
    UBYTE i;
    UBYTE *en_spawn_info;
    UBYTE *proj_spawn_info;
    if (logic_enemy_counter >= logic_enemy_counter_c) {
        logic_enemy_counter = 0;
        for (i = 0; i != NUM_ENEMIES; i++) {
            if (enemies[i].state == EN_DISABLED) {
                en_spawn_info = &en_spawn_pos;
                en_spawn_info += leftshift((rand() & 1), 1);
                enemies[i].x = *en_spawn_info++;
                enemies[i].y = GROUND_Y - 1;
                enemies[i].orientation = *en_spawn_info;
                enemies[i].animation_no = 4;
                enemies[i].animation_f = ANIMATION_CHANGE;
                enemies[i].animation_i = 0;
                enemies[i].animation_t = 1;
                enemies[i].state = EN_WALKING;
                break;
            }
        }
    }
    if (logic_proj_counter >= logic_proj_counter_c) {
        logic_proj_counter = 0;
        for (i = 0; i != NUM_PROJECTILES; i++) {
            if (shurikens[i].state == EN_DISABLED) {
                proj_spawn_info = &proj_spawn_pos;
                proj_spawn_info += leftshift((rand() & 3), 2);
                shurikens[i].x = *proj_spawn_info++;
                shurikens[i].y = *proj_spawn_info++;
                shurikens[i].orientation = *proj_spawn_info;
                shurikens[i].state = PROJ_ENABLED;
                break;
            }
        }
    }
    logic_enemy_counter++;
    logic_proj_counter++;
    if (logic_counter == 255) {
        if (logic_proj_counter_c > 55) {
            logic_proj_counter_c -= 8;
        }
        if (logic_enemy_counter_c > 31) {
            logic_enemy_counter_c -= 8;
        }
    }
}

void logic_enemies() {
    UBYTE i;
    UBYTE en_dying_pos;
    for (i = 0; i != NUM_ENEMIES; i++) {
        if (enemies[i].state == EN_WALKING) {
            if (enemies[i].orientation == STD_ORIENTATION) {
                enemies[i].x++;
                if (!(logic_counter & ENEMY_MOV_FREQ)) {
                    enemies[i].x++;
                }
                if (enemies[i].x > PRIN_LEFT_EN_X) {
                    enemies[i].x = PRIN_LEFT_EN_X;
                    if (prin_left_besieged_f) {
                        enemies[i].x -= 8;
                    }
                    prin_left_besieged_f = 1;
                    logic_enemy_attack(&enemies[i]);
                }
            } else {
                enemies[i].x--;
                if (!(logic_counter & ENEMY_MOV_FREQ)) {
                    enemies[i].x--;
                }
                if (enemies[i].x < PRIN_RIGHT_EN_X) {
                    enemies[i].x = PRIN_RIGHT_EN_X;
                    if (prin_right_besieged_f) {
                        enemies[i].x += 8;
                    }
                    prin_right_besieged_f = 1;
                    logic_enemy_attack(&enemies[i]);
                }
            }
        }
        if (enemies[i].state == EN_DYING) {
            en_dying_pos = en_dying_table[enemies_dying_i[i]];
            if (en_dying_pos == 128) {
                enemies[i].state = EN_DISABLED;
                return;
            }
            enemies[i].y += en_dying_pos;
            if (enemies[i].orientation == STD_ORIENTATION) {
                enemies[i].x--;
            } else {
                enemies[i].x++;
            }
            enemies_dying_i[i]++;
        }
    }
}

void logic_enemy_attack(Character *character) {
    character->animation_no = 5;
    character->animation_f = ANIMATION_CHANGE;
    character->animation_i = 0;
    character->animation_t = 1;
    character->state = EN_ATTACKING;
    if (!prin_besieged_f) {
        if (prin.state != PRIN_JUMP) {
            prin.animation_no = 1;
            prin.animation_f = ANIMATION_CHANGE;
            prin.animation_i = 0;
            prin.animation_t = 1;
        }
        prin_besieged_f = 1;
        prin_besieged_c = logic_counter & DAMAGE_FREQ;
    }
}

void logic_projectiles() {
UBYTE i;
    for (i = 0; i < NUM_PROJECTILES; i++) {
        if (shurikens[i].animation_t-- == 0) {
            shurikens[i].animation_t = 4;
            shurikens[i].tile = proj_animation_list[shurikens[i].animation_i];
            shurikens[i].animation_i = (shurikens[i].animation_i + 1) & 1;
        }
        if (shurikens[i].state == PROJ_ENABLED) {
            if (shurikens[i].orientation == STD_ORIENTATION) {
                shurikens[i].x += 2;
                if (!(logic_counter & PROJ_MOV_FREQ)) {
                    shurikens[i].x--;
                }
            } else {
                shurikens[i].x -= 2;
                if (!(logic_counter & PROJ_MOV_FREQ)) {
                    shurikens[i].x++;
                }
            }
            if (shurikens[i].x > PRIN_LEFT_PROJ_X && shurikens[i].x < PRIN_RIGHT_PROJ_X) {
                    logic_projectile_collision(&shurikens[i]);
            }
            if (shurikens[i].x > 168) {
                shurikens[i].state = PROJ_DISABLED;
            }
        }
    }
}

void logic_projectile_collision(Projectile *projectile) {
    if (projectile->y == 96) {
        if (prin.state == PRIN_CROUCH) {
            return;
        }
    } else {
        if (prin.state == PRIN_JUMP) {
            return;
        }
    }
    prin_damage();
    projectile->state = PROJ_DISABLED;
    projectile->x = 168;
}

void prin_damage() {
    if (prin_flick_t) {
        return;
    }
    prin_health--;
    prin_flick_t = PRIN_FLICK_TIME;
    if (!prin_health) {
        playGameOver();
        game_f = 0;
        return;
    }
    playPunch();
}

void logic_prin() {
    logic_prin_control();
    logic_prin_move();
    logic_prin_fight();
}

void logic_prin_control() {
    UBYTE j_up, j_right, j_down, j_left;
    UBYTE j_a, j_b;
    UBYTE cur_joypad;
    UBYTE prin_animation_no;
    UBYTE prin_animation_f;
    UBYTE prin_state;
    if (prin_besieged_f) {
        return;
    }
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
    if (prin_hit_t == 0) {
        if (j_left) {
            prin.orientation = INV_ORIENTATION;
        }
        if (j_right) {
            prin.orientation = STD_ORIENTATION;
        }
        if (prin.state != PRIN_JUMP) {
            if (j_down) {
                prin_animation_no = 2;
                prin_state = PRIN_CROUCH;
            } else {
                prin_animation_no = 1;
                prin_state = PRIN_IDLE;
            }
            if (j_a && !(pre_joypad & J_A)) {
                prin_animation_no = 3;
                prin_state = PRIN_JUMP;
            }
        }
        if (j_b && !(pre_joypad & J_B)) {
            logic_prin_hit();
            playSword();
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
	if(!j_up && !j_down && !j_right && !j_left && sfx_on == 1){
		sfx_on = 0;
	}
}

void logic_prin_hit() {
    UBYTE i;
    prin_hit_t = PRIN_HIT_DUR;
    for (i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].state == EN_WALKING) {
            if (prin.orientation == STD_ORIENTATION) {
                if (prin.state == PRIN_IDLE) {
                    if (enemies[i].x > PRIN_X && enemies[i].x < PRIN_RIGHT_HIT_X) {
                        kill_enemy(&enemies[i], i);
                    }
                } else  {
                    if (enemies[i].x > PRIN_X && enemies[i].x < PRIN_RIGHT_SHORT_HIT_X) {
                        kill_enemy(&enemies[i], i);
                    }
                }
            } else {
                if (prin.state == PRIN_IDLE) {
                    if (enemies[i].x < PRIN_X && enemies[i].x > PRIN_LEFT_HIT_X) {
                        kill_enemy(&enemies[i], i);
                    }
                } else  {
                    if (enemies[i].x < PRIN_X && enemies[i].x > PRIN_LEFT_SHORT_HIT_X) {
                        kill_enemy(&enemies[i], i);
                    }
                }
            }
        }
    }
}

void logic_prin_move() {
    BYTE mov_y;
    if (prin.state != PRIN_JUMP) {
        return;
    }
    mov_y = prin_jump_table[prin_jump_i++];
    if (mov_y == 128) {
        prin_jump_i = 0;
        prin.animation_no = 1;
        prin.animation_f = ANIMATION_CHANGE;
        prin.animation_i = 0;
        prin.animation_t = 1;
        prin.state = PRIN_IDLE;
        return;
    }
    prin.y += mov_y;
}

void logic_prin_fight() {
    UBYTE i;
    UBYTE cur_joypad;
    cur_joypad = joypad();
    if (prin_besieged_f) {
        if (comp((logic_counter & DAMAGE_FREQ), prin_besieged_c)) {
            prin_damage();
        }
        if (prin_besieged_t) {
            prin_besieged_t--;
            if (!prin_besieged_t) {
                prin_besieged_f = PRIN_BESIEGED_RELEASED_F;
            }
            return;
        }
        if ((cur_joypad & J_ANYKEY) && (cur_joypad != pre_joypad)) {
            if (prin_besieged_f == PRIN_BESIEGED_PARALIZED_F) {
                prin_besieged_t = PRIN_BESIEGED_RECOVERY_TIME;
                prin.orientation = prin.orientation ^ INV_ORIENTATION;
            } else {
                for (i = 0; i != NUM_ENEMIES; i++) {
                    if (enemies[i].state == EN_ATTACKING) {
                        kill_enemy(&enemies[i], i);
                    }
                }
                prin_besieged_f = 0;
                prin_left_besieged_f = 0;
                prin_right_besieged_f = 0;
                pre_joypad = 0;
                return;
            }
        }
        pre_joypad = cur_joypad;
    }
}

void kill_enemy(Character *character, UBYTE enemy_i) {
    character->state = EN_DYING;
    character->animation_no = 6;
    character->animation_f = ANIMATION_CHANGE;
    character->animation_i = 0;
    character->animation_t = 1;
    enemies_dying_i[enemy_i] = 0;
    gui_points++;
    playEnemyKilled();
}

void upd() {
    upd_sprites();
    upd_gui();
}

void upd_sprites() {
    UBYTE i;
    sprite_no_i = 0;
    upd_projectiles();
    upd_characters();
    for (i = sprite_no_i; i < MAX_SP; i++) {
        set_sprite_tile(i, 0);
    }
}

void upd_characters() {
    UBYTE i;
    upd_prin();
    for (i = 0; i != NUM_ENEMIES; i++) {
        upd_character(&enemies[i]);
    }
}

void upd_prin() {
    upd_character_animation(&prin);
    upd_prin_hit();
    upd_prin_sprite();
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
    }
}

void upd_prin_sprite() {
    UBYTE prin_frame;
    prin_frame = prin.frame;
    if (prin_flick_t) {
        prin_flick_t--;
        if (!(prin_flick_t & PRIN_FLICK_FREQ)) {
            prin.frame = 0;
        }
    }
    upd_character_sprite(&prin);
    prin.frame = prin_frame;
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
    character->animation_f = 0;
}

void upd_character_sprite(Character *character) {
    UBYTE *frame_info;
    frame_info = &frame_list;
    frame_info += frame_table[character->frame];
    if (character->orientation == STD_ORIENTATION) {
        sprite_no_i = set_sprite_a(sprite_no_i, frame_info, character->y, character->x);
    } else {
        sprite_no_i = set_sprite_b(sprite_no_i, frame_info, character->y, character->x);
    }
//    Not fast enough
//    while (*frame_info != 128) {
//        set_sprite_tile(sprite_no_i, *frame_info++);
//        move_sprite(sprite_no_i, toneg8(*frame_info++, f_orientation) + character->x, *frame_info++ + character->y);
//        set_sprite_prop(sprite_no_i, character->orientation);
//        sprite_no_i++;
//    }
}

void upd_projectiles() {
    UBYTE i;
    for (i = 0; i < NUM_PROJECTILES; i++) {
        set_sprite_tile(sprite_no_i, shurikens[i].tile);
        move_sprite(sprite_no_i, shurikens[i].x, shurikens[i].y);
        set_sprite_prop(sprite_no_i, shurikens[i].orientation);
        sprite_no_i++;
    }
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

void upd_score() {
    UWORD div_out;
    UBYTE div_res, div_rem;
    UBYTE gui_points_aux;
    BYTE i;
    gui_points_aux = gui_points;
    for(i = 2; i != -1; i--) {
        div_out = div8(gui_points_aux, 10);
        div_res = div_out;
        div_rem = div_out >> 8;
        gui_points_tiles[i] = 160 + div_rem;
        gui_points_aux = div_res;
    }
}

void upd_gui_hearths() {
    BYTE prin_health_aux;
    BYTE i;
    prin_health_aux = prin_health;
    for(i = 0; i != 5; i++) {
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
    set_win_tiles(GUI_HEARTHS_X, 0, 5, 1, gui_hearths_tiles); 
}

void draw_score(UBYTE x, UBYTE y) {
    set_bkg_tiles(x, y, 3, 1, gui_points_tiles);
}

void draw_bkg() {
    move_bkg(bkg_x, bkg_y);
}

// FX functions
void initFX(){
	NR52_REG = 0xF8U;
	NR51_REG = 0x00U;
	NR50_REG = 0x77U;
}

void playSound(fx fxsound){
	if(!sfx_on){
		sfx_on = 1;
		switch(fxsound){
			case SWORD_SOUND: 	// Sword Sound
								playSword();
								break;
			case KILL_SOUND:	// Matar enemigo Sound
								playEnemyKilled();
								break;
			case PUNCH_SOUND: 	// Golpe Princesa Sound
								playPunch();
								break;
			case GAMEOVER_SOUND:// Game Over Sound
								playGameOver();
								break;
			case START_SOUND: 	// Start Sound
								playStart();
								break;
			case JUMP_SOUND: 	playJump();
								break;
			default: break;	
		}	
	}
}

// Sword Sound
void playSword(){
	NR10_REG = 0x34U;
	NR11_REG = 0x70U;
	NR12_REG = 0xF0U;
	//Freq = 1546
	NR13_REG = 0x0AU;
	NR14_REG = 0xC6U;
	NR51_REG |= 0x11;
}

// Matar enemigo Sound
void playEnemyKilled(){
	NR10_REG = 0x34U;
	NR11_REG = 0x70U;
	NR12_REG = 0xF0U;
	// Freq = 2000
	NR13_REG = 0xD7U;
	NR14_REG = 0xC0U;
	NR51_REG |= 0x11;
}

// Golpe Princesa Sound
void playPunch(){
	NR10_REG = 0x34U;
	NR11_REG = 0x70U;
	NR12_REG = 0xF0U;
	// Freq = 500
	NR13_REG = 0x74U;
	NR14_REG = 0xC1U;
	NR51_REG |= 0x11;
}

// Game Over Sound
void playGameOver(){
	NR10_REG = 0x34U;
	NR11_REG = 0x70U;
	NR12_REG = 0xF0U;
	// Freq = 500
	NR13_REG = 0x74U;
	NR14_REG = 0xC1U;
	NR51_REG |= 0x11;
}

// Start Sound
void playStart(){
	NR10_REG = 0x34U;
	NR11_REG = 0x70U;
	NR12_REG = 0xF0U;
	// Freq = 500
	NR13_REG = 0x74U;
	NR14_REG = 0xC1U;
	NR51_REG |= 0x11;
}

// Jump Sound
void playJump(){
	// NR10_REG = 0x34U;
	// NR11_REG = 0x70U;
	// NR12_REG = 0xF0U;
	// Freq = 1546
	// NR13_REG = 0x0AU;
	// NR14_REG = 0xC6U;
	// NR51_REG |= 0x11;
	
	
	NR10_REG = 0x34U;
	NR11_REG = 0x70U;
	NR12_REG = 0xF0U;
	// Freq = 500
	NR13_REG = 0x74U;
	NR14_REG = 0xC1U;
	NR51_REG |= 0x11;
}

UBYTE twoNotesSound(UBYTE sfx_two_notes_on){
	if(NR52_REG & 0x02){
		return 0x01;
	}else if(sfx_two_notes_on){
		//Segunda nota
		NR21_REG = 0x80U;
		NR22_REG = 0x73U;
		NR23_REG = 0x9FU;
		NR24_REG = 0xC7U;
		NR51_REG |= 0x22;
		return 0x00;
	}else{
		// Primera nota
		NR21_REG = 0xAEU;
		NR22_REG = 0x68U;
		NR23_REG = 0xDBU;
		NR24_REG = 0xC7U;
		NR51_REG |= 0x22;
		return 0x01;
	}
}