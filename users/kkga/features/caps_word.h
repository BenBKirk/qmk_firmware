#pragma once

#include QMK_KEYBOARD_H

bool process_caps_word(uint16_t keycode, keyrecord_t* record);

// To be implemented by the consumer. Defines keys to ignore when caps_word is active.
bool is_caps_word_ignored_key(uint16_t keycode);
