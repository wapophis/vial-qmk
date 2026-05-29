#pragma once

// ─── USB ──────────────────────────────────────────────────────────────────────
// #define VENDOR_ID    0xFEED
// #define PRODUCT_ID   0x0001
// #define DEVICE_VERSION   0x0100
// #define MANUFACTURER "Custom"
// #define PRODUCT      "Custom RP2040 26-key"

// ─── Matriz 8×6 ───────────────────────────────────────────────────────────────
// Col 0 → 6 teclas físicas  (R0-R5)
// Col 1 → 5 teclas físicas  (R0-R4)
// Col 2 → 5 teclas físicas  (R0-R4)
// Col 3 → 8 teclas físicas  (R0-R7)
// Col 4 → 2 teclas físicas  (R0-R1)
// Col 5 → 8 posiciones VIRTUALES del joystick (R0-R7, sin pin físico)
#define MATRIX_ROWS 8
#define MATRIX_COLS 6

// ─── Joystick analógico → 8 vías virtuales ───────────────────────────────────
#define JOYSTICK_AXIS_COUNT      2
#define JOYSTICK_AXIS_RESOLUTION 8

#define JOY_X_PIN    GP26   // ADC0 — eje horizontal
#define JOY_Y_PIN    GP27   // ADC1 — eje vertical
//#define JOY_X_PIN    GP27   // ADC0 — eje horizontal
//#define JOY_Y_PIN    GP26   // ADC1 — eje vertical
#define JOY_BTN_PIN  GP28   // Botón de click (opcional)

// Zona muerta (0-100): sube si el joystick tiene deriva en reposo
#define JOY_DEAD_ZONE 25
// Offset angular en grados para calibrar el norte del joystick (0-359)
#define JOY_ANGLE_OFFSET_DEG 180

// Columna virtual del joystick en la matriz
#define JOY_MATRIX_COL 5

// ─── VIA/Vial ─────────────────────────────────────────────────────────────────
#define DYNAMIC_KEYMAP_LAYER_COUNT 4

// ─── Misc ─────────────────────────────────────────────────────────────────────
#define TAPPING_TERM    200
#define QUICK_TAP_TERM  120
// #define FORCE_NKRO

#define BOOTMAGIC_LITE_ROW    0
#define BOOTMAGIC_LITE_COLUMN 0
