#pragma once

// ─── Matriz 8×6 ───────────────────────────────────────────────────────────────
#define MATRIX_ROWS 8
#define MATRIX_COLS 6

// ─── Joystick analógico → 4 cardinales (diagonales = 2 teclas) ───────────────
#define JOYSTICK_AXIS_COUNT      2
#define JOYSTICK_AXIS_RESOLUTION 8

#define JOY_X_PIN    GP26
#define JOY_Y_PIN    GP27
#define JOY_BTN_PIN  GP28

// Zona muerta (0-100): sube si el joystick deriva en reposo
#define JOY_DEAD_ZONE 25

// Offset angular inicial (0-359). Rota el "norte" del joystick.
// También se guarda en EEPROM; usa JOY_ANG_* en capa SYS para calibrar sin reflash.
#ifndef JOY_ANGLE_OFFSET_DEG
#    define JOY_ANGLE_OFFSET_DEG 180
#endif
// Grados por pulsación de JOY_ANG_INC / JOY_ANG_DEC
#ifndef JOY_ANGLE_OFFSET_STEP
#    define JOY_ANGLE_OFFSET_STEP 5
#endif

#define JOY_MATRIX_COL 5

// EEPROM del teclado: magic + offset angular persistente
#define EECONFIG_KB_DATA_SIZE 4

// ─── Vial ─────────────────────────────────────────────────────────────────────
#define DYNAMIC_KEYMAP_LAYER_COUNT 4

#define TAPPING_TERM    200
#define QUICK_TAP_TERM  120

#define BOOTMAGIC_LITE_ROW    0
#define BOOTMAGIC_LITE_COLUMN 0
