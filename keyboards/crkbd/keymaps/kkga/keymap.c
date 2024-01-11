#include QMK_KEYBOARD_H
#include "kkga.h"

// include the gboards helpers for combos defined in userspace combos.def
#include "g/keymap_combo.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// -----------------------------------------------------------------------------------------
// |   q   |   w   |   e   |   r   |   t   | ----- |   y   |   u   |   i   |   o   |   p   |
// |   a   |   s   |   d   |   f   |   g   | ----- |   h   |   j   |   k   |   l   |   '   |
// |   z   |   x   |   c   |   v   |   b   | ----- |   n   |   m   |   ,   |   .   |   /   |
//                         |  EXT  |  spc  | ----- |  sft  |  SYM  |
[QWR] = LAYOUT(
  _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, _______,
  _______, KC_A,    HOME_S,  HOME_D,  HOME_F,  HOME_G,                    HOME_H,  HOME_J,  HOME_K,  HOME_L,  KC_QUOT, _______,
  _______, KC_Z,    KC_X,    KC_C,    HOME_V,  KC_B,                      KC_N,    HOME_M,  KC_COMM, KC_DOT,  KC_SLSH, _______,
                    _______, LA_EXT,  KC_SPC,                   KC_RSFT, LA_SYM,  _______
),

[CLM] = LAYOUT(
  _______, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                      KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT, _______,
  _______, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                      KC_M,    KC_N,    KC_E,    KC_I,    KC_O, _______,
  _______, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                      KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, _______,
                    _______, LA_EXT,  KC_SPC,                    KC_RSFT, LA_SYM,  _______
),

// -----------------------------------------------------------------------------------------
// |   !   |   @   |   ;   |   :   |   _   | ----- |   +   |   7   |   8   |   9   |   =   |
// |   \   |   |   |   {   |   (   |   [   | ----- |   -   |   4   |   5   |   6   |   0   |
// |   `   |   ~   |   }   |   )   |   ]   | ----- |   *   |   1   |   2   |   3   |   /   |
//                         |  ---  |  SYM2 | ----- |  ---  |  xxx  |
[SYM] = LAYOUT(
  _______, KC_EXLM, KC_AT,   KC_SCLN, KC_COLN, KC_UNDS,                   KC_PLUS, KC_7,    KC_8,    KC_9,    KC_EQL, _______,
  _______, KC_BSLS, KC_PIPE, KC_LCBR, KC_LPRN, KC_LBRC,                   KC_MINS, KC_4,    KC_5,    KC_6,    KC_0, _______,
  _______, KC_GRV,  KC_TILD, KC_RCBR, KC_RPRN, KC_RBRC,                   KC_ASTR, KC_1,    KC_2,    KC_3,    KC_SLSH, _______,
                    _______, _______, LA_SYM2,                   _______, _______, _______
),

// -----------------------------------------------------------------------------------------
// |       |       |       |       |       | ----- |       |   &   |   `   |   ~   |       |
// |       |       |       |       |       | ----- |       |   $   |   %   |   ^   |   _   |
// |       |       |       |       |       | ----- |       |   !   |   @   |   #   |   \   |
//                         |  ---  |  xxx  | ----- |  ---  |  ---  |
[SYM2] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   _______, KC_AMPR, KC_GRV,  KC_TILD, XXXXXXX, _______,
  _______, _______, _______, _______, _______, _______,                   _______, KC_DLR,  KC_PERC, KC_CIRC, KC_UNDS, _______,
  _______,_______,  _______, _______, _______, _______,                   _______, KC_EXLM, KC_AT,   KC_HASH, KC_BSLS, _______,
                             _______, _______, XXXXXXX,                   _______, _______, _______
),

// -----------------------------------------------------------------------------------------
// |       | cm-tb | al-tb |       | reset | ----- |  hom  | pgdwn | pgup  |  end  |  bsp  |
// |  esc  |  alt  |  sft  |  ctl  |  meh  | ----- |  lft  |  dwn  |  up   |  rht  |  bsp  |
// |       | sf-tb |  tab  |  cmd  |       | ----- |       |  ent  |       |       |  del  |
//                         |  xxx  |  fun  | ----- |  ---  |  ---  |
[EXT] = LAYOUT(
  _______, XXXXXXX, CMD_TAB, ALT_TAB, XXXXXXX, _______,                     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_BSPC, _______,
  _______, KC_ESC,  OS_ALT,  OS_SFT,  OS_CTL,  OSM_MEH,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,KC_BSPC, _______,
  _______, XXXXXXX, S_TAB,   KC_TAB,  OS_CMD,  XXXXXXX,                   _______, KC_ENT,  _______, _______, KC_DEL, _______,
                             _______, XXXXXXX, LA_FUN,                    _______, _______, _______
),

// -----------------------------------------------------------------------------------------
// |  br-  |  br+  |  vl-  |  vl+  |  mut  | ----- |  f12  |  f7   |  f8   |  f9   |       |
// |       |       |       |       |       | ----- |  f11  |  f4   |  f5   |  f6   |  clm  |
// |       |       |       |       |       | ----- |  f10  |  f1   |  f2   |  f3   |  qwr  |
//                         |  ---  |  ---  | ----- |  ---  |  ---  |
[FUN] = LAYOUT(
  _______, KC_BRID, KC_BRIU, KC_VOLD, KC_VOLU, KC_MUTE,                   KC_F12,  KC_F7,   KC_F8,   KC_F9,   XXXXXXX, _______,
  _______, _______, _______, _______, _______, _______,                   KC_F11,  KC_F4,   KC_F5,   KC_F6,   DF(CLM), _______,
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_F10,  KC_F1,   KC_F2,   KC_F3,   DF(QWR), _______,
                    _______, _______, XXXXXXX,                   _______, _______, _______
)
};
