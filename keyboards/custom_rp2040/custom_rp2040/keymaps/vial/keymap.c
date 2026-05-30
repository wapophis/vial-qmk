#include QMK_KEYBOARD_H

enum layers {
    _GAME = 0,
    _FN,
    _MMO,
    _SYS
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_GAME] = LAYOUT_custom(
        KC_LSFT, KC_LCTL, KC_Z, KC_X, KC_C, KC_V,
        KC_Q, KC_Y, KC_1, KC_2, KC_3,
        KC_E, KC_H, KC_4, KC_5, KC_6,
        KC_R, KC_F, KC_G, KC_T, KC_B, KC_SPC, KC_TAB, KC_ESC,
        MO(_FN), MO(_MMO),
        KC_W, KC_NO, KC_D, KC_NO,
        KC_S, KC_NO, KC_A, KC_NO
    ),

    [_FN] = LAYOUT_custom(
        _______, _______, KC_LALT, KC_LGUI, LALT(KC_F4), KC_GRV,
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,
        KC_F11, KC_F12, KC_PSCR, KC_INS, KC_DEL, KC_BSPC, KC_ENT, TG(_SYS),
        _______, _______,
        KC_UP, KC_NO, KC_RIGHT, KC_NO,
        KC_DOWN, KC_NO, KC_LEFT, KC_NO
    ),

    [_MMO] = LAYOUT_custom(
        KC_LSFT, KC_LCTL, KC_Z, KC_X, KC_C, KC_V,
        KC_7, KC_8, KC_9, KC_0, KC_MINS,
        KC_U, KC_I, KC_O, KC_P, KC_LBRC,
        KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_SPC, KC_TAB, KC_ESC,
        MO(_FN), _______,
        KC_W, KC_NO, KC_D, KC_NO,
        KC_S, KC_NO, KC_A, KC_NO
    ),

    [_SYS] = LAYOUT_custom(
        KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, KC_MPRV, KC_MNXT,
        JOY_ANG_DEC, JOY_ANG_RST, JOY_ANG_INC, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TG(_SYS), QK_BOOT,
        TG(_SYS), _______,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    )
};
