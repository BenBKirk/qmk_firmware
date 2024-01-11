/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration */

#define MASTER_RIGHT
// #define MASTER_LEFT
#define EE_HANDS

#define TAPPING_TOGGLE 2
#define COMBO_VARIABLE_LEN


#ifdef MOUSEKEY_ENABLE
    #define MOUSEKEY_MAX_SPEED 2
    #define MOUSEKEY_WHEEL_DELAY 0
    #define MOUSEKEY_TIME_TO_MAX 40
#endif

#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"

#define PS2_CLOCK_PIN B5
#define PS2_DATA_PIN  B4
#define PS2_PIO_USE_PIO1
#define PS2_MOUSE_ROTATE 180
// #define PS2_MOUSE_SCROLL_BTN_MASK (1 << PS2_MOUSE_BTN_MIDDLE)
#define PS2_MOUSE_SCROLL_BTN_MASK 0
#define PS2_MOUSE_SCROLL_BTN_SEND 300

// #define PS2_MOUSE_X_MULTIPLIER 3
// #define PS2_MOUSE_Y_MULTIPLIER 3

#define PS2_MOUSE_SCROLL_DIVISOR_H 16
#define PS2_MOUSE_SCROLL_DIVISOR_V 16



#define PS2_MOUSE_DEBUG_RAW
#define PS2_MOUSE_DEBUG_HID