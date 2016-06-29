.globl  _leftshift
; Desplazamos hacia la izquierda un número de 16 bits pasado por parámetro
; @entrada:
;   B - Posiciones a desplazar
;   DE - Valor a desplazar
; @salida:
;   DE - Resultado
_leftshift:
    lda hl, 2(sp)
    push    bc
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    ld  d,  a
    ld  a,  (hli)
    ld  b,  a
    xor a
_leftshift0:
    rl  e
    rl  d
    dec b
    jr  nz, _leftshift0
    pop bc
    ret
    
.globl  _rightshift
; Desplazamos hacia la derecha un número de 16 bits pasado por parámetro
; @entrada:
;   B - Posiciones a desplazar
;   DE - Valor a desplazar
; @salida:
;   DE - Resultado
_rightshift:
    lda hl, 2(sp)
    push    bc
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    ld  d,  a
    ld  a,  (hli)
    ld  b,  a
    xor a
_rightshift0:
    rr  d
    rr  e
    dec b
    jr  nz, _rightshift0
    pop bc
    ret

.globl  _assign
; Devolvemos el valor de la variable pasada por parámetro
; @entrada:
;   DE - Valor a asignar
; @salida:
;   DE - Valor a asignar
_assign:
    lda hl, 2(sp)
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    ld  d,  a
    ret

.globl  _comp
; Comparamos los dos bytes pasados por parámetro y devolvemos un uno si son iguales y un cero si no
; @entrada:
;   D - Valor 1
;   A - Valor 2
; @salida
;   DE - Resultado de la comparación
_comp:
    lda hl, 2(sp)
    push    bc
    ld  a,  (hli)
    ld  c,  a
    ld  a,  (hli)
    ld  de,  #0x0001
    cp  c
    pop bc
    ret z
    ld  de,  #0x0000
    ret