#pragma once

#include QMK_KEYBOARD_H

#include "features/caps_word.h"
#include "features/oneshot.h"
#include "features/swapper.h"

enum layers { QWR, CLM, SYM, SYM2, EXT, FUN };

enum keycodes {
  OS_SFT = SAFE_RANGE,
  OS_CTL,
  OS_ALT,
  OS_CMD,
  OSM_MEH,
  CMD_TAB,
  ALT_TAB,
};

// layer toggles
#define LA_EXT MO(EXT)
#define LA_SYM MO(SYM)
#define LA_SYM2 MO(SYM2)
#define LA_FUN MO(FUN)

// home block mods
#define HOME_S LALT_T(KC_S)
#define HOME_D LSFT_T(KC_D)
#define HOME_F LCTL_T(KC_F)
#define HOME_G MEH_T(KC_G)
#define HOME_V LGUI_T(KC_V)
#define HOME_M LGUI_T(KC_M)
#define HOME_H MEH_T(KC_H)
#define HOME_J RCTL_T(KC_J)
#define HOME_K RSFT_T(KC_K)
#define HOME_L LALT_T(KC_L)

// nav keys
#define S_TAB S(KC_TAB)
