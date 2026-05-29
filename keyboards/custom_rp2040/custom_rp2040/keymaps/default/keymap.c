#include QMK_KEYBOARD_H

enum layers { _BASE = 0, _FN, _MOUSE, _GAME };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT_custom(
        // Col 0 (6 teclas)
        KC_Q, KC_A, KC_Z, KC_1, KC_2, KC_3,
        // Col 1 (5 teclas)
        KC_W, KC_S, KC_X, KC_4, KC_5,
        // Col 2 (5 teclas)
        KC_E, KC_D, KC_C, KC_6, KC_7,
        // Col 3 (8 teclas)
        KC_R, KC_F, KC_V, KC_T, KC_G, KC_B, KC_SPC, KC_ENT,
        // Col 4 (2 teclas)
        MO(_FN), MO(_MOUSE),
        // Joystick 8 direcciones (visibles en Vial como JOY N..NW)
        KC_W,    KC_E,    KC_D,    KC_C,
        KC_S,    KC_Z,    KC_A,    KC_Q
    ),

    [_FN] = LAYOUT_custom(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
        KC_HOME, KC_END,  KC_PGUP, KC_PGDN, KC_INS,
        KC_ESC,  KC_TAB,  KC_BSPC, KC_DEL,  KC_PSCR, KC_SCRL, TG(_GAME), QK_BOOT,
        _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______
    ),

    [_MOUSE] = LAYOUT_custom(
        KC_NO,   MS_BTN1, MS_BTN2, MS_BTN3, KC_NO,   KC_NO,
        MS_UP, KC_NO,   KC_NO,   KC_NO,   KC_NO,
        MS_DOWN, MS_LEFT, MS_RGHT, KC_NO,   KC_NO,
        MS_WHLU, MS_WHLD, MS_WHLL, MS_WHLR, KC_NO, KC_NO, KC_NO, KC_NO,
        _______, _______,
        MS_UP, KC_NO,   MS_RGHT, KC_NO,
        MS_DOWN, KC_NO,   MS_LEFT, KC_NO
    ),

    [_GAME] = LAYOUT_custom(
        KC_TAB,  KC_LSFT, KC_LCTL, KC_1,    KC_2,    KC_3,
        KC_Q,    KC_A,    KC_Z,    KC_4,    KC_5,
        KC_W,    KC_S,    KC_X,    KC_6,    KC_7,
        KC_E,    KC_D,    KC_C,    KC_R,    KC_F, KC_V, KC_SPC, TG(_GAME),
        KC_ESC,  KC_ENT,
        KC_W,    KC_E,    KC_D,    KC_C,
        KC_S,    KC_Z,    KC_A,    KC_Q
    )
};

