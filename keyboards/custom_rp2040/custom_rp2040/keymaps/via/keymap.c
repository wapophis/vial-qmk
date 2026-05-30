#include QMK_KEYBOARD_H

// ─── Capas ────────────────────────────────────────────────────────────────────
enum layers {
    _GAME = 0,   // Base gaming (FPS/TPS)
    _FN,         // Números, F-keys, accesos extra
    _MMO,        // MMO / muchas habilidades
    _SYS         // Sistema, media, reset
};
 
// ─── Layout de referencia ─────────────────────────────────────────────────────
//
//  Col0(meñique)  Col1(anular)  Col2(medio)  Col3(índice+pulgar)  Col4(pulgar)
//
//  LSFT           Q             E            R                    FN
//  LCTL           A             D            F                    MMO
//  Z              1             4            G
//  X              2             5            T
//  C              3             6            B
//  V                                         SPC
//                                            TAB
//                                            ESC
//
//  Joystick → W/A/S/D (movimiento)
 
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 
    // ── GAME: FPS / TPS ───────────────────────────────────────────────────────
    // Joystick mueve el personaje (WASD)
    // Dedos controlan habilidades, modificadores y cámara
    [_GAME] = LAYOUT_custom(
        // Col 0 — meñique (modificadores y utility)
        KC_LSFT, KC_LCTL, KC_Z, KC_X, KC_C, KC_V,
        // Col 1 — anular (habilidades secundarias)
        KC_Q, KC_A, KC_1, KC_2, KC_3,
        // Col 2 — medio (habilidades principales)
        KC_E, KC_D, KC_4, KC_5, KC_6,
        // Col 3 — índice + pulgar (acciones frecuentes)
        KC_R, KC_F, KC_G, KC_T, KC_B, KC_SPC, KC_TAB, KC_ESC,
        // Col 4 — pulgar (capas)
        MO(_FN), MO(_MMO),
        // Joystick 8 direcciones → movimiento WASD
        KC_W, KC_NO, KC_D, KC_NO,
        KC_S, KC_NO, KC_A, KC_NO
    ),
 
    // ── FN: Números, F-keys, accesos extra ────────────────────────────────────
    [_FN] = LAYOUT_custom(
        // Col 0
        _______, _______, KC_LALT, KC_LGUI, LALT(KC_F4), KC_GRV,
        // Col 1
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        // Col 2
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,
        // Col 3
        KC_F11, KC_F12, KC_PSCR, KC_INS, KC_DEL, KC_BSPC, KC_ENT, TG(_SYS),
        // Col 4
        _______, _______,
        // Joystick → flechas de cursor en esta capa
        KC_UP, KC_NO, KC_RIGHT, KC_NO,
        KC_DOWN, KC_NO, KC_LEFT, KC_NO
    ),
 
    // ── MMO: Muchas habilidades (barras de acción) ────────────────────────────
    [_MMO] = LAYOUT_custom(
        // Col 0
        KC_LSFT, KC_LCTL, KC_Z, KC_X, KC_C, KC_V,
        // Col 1 — habilidades 7-11
        KC_7, KC_8, KC_9, KC_0, KC_MINS,
        // Col 2 — habilidades extra
        KC_U, KC_I, KC_O, KC_P, KC_LBRC,
        // Col 3
        KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_SPC, KC_TAB, KC_ESC,
        // Col 4
        MO(_FN), _______,
        // Joystick → movimiento igual
        KC_W, KC_NO, KC_D, KC_NO,
        KC_S, KC_NO, KC_A, KC_NO
    ),
 
    // ── SYS: Sistema, media, reset ────────────────────────────────────────────
    [_SYS] = LAYOUT_custom(
        // Col 0
        KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, KC_MPRV, KC_MNXT,
        // Col 1
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        // Col 2
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        // Col 3
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TG(_SYS), QK_BOOT,
        // Col 4
        _______, _______,
        // Joystick desactivado en SYS
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    )
};

/*enum layers { _BASE = 0, _FN, _MOUSE, _GAME };

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
        KC_W, KC_NO, KC_D, KC_NO,
        KC_S, KC_NO, KC_A, KC_NO
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
        KC_W, KC_NO, KC_D, KC_NO,
        KC_S, KC_NO, KC_A, KC_NO
    )
};*/

/*void keyboard_post_init_user(void) {
    debug_enable  = true;
    debug_matrix  = true;
    debug_keyboard = true;
    debug_mouse   = true;
}*/
