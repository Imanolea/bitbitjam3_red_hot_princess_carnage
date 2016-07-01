.globl  _set_sprite_a
; Dibujamos un personaje del juego en memoria de sprites con orientación regular
; @entrada:
;   1(UBYTE) - sprite_no_i
;   2(UWORD) - sprite_info
;   3(UBYTE) - y
;   4(UBYTE) - x
; @salida:
;   DE - Resultado
_set_sprite_a:
    lda hl, 2(sp)
    push    bc
    ld  a,  (hli)
    ld  c,  a
    xor a
    ld  b,  a
    rl  c
    rl  b
    rl  c
    rl  b
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    ld  d,  a
    push    bc
    ld  a,  (hli)
    ld  b,  a
    ld  a,  (hli)
    ld  c,  a 
    pop hl
    push    bc
    ld  bc, #.OAM
    add hl, bc
    pop bc
_set_sprite_a0:
    ld  a,  (de)
    inc de
    add a,  b
    ld  (hli),  a
    ld  a,  (de)
    inc de
    add a,  c
    ld  (hli),  a
    ld  a,  (de)
    inc de
    cp  #127
    jr  z,  _set_sprite_a1
    ld  (hli),  a
    xor a
    ld  (hli),  a
    jr  _set_sprite_a0
_set_sprite_a1:
    dec hl
    dec hl
    ld  de, #0x0200 ; - OAM_RAM
    add hl, de
    ld  d,  h
    ld  e,  l
    srl e
    srl e
    pop bc
    ret

.globl  _set_sprite_b
; Dibujamos un personaje del juego en memoria de sprites con orientación invertida
; @entrada:
;   1(UBYTE) - sprite_no_i
;   2(UWORD) - sprite_info
;   3(UBYTE) - y
;   4(UBYTE) - x
; @salida:
;   DE - Resultado
_set_sprite_b:
    lda hl, 2(sp)
    push    bc
    ld  a,  (hli)
    ld  c,  a
    xor a
    ld  b,  a
    rl  c
    rl  b
    rl  c
    rl  b
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    ld  d,  a
    push    bc
    ld  a,  (hli)
    ld  b,  a
    ld  a,  (hli)
    ld  c,  a 
    pop hl
    push    bc
    ld  bc, #.OAM
    add hl, bc
    pop bc
_set_sprite_b0:
    ld  a,  (de)
    inc de
    add a,  b
    ld  (hli),  a
    ld  a,  (de)
    cpl
    inc a
    inc de
    add a,  c
    ld  (hli),  a
    ld  a,  (de)
    inc de
    cp  #127
    jr  z,  _set_sprite_b1
    ld  (hli),  a
    ld  a,  #0x20
    ld  (hli),  a
    jr  _set_sprite_b0
_set_sprite_b1:
    dec hl
    dec hl
    ld  de, #0x0200 ; - OAM_RAM
    add hl, de
    ld  d,  h
    ld  e,  l
    srl e
    srl e
    pop bc
    ret