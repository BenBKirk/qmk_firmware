#include QMK_KEYBOARD_H

#include "kkga.h"

bool is_oneshot_cancel_key(uint16_t keycode) {
  switch (keycode) {
  // case LA_SYM:
  case LA_EXT:
    return true;
  default:
    return false;
  }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
  switch (keycode) {
  case LA_SYM:
  case LA_EXT:
  case KC_RSFT:
  case OS_SFT:
  case OS_CTL:
  case OS_ALT:
  case OS_CMD:
    return true;
  default:
    return false;
  }
}

bool is_swapper_ignored_key(uint16_t keycode) {
  switch (keycode) {
  case KC_LEFT:
  case KC_RIGHT:
  case KC_UP:
  case KC_DOWN:
  case KC_SPC:
  case KC_LSFT:
  case KC_ESC:
  case KC_W:
  case KC_M:
  case KC_Q:
  case KC_H:
    return true;
  default:
    return false;
  }
}

bool is_caps_word_ignored_key(uint16_t keycode) {
  switch (keycode) {
  case KC_1 ... KC_0:
  case KC_BSPC:
  case KC_MINS:
  case KC_UNDS:
  case LA_SYM:
  case LA_SYM2:
  case LA_EXT:
    return true;
  default:
    return false;
  }
}

bool cmd_tab_active = false;
bool alt_tab_active = false;

oneshot_state os_sft_state = os_up_unqueued;
oneshot_state os_ctl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  update_swapper(&alt_tab_active, KC_LALT, KC_TAB, ALT_TAB, keycode, record);
  update_swapper(&cmd_tab_active, KC_LGUI, KC_TAB, CMD_TAB, keycode, record);
  update_oneshot(&os_sft_state, KC_LSFT, OS_SFT, keycode, record);
  update_oneshot(&os_ctl_state, KC_LCTL, OS_CTL, keycode, record);
  update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
  update_oneshot(&os_cmd_state, KC_LCMD, OS_CMD, keycode, record);

  if (!process_caps_word(keycode, record)) {
    return false;
  }

  return true;
}
