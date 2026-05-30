#include "custom_rp2040.h"
#include "analog.h"
#include "eeconfig.h"

#define JOY_AXIS_MAX 1023
#define JOY_CENTER   (JOY_AXIS_MAX / 2)

#define CUSTOM_RP2040_EEPROM_MAGIC 0xC204

typedef struct {
    uint16_t magic;
    uint16_t joy_angle_offset_deg;
} custom_rp2040_eeprom_t;

_Static_assert(sizeof(custom_rp2040_eeprom_t) == EECONFIG_KB_DATA_SIZE, "EECONFIG_KB_DATA_SIZE mismatch");

static uint16_t joy_angle_offset_runtime = JOY_ANGLE_OFFSET_DEG;

typedef enum {
    DIR_NONE = 0,
    DIR_N,
    DIR_NE,
    DIR_E,
    DIR_SE,
    DIR_S,
    DIR_SW,
    DIR_W,
    DIR_NW,
    DIR_COUNT
} joy_dir_t;

#define JOY_BIT_N (1 << 0)
#define JOY_BIT_E (1 << 1)
#define JOY_BIT_S (1 << 2)
#define JOY_BIT_W (1 << 3)

static const uint8_t dir_mask[DIR_COUNT] = {
    [DIR_NONE] = 0,
    [DIR_N]    = JOY_BIT_N,
    [DIR_NE]   = JOY_BIT_N | JOY_BIT_E,
    [DIR_E]    = JOY_BIT_E,
    [DIR_SE]   = JOY_BIT_S | JOY_BIT_E,
    [DIR_S]    = JOY_BIT_S,
    [DIR_SW]   = JOY_BIT_S | JOY_BIT_W,
    [DIR_W]    = JOY_BIT_W,
    [DIR_NW]   = JOY_BIT_N | JOY_BIT_W,
};

static const uint8_t cardinal_row[4] = {0, 2, 4, 6};

static const joy_dir_t map8[8] = {
    DIR_N, DIR_NE, DIR_E, DIR_SE, DIR_S, DIR_SW, DIR_W, DIR_NW,
};

static uint16_t normalize_angle(int32_t degrees) {
    degrees %= 360;
    if (degrees < 0) {
        degrees += 360;
    }
    return (uint16_t)degrees;
}

static void joy_config_save(uint16_t degrees) {
    custom_rp2040_eeprom_t cfg = {
        .magic                = CUSTOM_RP2040_EEPROM_MAGIC,
        .joy_angle_offset_deg = degrees,
    };
    eeconfig_update_kb_datablock(&cfg, 0, sizeof(cfg));
}

static void joy_config_load(void) {
    custom_rp2040_eeprom_t cfg = {0};
    eeconfig_read_kb_datablock(&cfg, 0, sizeof(cfg));

    if (cfg.magic == CUSTOM_RP2040_EEPROM_MAGIC && cfg.joy_angle_offset_deg < 360) {
        joy_angle_offset_runtime = cfg.joy_angle_offset_deg;
    } else {
        joy_angle_offset_runtime = JOY_ANGLE_OFFSET_DEG;
    }
}

uint16_t joy_angle_offset_get(void) {
    return joy_angle_offset_runtime;
}

void joy_angle_offset_set(uint16_t degrees) {
    joy_angle_offset_runtime = normalize_angle(degrees);
    joy_config_save(joy_angle_offset_runtime);
}

void joy_angle_offset_add(int16_t delta) {
    joy_angle_offset_set(normalize_angle((int32_t)joy_angle_offset_runtime + delta));
}

void eeconfig_init_kb(void) {
    joy_angle_offset_set(JOY_ANGLE_OFFSET_DEG);
}

void keyboard_post_init_user(void) {
    joy_config_load();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case JOY_ANG_INC:
                joy_angle_offset_add(JOY_ANGLE_OFFSET_STEP);
                return false;
            case JOY_ANG_DEC:
                joy_angle_offset_add(-JOY_ANGLE_OFFSET_STEP);
                return false;
            case JOY_ANG_RST:
                joy_angle_offset_set(JOY_ANGLE_OFFSET_DEG);
                return false;
        }
    }
    return true;
}

static joy_dir_t read_direction(void) {
    int32_t raw_x = analogReadPin(JOY_X_PIN);
    int32_t raw_y = analogReadPin(JOY_Y_PIN);

    int32_t x = raw_x - JOY_CENTER;
    int32_t y = -(raw_y - JOY_CENTER);

    int32_t dead = (JOY_CENTER * JOY_DEAD_ZONE) / 100;
    bool in_deadzone = (x > -dead && x < dead && y > -dead && y < dead);
    joy_dir_t dir = DIR_NONE;

    if (!in_deadzone) {
        float raw_angle_deg = atan2f((float)x, (float)y) * (180.0f / 3.14159265f);
        if (raw_angle_deg < 0.0f) {
            raw_angle_deg += 360.0f;
        }
        float angle_deg = raw_angle_deg + (float)joy_angle_offset_runtime;
        while (angle_deg >= 360.0f) angle_deg -= 360.0f;
        while (angle_deg < 0.0f) angle_deg += 360.0f;
        int8_t sector = (int8_t)(((angle_deg + 22.5f) / 45.0f)) % 8;
        dir = map8[(uint8_t)sector];
    }

    return dir;
}

static void apply_cardinal_mask(uint8_t prev_mask, uint8_t new_mask) {
    for (uint8_t i = 0; i < 4; i++) {
        const uint8_t bit = (1 << i);
        if ((prev_mask & bit) && !(new_mask & bit)) {
            matrix_clear_row_col(cardinal_row[i], JOY_MATRIX_COL);
        }
        if (!(prev_mask & bit) && (new_mask & bit)) {
            matrix_set_row_col(cardinal_row[i], JOY_MATRIX_COL);
        }
    }
}

void matrix_scan_user(void) {
    static uint8_t prev_mask = 0;

    const joy_dir_t dir  = read_direction();
    const uint8_t   mask = dir_mask[dir];

    if (mask != prev_mask) {
        apply_cardinal_mask(prev_mask, mask);
        prev_mask = mask;
    }
}

void matrix_set_row_col(uint8_t row, uint8_t col) {
    extern matrix_row_t matrix[MATRIX_ROWS];
    matrix[row] |= (1 << col);
}

void matrix_clear_row_col(uint8_t row, uint8_t col) {
    extern matrix_row_t matrix[MATRIX_ROWS];
    matrix[row] &= ~(1 << col);
}
