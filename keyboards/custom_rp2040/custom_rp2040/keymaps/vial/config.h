#pragma once

#define VIAL_KEYBOARD_UID {0x79, 0x3D, 0x84, 0xC3, 0xE8, 0x3D, 0x46, 0x7F}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 1 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 0 }

// Valor por defecto del offset (0-359) si EEPROM está vacía
#ifndef JOY_ANGLE_OFFSET_DEG
#    define JOY_ANGLE_OFFSET_DEG 180
#endif
