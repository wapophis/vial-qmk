#include "custom_rp2040.h"
#include "action.h"
#include "action_layer.h"
#include "analog.h"
#include "eeconfig.h"
#include "keymap_common.h"

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

static uint8_t   joy_active_mask           = 0;
static uint16_t  joy_registered_keycodes[4] = {KC_NO, KC_NO, KC_NO, KC_NO};

static uint16_t joy_keycode_for(uint8_t cardinal_index) {
    const keypos_t key = {.row = cardinal_row[cardinal_index], .col = JOY_MATRIX_COL};
    return keymap_key_to_keycode(layer_switch_get_layer(key), key);
}

static void joy_release_direction(uint8_t cardinal_index) {
    const uint16_t keycode = joy_registered_keycodes[cardinal_index];
    if (keycode != KC_NO) {
        unregister_code16(keycode);
        joy_registered_keycodes[cardinal_index] = KC_NO;
    }
}

static void joy_press_direction(uint8_t cardinal_index) {
    const uint16_t keycode = joy_keycode_for(cardinal_index);
    if (keycode != KC_NO) {
        register_code16(keycode);
        joy_registered_keycodes[cardinal_index] = keycode;
    }
}

static void joy_sync_mask(uint8_t mask) {
    for (uint8_t i = 0; i < 4; i++) {
        const uint8_t bit     = (1 << i);
        const bool    active  = (mask & bit) != 0;
        const bool    was     = (joy_active_mask & bit) != 0;
        const uint16_t target = active ? joy_keycode_for(i) : KC_NO;

        if (active && was && target != joy_registered_keycodes[i]) {
            joy_release_direction(i);
            joy_press_direction(i);
        } else if (active && !was) {
            joy_press_direction(i);
        } else if (!active && was) {
            joy_release_direction(i);
        }
    }

    joy_active_mask = mask;
}

void matrix_scan_user(void) {
    const joy_dir_t dir  = read_direction();
    const uint8_t   mask = dir_mask[dir];

    joy_sync_mask(mask);
}
