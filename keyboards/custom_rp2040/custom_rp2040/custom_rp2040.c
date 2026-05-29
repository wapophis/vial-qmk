#include "custom_rp2040.h"
#include "analog.h"

#define JOY_AXIS_MAX 1023
#define JOY_CENTER   (JOY_AXIS_MAX / 2)

// ─── 8 direcciones (sectores de 45°) ─────────────────────────────────────────
typedef enum {
    DIR_NONE = 0,
    DIR_N,   // fila 0, col 5
    DIR_NE,  // fila 1, col 5
    DIR_E,   // fila 2, col 5
    DIR_SE,  // fila 3, col 5
    DIR_S,   // fila 4, col 5
    DIR_SW,  // fila 5, col 5
    DIR_W,   // fila 6, col 5
    DIR_NW,  // fila 7, col 5
    DIR_COUNT
} joy_dir_t;

// Fila en la matriz virtual para cada dirección
static const uint8_t dir_row[DIR_COUNT] = {
    [DIR_NONE] = 255,  // sin fila
    [DIR_N]    = 0,
    [DIR_NE]   = 1,
    [DIR_E]    = 2,
    [DIR_SE]   = 3,
    [DIR_S]    = 4,
    [DIR_SW]   = 5,
    [DIR_W]    = 6,
    [DIR_NW]   = 7,
};

// Mapa de 8 sectores de 45°
static const joy_dir_t map8[8] = {
    DIR_N,   //   0° (337.5–22.5°)
    DIR_NE,  //  45° (22.5–67.5°)
    DIR_E,   //  90° (67.5–112.5°)
    DIR_SE,  // 135° (112.5–157.5°)
    DIR_S,   // 180° (157.5–202.5°)
    DIR_SW,  // 225° (202.5–247.5°)
    DIR_W,   // 270° (247.5–292.5°)
    DIR_NW,  // 315° (292.5–337.5°)
};

static joy_dir_t read_direction(void) {
    int32_t raw_x = analogReadPin(JOY_X_PIN);
    int32_t raw_y = analogReadPin(JOY_Y_PIN);

    int32_t x =  raw_x - JOY_CENTER;
    int32_t y = -(raw_y - JOY_CENTER);  // quita el '-' si el eje Y está invertido

    int32_t dead = (JOY_CENTER * JOY_DEAD_ZONE) / 100;
    bool in_deadzone = (x > -dead && x < dead && y > -dead && y < dead);
    float raw_angle_deg = 0.0f;
    float angle_deg = 0.0f;
    int8_t sector = -1;
    joy_dir_t dir = DIR_NONE;

    if (!in_deadzone) {
        raw_angle_deg = atan2f((float)x, (float)y) * (180.0f / 3.14159265f);
        if (raw_angle_deg < 0.0f) {
            raw_angle_deg += 360.0f;
        }
        angle_deg = raw_angle_deg + (float)JOY_ANGLE_OFFSET_DEG;
        while (angle_deg >= 360.0f) angle_deg -= 360.0f;
        while (angle_deg < 0.0f) angle_deg += 360.0f;
        sector = (int8_t)(((angle_deg + 22.5f) / 45.0f)) % 8;
        dir = map8[(uint8_t)sector];
    }

    int16_t raw_angle_deg_i = (int16_t)raw_angle_deg;
    int16_t raw_angle_deg_centi = (int16_t)((raw_angle_deg - (float)raw_angle_deg_i) * 100.0f);
    if (raw_angle_deg_centi < 0) {
        raw_angle_deg_centi = -raw_angle_deg_centi;
    }
    int16_t angle_deg_i = (int16_t)angle_deg;
    int16_t angle_deg_centi = (int16_t)((angle_deg - (float)angle_deg_i) * 100.0f);
    if (angle_deg_centi < 0) {
        angle_deg_centi = -angle_deg_centi;
    }
    dprintf("joy dbg raw_x=%d raw_y=%d axis_max=%d center=%d x=%d y=%d dead_zone_pct=%d dead=%d in_dead=%d raw_angle=%d.%02d offset=%d angle=%d.%02d sector=%d dir=%d\n",
            (int)raw_x, (int)raw_y, JOY_AXIS_MAX, JOY_CENTER, (int)x, (int)y, JOY_DEAD_ZONE, (int)dead, in_deadzone ? 1 : 0,
            (int)raw_angle_deg_i, (int)raw_angle_deg_centi, JOY_ANGLE_OFFSET_DEG, (int)angle_deg_i, (int)angle_deg_centi, sector, dir);

    return dir;
}

// ─── Scan del joystick: activa/desactiva la fila virtual en col 5 ─────────────
void matrix_scan_user(void) {
    static joy_dir_t prev_dir = DIR_NONE;

    joy_dir_t dir = read_direction();
   // dprintf("joystick: dir=%d row=%d col=%d\n", dir, dir_row[dir], JOY_MATRIX_COL);

    if (dir != prev_dir) {
        dprintf("joystick: dir=%d row=%d col=%d\n", dir, dir_row[dir], JOY_MATRIX_COL);
        // Desactivar dirección anterior
        if (prev_dir != DIR_NONE) {
            matrix_clear_row_col(dir_row[prev_dir], JOY_MATRIX_COL);
        }
        // Activar nueva dirección
        if (dir != DIR_NONE) {
            matrix_set_row_col(dir_row[dir], JOY_MATRIX_COL);
        }
        prev_dir = dir;
    }
}

// ─── Helpers para manipular la matriz directamente ───────────────────────────
void matrix_set_row_col(uint8_t row, uint8_t col) {
    extern matrix_row_t matrix[MATRIX_ROWS];
    matrix[row] |= (1 << col);
}

void matrix_clear_row_col(uint8_t row, uint8_t col) {
    extern matrix_row_t matrix[MATRIX_ROWS];
    matrix[row] &= ~(1 << col);
}
