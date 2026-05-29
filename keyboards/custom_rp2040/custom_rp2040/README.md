# Custom RP2040 — 26 teclas, columnas irregulares

## Distribución de teclas

```
Matriz lógica 8×5 (X = celda vacía)

       C0    C1    C2    C3    C4
  R0 [ k00] [k01] [k02] [k03] [k04]   ← 5 teclas activas
  R1 [ k10] [k11] [k12] [k13] [k14]   ← 5 teclas activas
  R2 [ k20] [k21] [k22] [k23] [ X ]   ← 4 teclas activas
  R3 [ k30] [k31] [k32] [k33] [ X ]   ← 4 teclas activas
  R4 [ k40] [k41] [k42] [k43] [ X ]   ← 4 teclas activas
  R5 [ k50] [ X ] [ X ] [k53] [ X ]   ← 2 teclas activas
  R6 [  X ] [ X ] [ X ] [k63] [ X ]   ← 1 tecla  activa
  R7 [  X ] [ X ] [ X ] [k73] [ X ]   ← 1 tecla  activa
                                         ────────────────
  Total activas:  6 + 5 + 5 + 8 + 2 = 26 teclas
```

## Pines del RP2040

| Función  | Pin  | Nota                        |
|----------|------|-----------------------------|
| ROW 0    | GP10 |                             |
| ROW 1    | GP11 |                             |
| ROW 2    | GP12 |                             |
| ROW 3    | GP13 |                             |
| ROW 4    | GP14 |                             |
| ROW 5    | GP15 |                             |
| ROW 6    | GP16 |                             |
| ROW 7    | GP17 |                             |
| COL 0    | GP0  |                             |
| COL 1    | GP1  |                             |
| COL 2    | GP2  |                             |
| COL 3    | GP3  |                             |
| COL 4    | GP4  |                             |
| JOY X    | GP26 | ADC0 — eje horizontal       |
| JOY Y    | GP27 | ADC1 — eje vertical         |
| JOY BTN  | GP28 | Botón de click del joystick |

> Cambia los pines en `config.h` y `info.json` según tu PCB real.

## Compilar y flashear

```bash
# 1. Copiar a QMK
cp -r custom_rp2040/ ~/qmk_firmware/keyboards/

# 2. Compilar (keymap con VIA)
qmk compile -kb custom_rp2040 -km via

# 3. Flashear (conecta el RP2040 en modo BOOTSEL)
qmk flash -kb custom_rp2040 -km via
# O arrastra el .uf2 al dispositivo USB que aparece
```

## Joystick — ajustes en `config.h`

```c
#define JOY_X_PIN    GP26   // Cambia si usas otro pin ADC
#define JOY_Y_PIN    GP27
#define JOY_BTN_PIN  GP28
#define JOY_DEAD_ZONE 25    // 0-100; sube si hay deriva en reposo
```

Si el eje Y está invertido, edita `custom_rp2040.c`:
```c
int32_t y = -(raw_y - 2048);   // quita el '-' para invertir
```

## Teclas del joystick — `custom_rp2040.c`

```c
static const uint16_t dir_keys[DIR_COUNT] = {
    [DIR_N]  = KC_W,
    [DIR_E]  = KC_D,
    [DIR_S]  = KC_S,
    [DIR_W]  = KC_A,
    [DIR_NE] = KC_E,
    [DIR_SE] = KC_C,
    [DIR_SW] = KC_Z,
    [DIR_NW] = KC_Q,
    // ... diagonales intermedias
};
```

## VIA — reconfiguración en vivo

1. Compila con `-km via`
2. Abre [usevia.app](https://usevia.app) en Chrome/Edge
3. Conecta el teclado → carga `keymaps/via/via.json`
4. Reasigna teclas sin recompilar

## Capas

| # | Nombre | Activación            |
|---|--------|-----------------------|
| 0 | BASE   | Siempre activa        |
| 1 | FN     | Mantener Col4-fila0   |
| 2 | MOUSE  | Mantener Col4-fila1   |
| 3 | GAME   | Toggle desde FN layer |
