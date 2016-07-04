REM graphics
lcc -Wf-bo1 -c dev/data/graphics/title_tileset.c -o title_tileset.o
lcc -Wf-bo1 -c dev/data/graphics/story_tileset.c -o story_tileset.o
lcc -Wf-bo1 -c dev/data/graphics/sprite_tileset.c -o sprite_tileset.o
lcc -Wf-bo1 -c dev/data/graphics/sprite_bkg_tileset.c -o sprite_bkg_tileset.o
lcc -Wf-bo1 -c dev/data/graphics/bkg_tileset.c -o bkg_tileset.o 
REM music
lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o music.o dev/data/music/music.c
lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o gbt_player.o dev/data/music/gbt_player.s
lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o gbt_player_bank1.o dev/data/music/gbt_player_bank1.s
REM maps
lcc -Wf-bo2 -c dev/data/maps/title_map.c -o title_map.o
lcc -Wf-bo2 -c dev/data/maps/story_map.c -o story_map.o
lcc -Wf-bo2 -c dev/data/maps/level0_map.c -o level0_map.o
lcc -Wf-bo2 -c dev/data/maps/gui_map.c -o gui_map.o
REM logic
lcc -c dev/main.c -o main.o
REM build
lcc -Wl-yt1 -Wl-yo4 -Wl-ya0 main.o gbt_player.o gbt_player_bank1.o title_tileset.o story_tileset.o sprite_tileset.o sprite_bkg_tileset.o bkg_tileset.o title_map.o story_map.o level0_map.o gui_map.o dev/utils/math.s dev/utils/fixes.s dev/utils/extra.s -o rom/AkaruHime_TheRedPrincess.gb
REM clean
del *.o *.lst