#pragma once

#include "quantum.h"
#include <math.h>

enum custom_keycodes {
    JOY_ANG_INC = QK_KB_0,
    JOY_ANG_DEC = QK_KB_1,
    JOY_ANG_RST = QK_KB_2,
};

void matrix_scan_user(void);
void matrix_set_row_col(uint8_t row, uint8_t col);
void matrix_clear_row_col(uint8_t row, uint8_t col);

uint16_t joy_angle_offset_get(void);
void joy_angle_offset_set(uint16_t degrees);
void joy_angle_offset_add(int16_t delta);

// Layout: 26 teclas fisicas + 4 cardinales virtuales (col 5, filas 0/2/4/6)
