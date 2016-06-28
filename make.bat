REM graphics
lcc -Wf-bo1 -c dev/data/graphics/sprite_tileset.c -o sprite_tileset.o
lcc -Wf-bo1 -c dev/data/graphics/sprite_bkg_tileset.c -o sprite_bkg_tileset.o
lcc -Wf-bo1 -c dev/data/graphics/bkg_tileset.c -o bkg_tileset.o 
REM maps
lcc -Wf-bo2 -c dev/data/maps/level0_map.c -o level0_map.o
lcc -Wf-bo2 -c dev/data/maps/gui_map.c -o gui_map.o
REM logic
lcc -c dev/main.c -o main.o
REM build
lcc -Wl-yt1 -Wl-yo4 -Wl-ya0 main.o sprite_tileset.o sprite_bkg_tileset.o bkg_tileset.o level0_map.o gui_map.o dev/utils/math.s dev/utils/fixes.s -o rom/red_hot_princess_carnage.gb
REM clean
del *.o