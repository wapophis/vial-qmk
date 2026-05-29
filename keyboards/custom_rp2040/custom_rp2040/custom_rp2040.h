#pragma once
#include "quantum.h"
#include <math.h>

void matrix_scan_user(void);
void matrix_set_row_col(uint8_t row, uint8_t col);
void matrix_clear_row_col(uint8_t row, uint8_t col);

// ─── Macro de layout: 26 teclas físicas + 8 virtuales del joystick ───────────
//
//        C0    C1    C2    C3    C4    C5(virtual)
//  R0  [k00] [k01] [k02] [k03] [k04] [JOY_N ]
//  R1  [k10] [k11] [k12] [k13] [k14] [JOY_NE]
//  R2  [k20] [k21] [k22] [k23] [ X ] [JOY_E ]
//  R3  [k30] [k31] [k32] [k33] [ X ] [JOY_SE]
//  R4  [k40] [k41] [k42] [k43] [ X ] [JOY_S ]
//  R5  [k50] [ X ] [ X ] [k53] [ X ] [JOY_SW]
//  R6  [ X ] [ X ] [ X ] [k63] [ X ] [JOY_W ]
//  R7  [ X ] [ X ] [ X ] [k73] [ X ] [JOY_NW]

/*#define LAYOUT_custom( \
    c0r0, c0r1, c0r2, c0r3, c0r4, c0r5,             \
    c1r0, c1r1, c1r2, c1r3, c1r4,                    \
    c2r0, c2r1, c2r2, c2r3, c2r4,                    \
    c3r0, c3r1, c3r2, c3r3, c3r4, c3r5, c3r6, c3r7, \
    c4r0, c4r1,                                       \
    j_n, j_ne, j_e, j_se, j_s, j_sw, j_w, j_nw      \
) { \
    { c0r0, c1r0, c2r0, c3r0, c4r0, j_n  }, \
    { c0r1, c1r1, c2r1, c3r1, c4r1, j_ne }, \
    { c0r2, c1r2, c2r2, c3r2, KC_NO, j_e }, \
    { c0r3, c1r3, c2r3, c3r3, KC_NO, j_se}, \
    { c0r4, c1r4, c2r4, c3r4, KC_NO, j_s }, \
    { c0r5, KC_NO, KC_NO, c3r5, KC_NO, j_sw}, \
    { KC_NO, KC_NO, KC_NO, c3r6, KC_NO, j_w }, \
    { KC_NO, KC_NO, KC_NO, c3r7, KC_NO, j_nw}  \
     
}
*/