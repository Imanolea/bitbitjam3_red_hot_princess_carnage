; Funciones matemáticas
; Imanolea 2016

.globl _div8
; @entrada:
;   E - Dividendo
;   D - Divisor
; @salida:
;   D - Resto
;   E - Resultado
_div8:
    lda hl, 2(sp)
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    ld  d,  a
    ld  l,  #8
    xor a
_div8_0:
    sla e
    rla
    cp  d
    jr  c,  _div8_1
    inc e
    sub d
_div8_1: 
    dec l
    jr  nz, _div8_0
    ld  d,  a
    ret
    
.globl  _mul8
; @entrada:
;   E - Factor 1
;   D - Factor 2
; @salida:
;   E - Resultado
_mul8:
    lda hl, 2(sp)
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    ld  d,  a
    ld  l,  #8
    xor a
_mul8_0:
    rlca
    rlc e
    jr  nc, _mul8_1
    add a,  d
_mul8_1:
    dec l
    jr  nz, _mul8_0
    ld  e,  a
    ret
    
.globl  _toneg8
; Convertimos un número de 8 bits a su equivalente negativo si el flag de entrada está subido
; @entrada:
;   A - Flag de entrada
; @salida:
;   E - Resultado
_toneg8:
    lda hl, 2(sp)
    ld  a,  (hli)
    ld  e,  a
    ld  a,  (hli)
    and a
    ret z
    ld  a,  e
    cpl
    inc a
    ld  e,  a
    ret