/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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

#include QMK_KEYBOARD_H
#include <stdio.h>
#define KC_ KC_TRNS

#ifdef PS2_MOUSE_ENABLE
#include "ps2_mouse.h"
#include "ps2.h"
#endif

// Trackpoint sensitivity
static uint8_t trackpoint_sensitivity = 255; // 0-255

enum custom_keycodes {
    DRAG_SCROLL = SAFE_RANGE
};
// scroll declarations
bool set_user_scroll = false;

//variables to store accumulated scroll values
float scroll_accumulated_h= 0;
float scroll_accumulated_v= 0;

extern int tp_buttons; // mousekey button state set in action.c and used in ps2_mouse.c


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_LGUI,   KC_Q,   KC_W,     KC_E,    KC_R,   KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   C(KC_C),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LALT,   KC_A,   KC_S,     KC_D,    KC_F,   KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,   KC_UP,   C(KC_V),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,   KC_Z,   KC_X,     KC_C,    KC_V,   KC_B,                         KC_N,    KC_M,  DRAG_SCROLL, KC_LEFT, KC_DOWN, KC_RGHT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_ESC, KC_CAPS, KC_SPC,     KC_BSPC, KC_LSFT,  KC_DEL
                                      //`--------------------------'  `--------------------------'
  ),
    
  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS,  KC_1,    KC_2,    KC_3,  KC_TRNS, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS,  KC_4,    KC_5,    KC_6,  KC_TRNS, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS,  KC_7,    KC_8,    KC_9,  KC_TRNS, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(0),  KC_TRNS, KC_TRNS,    KC_TRNS,   KC_0,  KC_TRNS
                                      //`--------------------------'  `--------------------------'
  ),
    
  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS,  KC_F1,   KC_F2,   KC_F3,   KC_F4, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS,  KC_F5,   KC_F6,   KC_F7,   KC_F8, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS,  KC_F9,   KC_F10, KC_F11,  KC_F12, KC_TRNS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TO(0),  KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  )
    
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Default"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
    }
}

char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}



enum combos { 
    //Number layer
    D_F_LAYER,
    W_D_LAYER,
    
    //Function layer
    Q_W_LAYER,
    
    //Common symbols
    L_UP_DOT,
    K_L_COMM,
    J_L_UP_COLN,
    J_K_L_SCLN,
    I_L_QUOT,
    U_L_DQUO,
    //Number layer
    numDOT,
    numCOMM,
    numCOLN,
    numSCLN,
    numQUOT,
    numDQUO,
    
    //Basic math symbols
    Y_U_EQL,
    H_J_MINS,
    H_J_K_PLUS,
    N_M_UNDS,
    //Number layer
    Y_1_EQL,
    H_4_MINS,
    H_4_5_PLUS,
    N_7_UNDS,
    
    //Parentheses
    BSPC_U_LBRC,
    BSPC_I_RBRC,
    BSPC_J_LPRN,
    BSPC_K_RPRN,
    BSPC_M_LCBR,
    BSPC_BTN1_RCBR,
    //Number layer
    BSPC_1_LBRC,
    BSPC_2_RBRC,
    BSPC_4_LPRN,
    BSPC_5_RPRN,
    BSPC_7_LCBR,
    BSPC_8_RCBR,
    
    //Symbols
    M_K_O_SLSH,
    U_K_LEFT_BSLS,
    I_J_LSFT_PIPE,
    J_I_L_CIRC,
    I_J_DEL_LT,
    U_K_BSPC_GT,
    Y_J_GRV,
    J_P_TILD,
    Y_U_I_HASH,
    U_LEFT_PERC,
    U_I_L_EXLM,
    J_K_O_QUES,
    M_BTN1_O_DLR,
    U_K_L_AT,
    H_K_AMPR,
    N_M_BTN1_ASTR,
    //Symbols on number layer
    numSLSH,
    numBSLS,
    numPIPE,
    numCIRC,
    numLT,
    numGT,
    numGRV,
    numTILD,
    numHASH,
    numPERC,
    numEXLM,
    numQUES,
    numDLR,
    numAT,
    numAMPR,
    numASTR,
    
    //Control keys
    J_K_L_UP_ENT,
    numENT,
    // V_B_LSFT_CAPS,

    //Navigation keys
    U_I_O_HOME,
    I_O_P_END,
    H_L_PGUP,
    N_LEFT_PGDN,
    LSFT_DEL_INS,
    
    //Additional media keys
    P_VOLU_MPRV,
    L_VOLD_MNXT,
    RGHT_MPLY_MUTE,
    
    //Mouse keys
    // M_DOWN_BTN2,
    // M_LEFT_DOWN_BTN3,
    J_K_BTN1,
    J_UP_DRAG_SCROLL,
    // M_RGHT_WH_D,
    // M_L_WH_L,
    // J_RGHT_WH_R,
    J_L_BTN2,
    
    //Misc 
    U_I_O_P_TAB,
    P_C_M_R_LCG_SWP,
    M_A_C_LCG_NRM,
    // Q_S_C_F_T_RESET,
    // Y_J_LEFT_UP_P_RESET,
    Q_W_E_R_T_NK_TOGG,
    ASDF_LOREM,

    COMBO_LENGTH
};



uint16_t COMBO_LEN = COMBO_LENGTH;
    //Number layer
    const uint16_t PROGMEM d_f_layer[] = { KC_D, KC_F, COMBO_END};
    const uint16_t PROGMEM w_d_layer[] = { KC_W, KC_D, COMBO_END};

    //Function layer
    const uint16_t PROGMEM q_w_layer[] = { KC_Q, KC_W, COMBO_END};

    //Common symbols
    const uint16_t PROGMEM l_up_dot[] = { KC_L, KC_UP, COMBO_END};
    const uint16_t PROGMEM k_l_comm[] = { KC_K, KC_L, COMBO_END};
    const uint16_t PROGMEM j_l_up_coln[] = { KC_J, KC_UP, KC_L, COMBO_END};
    const uint16_t PROGMEM j_k_l_scln[] = { KC_J, KC_K, KC_L, COMBO_END};
    const uint16_t PROGMEM i_l_quot[] = { KC_I, KC_L, COMBO_END};
    const uint16_t PROGMEM u_l_dquo[] = { KC_U, KC_L, COMBO_END};
    //Number layer
    const uint16_t PROGMEM numdot[] = { KC_6, KC_UP, COMBO_END};
    const uint16_t PROGMEM numcomm[] = { KC_5, KC_6, COMBO_END};
    const uint16_t PROGMEM numcoln[] = { KC_4, KC_6, KC_UP, COMBO_END};
    const uint16_t PROGMEM numscln[] = { KC_4, KC_5, KC_6, COMBO_END};
    const uint16_t PROGMEM numquot[] = { KC_2, KC_6, COMBO_END};
    const uint16_t PROGMEM numdquo[] = { KC_1, KC_6, COMBO_END};

    //Basic math symbols
    const uint16_t PROGMEM y_u_eql[] = { KC_Y, KC_U, COMBO_END};
    const uint16_t PROGMEM h_j_mins[] = { KC_H, KC_J, COMBO_END};
    const uint16_t PROGMEM h_j_k_plus[] = { KC_H, KC_J, KC_K, COMBO_END};
    const uint16_t PROGMEM n_m_unds[] = { KC_N, KC_M, COMBO_END};
    //Number layer
    const uint16_t PROGMEM y_1_eql[] = { KC_Y, KC_1, COMBO_END};
    const uint16_t PROGMEM h_4_mins[] = { KC_H, KC_4, COMBO_END};
    const uint16_t PROGMEM h_4_5_plus[] = { KC_H, KC_4, KC_5, COMBO_END};
    const uint16_t PROGMEM n_7_unds[] = { KC_N, KC_7, COMBO_END};

    //Parentheses
    const uint16_t PROGMEM lsft_u_lbrc[] = { KC_BSPC, KC_U, COMBO_END};
    const uint16_t PROGMEM lsft_i_rbrc[] = { KC_BSPC, KC_I, COMBO_END};
    const uint16_t PROGMEM lsft_j_lprn[] = { KC_BSPC, KC_J, COMBO_END};
    const uint16_t PROGMEM lsft_k_rprn[] = { KC_BSPC, KC_K, COMBO_END};
    const uint16_t PROGMEM lsft_m_lcbr[] = { KC_BSPC, KC_M, COMBO_END};
    const uint16_t PROGMEM lsft_btn1_rcbr[] = { KC_BSPC, KC_MS_BTN1, COMBO_END};
    //Number layer
    const uint16_t PROGMEM lsft_1_lbrc[] = { KC_BSPC, KC_1, COMBO_END};
    const uint16_t PROGMEM lsft_2_rbrc[] = { KC_BSPC, KC_2, COMBO_END};
    const uint16_t PROGMEM lsft_4_lprn[] = { KC_BSPC, KC_4, COMBO_END};
    const uint16_t PROGMEM lsft_5_rprn[] = { KC_BSPC, KC_5, COMBO_END};
    const uint16_t PROGMEM lsft_7_lcbr[] = { KC_BSPC, KC_7, COMBO_END};
    const uint16_t PROGMEM lsft_8_rcbr[] = { KC_BSPC, KC_8, COMBO_END};

    //Symbols
    const uint16_t PROGMEM m_k_o_slsh[] = { KC_M, KC_K, KC_O, COMBO_END};
    const uint16_t PROGMEM u_k_LEFT_bsls[] = { KC_U, KC_K, KC_LEFT, COMBO_END};
    const uint16_t PROGMEM i_j_lsft_pipe[] = { KC_I, KC_J, KC_LSFT, COMBO_END};
    const uint16_t PROGMEM j_i_l_circ[] = { KC_J, KC_I, KC_L, COMBO_END};
    const uint16_t PROGMEM i_j_del_lt[] = { KC_I, KC_J, KC_DEL, COMBO_END};
    const uint16_t PROGMEM u_k_lsft_gt[] = { KC_U, KC_K, KC_BSPC, COMBO_END};
    const uint16_t PROGMEM y_j_grv[] = { KC_Y, KC_J, COMBO_END};
    const uint16_t PROGMEM j_p_tild[] = { KC_J, KC_P, COMBO_END};
    const uint16_t PROGMEM y_u_i_hash[] = { KC_Y, KC_U, KC_I, COMBO_END};
    const uint16_t PROGMEM u_left_perc[] = { KC_U, KC_LEFT, COMBO_END};
    const uint16_t PROGMEM u_i_l_exlm[] = { KC_U, KC_I, KC_L, COMBO_END};
    const uint16_t PROGMEM j_k_o_ques[] = { KC_J, KC_K, KC_O, COMBO_END};
    const uint16_t PROGMEM m_btn1_o_dlr[] = { KC_M, KC_MS_BTN1, KC_O, COMBO_END};
    const uint16_t PROGMEM u_k_l_at[] = { KC_U, KC_K, KC_L, COMBO_END};
    const uint16_t PROGMEM h_k_ampr[] = { KC_H, KC_K, COMBO_END};
    const uint16_t PROGMEM n_m_btn1_astr[] = { KC_N, KC_M, KC_MS_BTN1, COMBO_END};
    //Symbols on number layer
    const uint16_t PROGMEM numslsh[] = { KC_7, KC_5, KC_3, COMBO_END};
    const uint16_t PROGMEM numbsls[] = { KC_1, KC_5, KC_9, COMBO_END};
    const uint16_t PROGMEM numpipe[] = { KC_2, KC_4, KC_0, COMBO_END};
    const uint16_t PROGMEM numcirc[] = { KC_4, KC_2, KC_6, COMBO_END};
    const uint16_t PROGMEM numlt[] = { KC_2, KC_4, KC_DEL, COMBO_END};
    const uint16_t PROGMEM numgt[] = { KC_1, KC_5, KC_BSPC, COMBO_END};
    const uint16_t PROGMEM numgrv[] = { KC_Y, KC_4, COMBO_END};
    const uint16_t PROGMEM numtild[] = { KC_4, KC_P, COMBO_END};
    const uint16_t PROGMEM numhash[] = { KC_Y, KC_1, KC_2, COMBO_END};
    const uint16_t PROGMEM numperc[] = { KC_1, KC_9, COMBO_END};
    const uint16_t PROGMEM numexlm[] = { KC_1, KC_2, KC_6, COMBO_END};
    const uint16_t PROGMEM numques[] = { KC_4, KC_5, KC_3, COMBO_END};
    const uint16_t PROGMEM numdlr[] = { KC_7, KC_8, KC_3, COMBO_END};
    const uint16_t PROGMEM numat[] = { KC_1, KC_5, KC_6, COMBO_END};
    const uint16_t PROGMEM numampr[] = { KC_H, KC_5, COMBO_END};
    const uint16_t PROGMEM numastr[] = { KC_N, KC_7, KC_8, COMBO_END};

    //Control keys
    const uint16_t PROGMEM j_k_l_up_ent[] = { KC_J, KC_K, KC_L, KC_UP, COMBO_END};
    const uint16_t PROGMEM nument[] = { KC_4, KC_5, KC_6, KC_UP, COMBO_END};
    // const uint16_t PROGMEM v_b_lsft _caps[] = { KC_V, KC_B, KC_LSFT, COMBO_END};

    //Navigation keys
    const uint16_t PROGMEM u_i_o_home[] = { KC_U, KC_I, KC_O, COMBO_END};
    const uint16_t PROGMEM i_o_p_end[] = { KC_I, KC_O, KC_P, COMBO_END};
    const uint16_t PROGMEM h_l_pgup[] = { KC_H, KC_L, COMBO_END};
    const uint16_t PROGMEM n_left_pgdn[] = { KC_N, KC_LEFT, COMBO_END};
    const uint16_t PROGMEM lsft_del_ins[] = { KC_LSFT, KC_DEL, COMBO_END};

    //Additional media keys
    const uint16_t PROGMEM p_volu_mprv[] = { KC_P, KC_VOLU, COMBO_END};
    const uint16_t PROGMEM l_vold_mnxt[] = { KC_L, KC_VOLD, COMBO_END};
    const uint16_t PROGMEM rght_mply_mute[] = { KC_RGHT, KC_MPLY, COMBO_END};

    //Mouse keys
    const uint16_t PROGMEM j_k_btn1[] = { KC_J, KC_K, COMBO_END};
    // const uint16_t PROGMEM m_left_btn1[] = { KC_M, KC_LEFT, COMBO_END};
    // const uint16_t PROGMEM m_down_btn2[] = { KC_M, KC_DOWN, COMBO_END};
    // const uint16_t PROGMEM m_left_down_btn3[] = { KC_M, KC_LEFT, KC_DOWN, COMBO_END};
    const uint16_t PROGMEM j_up_drag_scroll[] = { KC_J, KC_UP, COMBO_END};
    // const uint16_t PROGMEM m_RGHT_wh_d[] = { KC_M, KC_RGHT, COMBO_END};
    // const uint16_t PROGMEM m_L_wh_l[] = { KC_M, KC_L, COMBO_END};
    // const uint16_t PROGMEM j_RGHT_wh_r[] = { KC_J, KC_RGHT, COMBO_END};
    const uint16_t PROGMEM j_l_btn2[] = { KC_J, KC_L, COMBO_END};

    //Misc
    const uint16_t PROGMEM u_i_o_p_tab[] = { KC_U, KC_I, KC_O, KC_P, COMBO_END};
    const uint16_t PROGMEM p_c_m_r_lcg_swp[] = { KC_P, KC_C, KC_M, KC_R, COMBO_END};
    const uint16_t PROGMEM m_a_c_lcg_nrm[] = { KC_M, KC_A, KC_C, COMBO_END};
    // const uint16_t PROGMEM q_s_c_f_t_reset[] = { KC_Q, KC_S, KC_C, KC_F, KC_T, COMBO_END};
    // const uint16_t PROGMEM y_j_left_UP_p_reset[] = { KC_Y, KC_J, KC_LEFT, KC_UP, KC_P, COMBO_END};
    const uint16_t PROGMEM q_w_e_r_t_nk_togg[] = { KC_Q, KC_W, KC_E, KC_R, KC_T, COMBO_END};
    const uint16_t PROGMEM lorem_combo[] = {KC_A, KC_S, KC_D, KC_F, COMBO_END};



combo_t key_combos[] = {
    //Number layer
    [D_F_LAYER] = COMBO(d_f_layer, TT(1)),
    [W_D_LAYER] = COMBO(w_d_layer, TT(1)),

    //Function layer
    [Q_W_LAYER] = COMBO(q_w_layer, TT(2)),

    //Common symbols
    [L_UP_DOT] = COMBO(l_up_dot, KC_DOT),
    [K_L_COMM] = COMBO(k_l_comm, KC_COMM),
    [J_L_UP_COLN] = COMBO(j_l_up_coln, KC_COLN),
    [J_K_L_SCLN] = COMBO(j_k_l_scln, KC_SCLN),
    [I_L_QUOT] = COMBO(i_l_quot, KC_QUOT),
    [U_L_DQUO] = COMBO(u_l_dquo, KC_DQUO),
    //Number layer
    [numDOT] = COMBO(numdot, KC_DOT),
    [numCOMM] = COMBO(numcomm, KC_COMM),
    [numCOLN] = COMBO(numcoln, KC_COLN),
    [numSCLN] = COMBO(numscln, KC_SCLN),
    [numQUOT] = COMBO(numquot, KC_QUOT),
    [numDQUO] = COMBO(numdquo, KC_DQUO),

    //Basic math symbols
    [Y_U_EQL] = COMBO(y_u_eql, KC_EQL),
    [H_J_MINS] = COMBO(h_j_mins, KC_MINS),
    [H_J_K_PLUS] = COMBO(h_j_k_plus, KC_PLUS),
    [N_M_UNDS] = COMBO(n_m_unds, KC_UNDS),
    //Number layer
    [Y_1_EQL] = COMBO(y_1_eql, KC_EQL),
    [H_4_MINS] = COMBO(h_4_mins, KC_MINS),
    [H_4_5_PLUS] = COMBO(h_4_5_plus, KC_PLUS),
    [N_7_UNDS] = COMBO(n_7_unds, KC_UNDS),

    //Parentheses
    [BSPC_U_LBRC] = COMBO(lsft_u_lbrc, KC_LBRC),
    [BSPC_I_RBRC] = COMBO(lsft_i_rbrc, KC_RBRC),
    [BSPC_J_LPRN] = COMBO(lsft_j_lprn, KC_LPRN),
    [BSPC_K_RPRN] = COMBO(lsft_k_rprn, KC_RPRN),
    [BSPC_M_LCBR] = COMBO(lsft_m_lcbr, KC_LCBR),
    [BSPC_BTN1_RCBR] = COMBO(lsft_btn1_rcbr, KC_RCBR),
    //Number layer   
    [BSPC_1_LBRC] = COMBO(lsft_1_lbrc, KC_LBRC),
    [BSPC_2_RBRC] = COMBO(lsft_2_rbrc, KC_RBRC),
    [BSPC_4_LPRN] = COMBO(lsft_4_lprn, KC_LPRN),
    [BSPC_5_RPRN] = COMBO(lsft_5_rprn, KC_RPRN),
    [BSPC_7_LCBR] = COMBO(lsft_7_lcbr, KC_LCBR),
    [BSPC_8_RCBR] = COMBO(lsft_8_rcbr, KC_RCBR),

    //Symbols
    [M_K_O_SLSH] = COMBO(m_k_o_slsh, KC_SLSH),
    [U_K_LEFT_BSLS] = COMBO(u_k_LEFT_bsls, KC_BSLS),
    [I_J_LSFT_PIPE] = COMBO(i_j_lsft_pipe, KC_PIPE),
    [J_I_L_CIRC] = COMBO(j_i_l_circ, KC_CIRC),
    [I_J_DEL_LT] = COMBO(i_j_del_lt, KC_LT),
    [U_K_BSPC_GT] = COMBO(u_k_lsft_gt, KC_GT),
    [Y_J_GRV] = COMBO(y_j_grv, KC_GRV),
    [J_P_TILD] = COMBO(j_p_tild, KC_TILD),
    [Y_U_I_HASH] = COMBO(y_u_i_hash, KC_HASH),
    [U_LEFT_PERC] = COMBO(u_left_perc, KC_PERC),
    [U_I_L_EXLM] = COMBO(u_i_l_exlm, KC_EXLM),
    [J_K_O_QUES] = COMBO(j_k_o_ques, KC_QUES),
    [M_BTN1_O_DLR] = COMBO(m_btn1_o_dlr, KC_DLR),
    [U_K_L_AT] = COMBO(u_k_l_at, KC_AT),
    [H_K_AMPR] = COMBO(h_k_ampr, KC_AMPR),
    [N_M_BTN1_ASTR] = COMBO(n_m_btn1_astr, KC_ASTR),
    //Symbols on number layer
    [numSLSH] = COMBO(numslsh, KC_SLSH),
    [numBSLS] = COMBO(numbsls, KC_BSLS),
    [numPIPE] = COMBO(numpipe, KC_PIPE),
    [numCIRC] = COMBO(numcirc, KC_CIRC),
    [numLT] = COMBO(numlt, KC_LT),
    [numGT] = COMBO(numgt, KC_GT),
    [numGRV] = COMBO(numgrv, KC_GRV),
    [numTILD] = COMBO(numtild, KC_TILD),
    [numHASH] = COMBO(numhash, KC_HASH),
    [numPERC] = COMBO(numperc, KC_PERC),
    [numEXLM] = COMBO(numexlm, KC_EXLM),
    [numQUES] = COMBO(numques, KC_QUES),
    [numDLR] = COMBO(numdlr, KC_DLR),
    [numAT] = COMBO(numat, KC_AT),
    [numAMPR] = COMBO(numampr, KC_AMPR),
    [numASTR] = COMBO(numastr, KC_ASTR),

    //Control keys
    [J_K_L_UP_ENT] = COMBO(j_k_l_up_ent, KC_ENT),
    [numENT] = COMBO(nument, KC_ENT),
    // [V_B_LSFT_CAPS] = COMBO(v_b_lsft_caps, KC_CAPS),

    //Navigation keys
    [U_I_O_HOME] = COMBO(u_i_o_home, KC_HOME),
    [I_O_P_END] = COMBO(i_o_p_end, KC_END),
    [H_L_PGUP] = COMBO(h_l_pgup, KC_PGUP),
    [N_LEFT_PGDN] = COMBO(n_left_pgdn, KC_PGDN),
    [LSFT_DEL_INS] = COMBO(lsft_del_ins, KC_INS),

    //Additional media keys
    [P_VOLU_MPRV] = COMBO(p_volu_mprv, KC_MPRV),
    [L_VOLD_MNXT] = COMBO(l_vold_mnxt, KC_MNXT),
    [RGHT_MPLY_MUTE] = COMBO(rght_mply_mute, KC_MUTE),

    //Mouse keys
    // [M_LEFT_BTN1] = COMBO(m_left_btn1, KC_BTN1),
    // [M_DOWN_BTN2] = COMBO(m_down_btn2, KC_BTN2),
    // [M_LEFT_DOWN_BTN3] = COMBO(m_left_down_btn4, KC_BTN3),
    [J_K_BTN1] = COMBO(j_k_btn1, KC_BTN1),
    [J_UP_DRAG_SCROLL] = COMBO(j_up_drag_scroll, DRAG_SCROLL),
    // [M_RGHT_WH_D] = COMBO(m_RGHT_wh_d, KC_WH_D),
    // [M_L_WH_L] = COMBO(m_L_wh_l, KC_WH_L),
    // [J_RGHT_WH_R] = COMBO(j_RGHT_wh_r, KC_WH_R),
    [J_L_BTN2] = COMBO(j_l_btn2, KC_BTN2),

    //Misc
    [U_I_O_P_TAB] = COMBO(u_i_o_p_tab, KC_TAB),
    [ASDF_LOREM] = COMBO_ACTION(lorem_combo),
    [P_C_M_R_LCG_SWP] = COMBO(p_c_m_r_lcg_swp, LCG_SWP),
    [M_A_C_LCG_NRM] = COMBO(m_a_c_lcg_nrm, LCG_NRM),
    // [Q_S_C_F_T_RESET] = COMBO(q_s_c_f_t_reset, RESET),
    // [Y_J_LEFT_UP_P_RESET] = COMBO(y_j_left_UP_p_reset, RESET),
    [Q_W_E_R_T_NK_TOGG] = COMBO(q_w_e_r_t_nk_togg, NK_TOGG),
};



void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case ASDF_LOREM:
      if (pressed) {
        SEND_STRING("Lorem ipsum dolor sit amet ");
      }
      break;
  }
}


void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
    return false;
}

#endif // OLED_ENABLE

#ifdef PS2_MOUSE_ENABLE
void trackpoint_set_sensitivity(uint8_t sensitivity) {
    PS2_MOUSE_SEND(0xE2, "trackpoint_set_sensitivity: 0xE2");
    PS2_MOUSE_SEND(0x81, "trackpoint_set_sensitivity: 0x81");
    PS2_MOUSE_SEND(0x4A, "trackpoint_set_sensitivity: 0x4A");
    PS2_MOUSE_SEND(sensitivity, "trackpoint_set_sensitivity: xxx");
}

void trackpoint_reset(void) {
    PS2_MOUSE_SEND(0xE2, "trackpoint_reset: 0xE2");
    PS2_MOUSE_SEND(0x51, "trackpoint_reset: 0x51");
}

void ps2_mouse_init_user() {
    // set TrackPoint sensitivity
    trackpoint_set_sensitivity(trackpoint_sensitivity);
}

void ps2_regdump(void) {
    println("Register dump:");
    print("     ");
    for (uint8_t addr = 0; addr <= 0xF; addr++) {
        print_hex8(addr);
        print(" ");
    }
    println("");
    for (uint16_t addr = 0; addr <= 0xFF; addr++) {
        uint8_t data;
        if ((addr & 0xf) == 0) {
            print_hex8(addr);
            print(" | ");
        }

        ps2_host_send(0xE2);
        ps2_host_send(0x80);
        data = ps2_host_send((uint8_t)addr);
        if (data == 0xFA) {
            data = ps2_host_recv_response();
            print_hex8(data);
            print(" ");
        } else {
            print("xx ");
        }
        if ((addr & 0xf) == 0xf) {
            println("");
        }
    }
    println("");
}
#endif // PS2_MOUSE_ENABLE


void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
		// Set Scrolling 
	if (set_user_scroll) {
		 // Calculate and accumulate scroll values based on mouse movement and divisors
		scroll_accumulated_h += (float)mouse_report->x / PS2_MOUSE_SCROLL_DIVISOR_H;
		scroll_accumulated_v += (float)mouse_report->y / PS2_MOUSE_SCROLL_DIVISOR_V;
		
		// Assign integer parts of accumulated scroll values to the mouse report
		mouse_report->h = (int8_t)scroll_accumulated_h;
		mouse_report->v = (int8_t)scroll_accumulated_v;

    //     // Assign integer parts of accumulated scroll values to the mouse report
    //    temp = scroll_accumulated_h/ PS2_MOUSE_SCROLL_DIVISOR_H;
    //    if (temp > 127) {
    //    mouse_report->h = 127;
    //    } else if (temp < -127) {
    //    mouse_report->h = -127;
    //    } else {
    //    mouse_report->h = temp;
    //    }
		
		// Update accumulated scroll values by subtracting the integer parts
		scroll_accumulated_h -= (int8_t)scroll_accumulated_h;
		scroll_accumulated_v -= (int8_t)scroll_accumulated_v;

		// Clear the X and Y values of the mouse report
		mouse_report->x = 0;
		mouse_report->y = 0;
		
		// Invert scrolling direction
		// mouse_report->h = -mouse_report->h;
		mouse_report->v = -mouse_report->v;
		
		print("Scrolling!\n");
		// Send mouse report
		// pointing_device_set_report(*mouse_report);
		// pointing_device_send();
	}
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        if ( keycode == DRAG_SCROLL && record->event.pressed) {
            // set_keylog(keycode, record);
            set_user_scroll = true;
            
        } else {
            set_user_scroll = false;
        }
        return true; 
}