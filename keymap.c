#include "macros.h"
#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
    extern rgblight_config_t rgblight_config;
#endif

#ifdef OLED_ENABLE
    static uint32_t oled_timer = 0;
#endif

/* --------- DEFINITION VARIABLES --------------- */

bool is_shift_hold = false;
bool is_editor_focus = true;
bool screen_is_maximized = false;
bool screen_change_active = false;

uint8_t split_screen_half_counter = 1;
uint8_t split_screen_updown_counter = 1;
uint8_t split_screen_third_counter = 1;

int RGB_current_mode;

enum layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    BSP_DEL,
    LOWER,
    RAISE,
    ADJUST,
    TOGGLE_SIDEBAR_EDITOR,
    SPLIT_SCREEN_HALF,
    SPLIT_SCREEN_UPDOWN,
    SPLIT_SCREEN_THIRD,
    RGBRST,
    MACRO_1,
    MACRO_2,
    MACRO_3,
    MACRO_4,
    MACRO_5,
    MACRO_6,
    MACRO_7,
    MACRO_8,
    SLEEP,
    COLOR_PICKER,
    SCREEN_CAPTURE,
    SCREEN_RECORDER,
    SCREEN_CHANGE,
    APPS_SEARCH,
    MOD_ALT,
    MOD_GUI,
    MOD_CTL,
};

enum {
    TILD_BSLS,
    ROUNDBRCKTS,
    CURLYBRCKTS,
    SQUAREBRCKTS,
    ANGLEBRCKTS,
    SQUOT_DQUOT,
    DESKTOPLEFT,
    DESKTOPRIGHT,
    APPS_WORKSPACES,
    CREATEWSP_REMOVEWSP,
    MAXREST_CHDISPLAY,
    MORDINAL_FORDINAL,
    SRCHFILES_SRCHREPOS,
    EXC_QUESTION,
    DLR_EURO,
    ENE_ENIE,
    A_ACUTE_ACCENT,
    E_ACUTE_ACCENT,
    I_ACUTE_ACCENT,
    O_ACUTE_ACCENT,
    U_ACUTE_ACCENT,
};


/* ------------------------------------------------------------ LAYERS KEYMAPS ------------------------------------------------------------------------ */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
    //|-----------------------------------------------------|                    |-----------------------------------------------------|
        KC_TAB,  KC_Q,  KC_W,  TD(E_ACUTE_ACCENT),  KC_R,  KC_T,                       KC_Y,  TD(U_ACUTE_ACCENT),  TD(I_ACUTE_ACCENT),  TD(O_ACUTE_ACCENT),  KC_P,  BSP_DEL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT,  TD(A_ACUTE_ACCENT),  KC_S,  KC_D,  KC_F,  KC_G,          KC_H,  KC_J,  KC_K,  KC_L,  KC_SCLN,  TD(SQUOT_DQUOT),
    //---------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+---------+--------------|
        MOD_CTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                          TD(ENE_ENIE),  KC_M,  KC_COMM,  KC_DOT,  KC_SLSH,  KC_ESC,
    //---------+--------+--------+--------+--------+--------+--------|  |--------+--------+-----------+---------------+--------------+-------------------+--------|
                                            MOD_GUI, LOWER, KC_SPC,     KC_ENT, RAISE, KC_LALT
                                        //|--------------------------|  |--------------------------|
    ),

    [_LOWER] = LAYOUT(
    //|-----------------------------------------------------|                                              |-----------------------------------------------------|
        KC_F2, LALT(KC_F12),  LCTL(KC_SPC), LSFT(KC_F12), LALT(LSFT(KC_SLSH)), TD(SRCHFILES_SRCHREPOS),           SGUI(KC_2), COLOR_PICKER, SCREEN_CAPTURE, SCREEN_RECORDER, TD(APPS_WORKSPACES), TD(CREATEWSP_REMOVEWSP),
    //|--------+--------+--------+--------+--------+--------|                                              |--------+--------+--------+--------+--------+--------|
        TOGGLE_SIDEBAR_EDITOR, KC_LSFT, MOD_ALT, MOD_GUI, MOD_CTL, LGUI(LALT(LSFT(KC_H))),                 KC_LEFT, KC_DOWN, KC_UP, KC_RGHT,  SCREEN_CHANGE, APPS_SEARCH,
    //|--------+--------+--------+--------+--------+--------|                                              |--------+--------+--------+--------+-----i---+--------|
        KC_F12, LGUI(KC_B), LGUI(KC_F8), KC_F8, KC_F10, KC_F11,                                               SPLIT_SCREEN_HALF,   SPLIT_SCREEN_UPDOWN,    SPLIT_SCREEN_THIRD,  TD(MAXREST_CHDISPLAY),    TD(DESKTOPLEFT),    TD(DESKTOPRIGHT),
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            MOD_GUI, LOWER, KC_SPC,     KC_ENT, RAISE, KC_LALT
                                        //|--------------------------|  |--------------------------|
    ),

    [_RAISE] = LAYOUT(
    //|-----------------------------------------------------|                                         |-----------------------------------------------------|
        TD(DLR_EURO),  KC_AMPR,  KC_PIPE, KC_EXLM, LSFT(KC_SLSH), KC_HASH,                               KC_COMM, KC_P7, KC_P8, KC_P9, KC_PLUS, KC_MINS,
    //|--------+--------+--------+--------+--------+--------|                                         |--------+--------+--------+--------+--------+--------|
        KC_EQL, TD(ROUNDBRCKTS), TD(CURLYBRCKTS), TD(SQUAREBRCKTS), TD(ANGLEBRCKTS), LSFT(KC_SCLN),      KC_DOT, KC_P4, KC_P5, KC_P6, KC_ASTR, KC_SLSH,
    //|--------+--------+--------+--------+--------+--------|                                         |--------+--------+--------+--------+--------+--------|
        KC_GRV, KC_AT, KC_UNDS, TD(TILD_BSLS), TD(EXC_QUESTION), TD(MORDINAL_FORDINAL),                  KC_P0, KC_P1, KC_P2, KC_P3, KC_CIRC, KC_PERC,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            MOD_GUI, LOWER, KC_SPC,     KC_ENT, RAISE, KC_LALT
                                        //|--------------------------|  |--------------------------|
    ),

    [_ADJUST] = LAYOUT(
    //|-----------------------------------------------------|                    |-----------------------------------------------------|
        KC_F1, KC_F2,  KC_F3, KC_F4, KC_F5, KC_F6,                                  KC_F7, KC_F8,  KC_F9,   KC_F10,   KC_F11,   KC_F12,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        UC_MOD, KC_MEDIA_NEXT_TRACK, KC_MEDIA_PLAY_PAUSE, KC_VOLU, KC_BRIU, RGB_MOD,        RGB_VAI, RGB_SPI,  MACRO_1,  MACRO_3,   MACRO_5,   MACRO_7,  
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        SLEEP, KC_MEDIA_PREV_TRACK, HYPR(KC_A), KC_VOLD, KC_BRID, RGB_TOG,        RGB_VAD, RGB_SPD,  MACRO_2,  MACRO_4,   MACRO_6,   MACRO_8,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            XXXXXXX, LOWER, XXXXXXX,     XXXXXXX, RAISE, XXXXXXX 
                                        //|--------------------------|  |--------------------------|
    ),
};


/* --------------------------------------- TAP DANCE HOLD FUNCTION ----------------------------------------- */

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }


/* --------------------------------------- TAP DANCE HOLD MAXRESTCHDISPLAY FUNCTION ----------------------------------------- */

typedef struct {
    uint16_t tap_macos;
    uint16_t tap_macos_maximized;
    uint16_t hold_macos;
    uint16_t held_macos;
    uint16_t tap_win;
    uint16_t tap_win_maximized;
    uint16_t hold_win;
    uint16_t held_win;
} tap_dance_tap_hold_maxrestchdisplay_os_t;

void tap_dance_tap_hold_maxrestchdisplay_os_finished(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_maxrestchdisplay_os_t *tap_hold = (tap_dance_tap_hold_maxrestchdisplay_os_t *)user_data;

    bool is_unicode_macos = get_unicode_input_mode() == UNICODE_MODE_MACOS;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            if (is_unicode_macos) {
                register_code16(tap_hold->hold_macos);
                tap_hold->held_macos = tap_hold->hold_macos;
            } else {
                register_code16(tap_hold->hold_win);
                tap_hold->held_win = tap_hold->hold_win;
            }
        } else {
             if (screen_is_maximized) {
                if (is_unicode_macos) {
                    register_code16(tap_hold->tap_macos_maximized);
                    tap_hold->held_macos = tap_hold->tap_macos_maximized;
                } else {
                    register_code16(tap_hold->tap_win_maximized);
                    tap_hold->held_win = tap_hold->tap_win_maximized;
                }
            } else {
                if (is_unicode_macos) {
                    register_code16(tap_hold->tap_macos);
                    tap_hold->held_macos = tap_hold->tap_macos;
                } else {
                    register_code16(tap_hold->tap_win);
                    tap_hold->held_win = tap_hold->tap_win;
                }
            }
        }
    }
}

void tap_dance_tap_hold_maxrestchdisplay_os_reset(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_maxrestchdisplay_os_t *tap_hold = (tap_dance_tap_hold_maxrestchdisplay_os_t *)user_data;

    if (get_unicode_input_mode() == UNICODE_MODE_MACOS) {
        if (tap_hold->held_macos) {
            unregister_code16(tap_hold->held_macos);
            tap_hold->held_macos = 0;
        }
    } else {
        if (tap_hold->held_win) {
            unregister_code16(tap_hold->held_win);
            tap_hold->held_win = 0;
        }
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD_MAXRESTCHDISPLAY_OS(tap_macos, tap_macos_maximized, hold_macos, tap_win, tap_win_maximized, hold_win) \
    { .fn = {NULL, tap_dance_tap_hold_maxrestchdisplay_os_finished, tap_dance_tap_hold_maxrestchdisplay_os_reset}, .user_data = (void *)&((tap_dance_tap_hold_maxrestchdisplay_os_t){tap_macos, tap_macos_maximized, hold_macos, 0, tap_win, tap_win_maximized, hold_win, 0}), }

/* --------------------------------------- TAP DANCE HOLD UNICODDE FUNCTION ----------------------------------------- */


typedef struct {
    uint16_t tap;
    char *hold;
    char *hold_shifted;
    uint16_t held;
} tap_dance_tap_hold_unicode_t;

void tap_dance_tap_hold_unicode_finished(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_unicode_t *tap_hold = (tap_dance_tap_hold_unicode_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            if (is_shift_hold) {
                send_unicode_string(tap_hold->hold_shifted);
            } else {
                send_unicode_string(tap_hold->hold);
            }
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_unicode_reset(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_unicode_t *tap_hold = (tap_dance_tap_hold_unicode_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD_UNICODE(tap, hold, hold_shifted) \
    { .fn = {NULL, tap_dance_tap_hold_unicode_finished, tap_dance_tap_hold_unicode_reset}, .user_data = (void *)&((tap_dance_tap_hold_unicode_t){tap, hold, hold_shifted, 0}), }



/* --------------------------------------- TAP DANCE HOLD OTHER FUNCTION ----------------------------------------- */



typedef struct {
    char *tap;
    char *hold;
    uint16_t held;
} tap_dance_tap_hold_others_t;

void tap_dance_tap_hold_others_finished(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_others_t *tap_hold = (tap_dance_tap_hold_others_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            send_unicode_string(tap_hold->hold);
        } else {
            send_unicode_string(tap_hold->tap);
        }
    }
}

void tap_dance_tap_hold_others_reset(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_unicode_t *tap_hold = (tap_dance_tap_hold_unicode_t *)user_data;

    if (tap_hold->held) {
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD_OTHERS(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_others_finished, tap_dance_tap_hold_others_reset}, .user_data = (void *)&((tap_dance_tap_hold_others_t){tap, hold, 0}), }

/* --------------------------------------- TAP DANCE HOLD OS ----------------------------------------- */


typedef struct {
    uint16_t tap_macos;
    uint16_t hold_macos;
    uint16_t held_macos;
    uint16_t tap_win;
    uint16_t hold_win;
    uint16_t held_win;
} tap_dance_tap_hold_os_t;

void tap_dance_tap_hold_os_finished(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_os_t *tap_hold = (tap_dance_tap_hold_os_t *)user_data;

    bool is_unicode_macos = get_unicode_input_mode() == UNICODE_MODE_MACOS;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            if(is_unicode_macos) {
                register_code16(tap_hold->hold_macos);
                tap_hold->held_macos = tap_hold->hold_macos;
            } else {
                register_code16(tap_hold->hold_win);
                tap_hold->held_win = tap_hold->hold_win;
            }
        } else {
            if(is_unicode_macos) {
                register_code16(tap_hold->tap_macos);
                tap_hold->held_macos = tap_hold->tap_macos;
            } else {
                register_code16(tap_hold->tap_win);
                tap_hold->held_win = tap_hold->tap_win;
            }
        }
    }
}

void tap_dance_tap_hold_os_reset(qk_tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_os_t *tap_hold = (tap_dance_tap_hold_os_t *)user_data;

    if(get_unicode_input_mode() == UNICODE_MODE_MACOS) {
        if (tap_hold->held_macos) {
            unregister_code16(tap_hold->held_macos);
            tap_hold->held_macos = 0;
        }
    } else {
        if (tap_hold->held_win) {
            unregister_code16(tap_hold->held_win);
            tap_hold->held_win = 0;
        }
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD_OS(tap_macos, hold_macos, tap_win, hold_win) \
    { .fn = {NULL, tap_dance_tap_hold_os_finished, tap_dance_tap_hold_os_reset}, .user_data = (void *)&((tap_dance_tap_hold_os_t){tap_macos, hold_macos, 0, tap_win, hold_win, 0}), }


/* --------------------------------------- TAP DANCE DEFINITIONS ----------------------------------------- */


qk_tap_dance_action_t tap_dance_actions[] = {
    [TILD_BSLS] = ACTION_TAP_DANCE_TAP_HOLD(KC_TILD, KC_BSLS),
    [ROUNDBRCKTS] = ACTION_TAP_DANCE_TAP_HOLD(KC_LPRN, LSFT(KC_RPRN)),
    [CURLYBRCKTS] = ACTION_TAP_DANCE_TAP_HOLD(LSFT(KC_LBRC), LSFT(KC_RBRC)),
    [SQUAREBRCKTS] = ACTION_TAP_DANCE_TAP_HOLD(KC_LBRC, KC_RBRC),
    [ANGLEBRCKTS] = ACTION_TAP_DANCE_TAP_HOLD(LSFT(KC_COMM), LSFT(KC_DOT)),
    [SQUOT_DQUOT] = ACTION_TAP_DANCE_TAP_HOLD(KC_QUOT,LSFT(KC_QUOT)), 
    [DESKTOPLEFT] = ACTION_TAP_DANCE_TAP_HOLD_OS(LCTL(KC_LEFT), MEH(KC_LEFT), LGUI(LCTL(KC_LEFT)), LGUI(LCTL(KC_LEFT))),
    [DESKTOPRIGHT] = ACTION_TAP_DANCE_TAP_HOLD_OS(LCTL(KC_RGHT), MEH(KC_RGHT), LGUI(LCTL(KC_RGHT)), LGUI(LCTL(KC_RGHT))),
    [APPS_WORKSPACES] = ACTION_TAP_DANCE_TAP_HOLD_OS(SGUI(KC_A), LCTL(KC_UP), KC_LGUI, LGUI(KC_TAB)),
    [CREATEWSP_REMOVEWSP] = ACTION_TAP_DANCE_TAP_HOLD_OS(HYPR(KC_N), HYPR(KC_D), LGUI(LCTL(KC_D)), LGUI(LCTL(KC_F4))),
    [SRCHFILES_SRCHREPOS] = ACTION_TAP_DANCE_TAP_HOLD(LCTL(KC_P), LCTL(KC_R)),
    [MAXREST_CHDISPLAY] = ACTION_TAP_DANCE_TAP_HOLD_MAXRESTCHDISPLAY_OS(LCA(KC_ENT), LCA(KC_BSPC), LCAG(KC_RGHT), LGUI(KC_UP), LGUI(KC_DOWN), LSG(KC_RGHT)),    

    // Spanish Characters
    [MORDINAL_FORDINAL] = ACTION_TAP_DANCE_TAP_HOLD_OTHERS("º", "ª"),    
    [EXC_QUESTION] = ACTION_TAP_DANCE_TAP_HOLD_OTHERS("¡", "¿"), 
    [DLR_EURO] = ACTION_TAP_DANCE_TAP_HOLD_UNICODE(KC_DLR, "€", "€"),    
    [ENE_ENIE] = ACTION_TAP_DANCE_TAP_HOLD_UNICODE(KC_N, "ñ", "Ñ"),    
    [A_ACUTE_ACCENT] = ACTION_TAP_DANCE_TAP_HOLD_UNICODE(KC_A, "á", "Á"),    
    [E_ACUTE_ACCENT] = ACTION_TAP_DANCE_TAP_HOLD_UNICODE(KC_E, "é", "É"),    
    [I_ACUTE_ACCENT] = ACTION_TAP_DANCE_TAP_HOLD_UNICODE(KC_I, "í", "Í"),    
    [O_ACUTE_ACCENT] = ACTION_TAP_DANCE_TAP_HOLD_UNICODE(KC_O, "ó", "Ó"),    
    [U_ACUTE_ACCENT] = ACTION_TAP_DANCE_TAP_HOLD_UNICODE(KC_U, "ú", "Ú"),    
};

/* --------------------------------------- RGB CONFIG ----------------------------------------- */

void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
    if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
        layer_on(layer3);
    } else {
        layer_off(layer3);
    }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
        RGB_current_mode = rgblight_config.mode;
    #endif
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if(layer_state_is(_ADJUST)) {
        for (uint8_t i = led_min; i < led_max; i++) {
            switch(get_unicode_input_mode()) {
                case UNICODE_MODE_WINCOMPOSE:
                    rgb_matrix_set_color(i, 0, 0, 255);
                break;
                case UNICODE_MODE_LINUX:
                
                    rgb_matrix_set_color(i, 255, 0, 0);
                break;
                default:
                    rgb_matrix_set_color(i, 0, 255, 0);
                break;
            }
        } 
        return true;
    }
    return false;
}



/* --------------------------------------- OLED CONFIG ----------------------------------------- */

#ifdef OLED_ENABLE
    oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

    void render_space(void) {
        oled_write_P(PSTR("     "), false);                                 
    }

    void render_mod_status_gui_alt(uint8_t modifiers) {
        static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
        static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
        static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};
        static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};

        static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
        static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
        static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};
        static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

        static const char PROGMEM off_off_1[] = {0xc5, 0};
        static const char PROGMEM off_off_2[] = {0xc6, 0};
        static const char PROGMEM on_off_1[] = {0xc7, 0};
        static const char PROGMEM on_off_2[] = {0xc8, 0};
        static const char PROGMEM off_on_1[] = {0xc9, 0};
        static const char PROGMEM off_on_2[] = {0xca, 0};
        static const char PROGMEM on_on_1[] = {0xcb, 0};
        static const char PROGMEM on_on_2[] = {0xcc, 0};

        if (modifiers & MOD_MASK_GUI) {
            oled_write_P(gui_on_1, false);
        } else {
            oled_write_P(gui_off_1, false);
        }

        if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
            oled_write_P(on_on_1, false);
        } else if (modifiers & MOD_MASK_GUI) {
            oled_write_P(on_off_1, false);
        } else if (modifiers & MOD_MASK_ALT) {
            oled_write_P(off_on_1, false);
        } else {
            oled_write_P(off_off_1, false);
        }

        if (modifiers & MOD_MASK_ALT) {
            oled_write_P(alt_on_1, false);
        } else {
            oled_write_P(alt_off_1, false);
        }

        if (modifiers & MOD_MASK_GUI) {
            oled_write_P(gui_on_2, false);
        } else {
            oled_write_P(gui_off_2, false);
        }

        if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
            oled_write_P(on_on_2, false);
        } else if (modifiers & MOD_MASK_GUI) {
            oled_write_P(on_off_2, false);
        } else if (modifiers & MOD_MASK_ALT) {
            oled_write_P(off_on_2, false);
        } else {
            oled_write_P(off_off_2, false);
        }

        if (modifiers & MOD_MASK_ALT) {
            oled_write_P(alt_on_2, false);
        } else {
            oled_write_P(alt_off_2, false);
        }
    }

    void render_mod_status_ctrl_shift(uint8_t modifiers) {
        static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
        static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
        static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};
        static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

        static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
        static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
        static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};
        static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

        static const char PROGMEM off_off_1[] = {0xc5, 0};
        static const char PROGMEM off_off_2[] = {0xc6, 0};
        static const char PROGMEM on_off_1[] = {0xc7, 0};
        static const char PROGMEM on_off_2[] = {0xc8, 0};
        static const char PROGMEM off_on_1[] = {0xc9, 0};
        static const char PROGMEM off_on_2[] = {0xca, 0};
        static const char PROGMEM on_on_1[] = {0xcb, 0};
        static const char PROGMEM on_on_2[] = {0xcc, 0};

        if (modifiers & MOD_MASK_CTRL) {
            oled_write_P(ctrl_on_1, false);
        } else {
            oled_write_P(ctrl_off_1, false);
        }

        if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
            oled_write_P(on_on_1, false);
        } else if (modifiers & MOD_MASK_CTRL) {
            oled_write_P(on_off_1, false);
        } else if (modifiers & MOD_MASK_SHIFT) {
            oled_write_P(off_on_1, false);
        } else {
            oled_write_P(off_off_1, false);
        }

        if (modifiers & MOD_MASK_SHIFT) {
            oled_write_P(shift_on_1, false);
        } else {
            oled_write_P(shift_off_1, false);
        }

        if (modifiers & MOD_MASK_CTRL) {
            oled_write_P(ctrl_on_2, false);
        } else {
            oled_write_P(ctrl_off_2, false);
        }

        if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
            oled_write_P(on_on_2, false);
        } else if (modifiers & MOD_MASK_CTRL) {
            oled_write_P(on_off_2, false);
        } else if (modifiers & MOD_MASK_SHIFT) {
            oled_write_P(off_on_2, false);
        } else {
            oled_write_P(off_off_2, false);
        }

        if (modifiers & MOD_MASK_SHIFT) {
            oled_write_P(shift_on_2, false);
        } else {
            oled_write_P(shift_off_2, false);
        }
    }

    void render_logo(void) {
        static const char PROGMEM corne_logo[] = {
            0x80, 0x81, 0x82, 0x83, 0x84,
            0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
            0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};

        oled_write_P(corne_logo, false);
    }

    void render_name(void) {
        static const char PROGMEM luis[] = {
            0x40, 0x41, 0x42, 0x43, 0x44,
            0x60, 0x61, 0x62, 0x63, 0x64, 0};

        oled_write_P(luis, false);
    }

    void render_layer_state(void) {
        static const char PROGMEM default_layer[] = {
            0x20, 0x94, 0x95, 0x96, 0x20,
            0x20, 0xb4, 0xb5, 0xb6, 0x20,
            0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
        static const char PROGMEM raise_layer[] = {
            0x20, 0x97, 0x98, 0x99, 0x20,
            0x20, 0xb7, 0xb8, 0xb9, 0x20,
            0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
        static const char PROGMEM lower_layer[] = {
            0x20, 0x9a, 0x9b, 0x9c, 0x20,
            0x20, 0xba, 0xbb, 0xbc, 0x20,
            0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
        static const char PROGMEM adjust_layer[] = {
            0x20, 0x9d, 0x9e, 0x9f, 0x20,
            0x20, 0xbd, 0xbe, 0xbf, 0x20,
            0x20, 0xdd, 0xde, 0xdf, 0x20, 0};
        if (layer_state_is(_ADJUST)) {
            oled_write_P(adjust_layer, false);
        } else if (layer_state_is(_LOWER)) {
            oled_write_P(lower_layer, false);
        } else if (layer_state_is(_RAISE)) {
            oled_write_P(raise_layer, false);
        } else {
            oled_write_P(default_layer, false);
        }
    }

    bool oled_task_user(void) {
        render_logo();
        render_space();
        render_name();
        render_space();
        render_layer_state();
        render_space();
        render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
        render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());
        return false;
    }
#endif


/* --------------------------------------- PROCESS RECORD ----------------------------------------- */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    qk_tap_dance_action_t *action;

    static uint8_t saved_mods   = 0;
    bool is_unicode_macos = get_unicode_input_mode() == UNICODE_MODE_MACOS;

    if (record->event.pressed) {
        #ifdef OLED_ENABLE
                oled_timer = timer_read32();
        #endif
    }

    switch (keycode) {
        case BSP_DEL:
            if (record->event.pressed) {
                saved_mods = get_mods() & MOD_MASK_SHIFT;

                if (saved_mods == MOD_MASK_SHIFT) {
                    register_code(KC_DEL);
                } else if (saved_mods) {
                    del_mods(saved_mods);  
                    register_code(KC_DEL);
                    add_mods(saved_mods);
                } else {
                    register_code(KC_BSPC);
                }
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_BSPC);
            }
        return false;
        case KC_LSFT:
            if (record->event.pressed) {
                is_shift_hold = true;
                register_code16(KC_LSFT);
            } else {
                is_shift_hold = false;
                unregister_code16(KC_LSFT);
            }
        return false;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            }
        return false;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
            }
        return false;
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
        return false;
        case MOD_GUI: // posición GUI
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(KC_LGUI);
                } else {
                    register_code16(KC_LCTL);
                }
            } else {
                if(is_unicode_macos) {
                    unregister_code16(KC_LGUI);
                } else {
                    unregister_code16(KC_LCTL);
                }
            }
        return false;
        case MOD_ALT: // posición Alt
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(KC_LALT);
                } else {
                    register_code16(KC_LCTL);
                }
            } else {
                if(is_unicode_macos) {
                    unregister_code16(KC_LALT);
                } else {
                    unregister_code16(KC_LCTL);
                }
            }
        return false;
        case MOD_CTL: // posición Ctrl
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(KC_LCTL);
                } else {
                    register_code16(KC_LGUI);
                }
            } else {
                if(is_unicode_macos) {
                    unregister_code16(KC_LCTL);
                } else {
                    unregister_code16(KC_LGUI);
                }
            }
        return false;
        case MACRO_1:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_1);
            } 
        return false;
        case MACRO_2:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_2);
            } 
        return false;
        case MACRO_3:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_3);
            } 
        return false;
        case MACRO_4:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_4);
            } 
        return false;
        case MACRO_5:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_5);
            } 
        return false;
        case MACRO_6:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_6);
            } 
        return false;
        case MACRO_7:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_7);
            } 
        return false;
        case MACRO_8:
            if (record->event.pressed) {
                SEND_STRING(PRIVATE_MACRO_8);
            } 
        return false;
        case SLEEP:
            if (record->event.pressed) {
                register_code16(KC_SLEP);
            } else {
                unregister_code16(KC_SLEP);

            }
        return false;
        case TOGGLE_SIDEBAR_EDITOR:
            if (record->event.pressed) {
                if (is_editor_focus) {
                    if(is_unicode_macos) {
                        register_code16(LGUI(KC_0));
                        unregister_code16(LGUI(KC_0));
                    } else {
                        register_code16(LCTL(KC_0));
                        unregister_code16(LCTL(KC_0));
                    }
                } else {
                    register_code16(LSA(KC_R));
                    unregister_code16(LSA(KC_R));
                }
                is_editor_focus = !is_editor_focus;
            } 
        return false;
        case SPLIT_SCREEN_HALF:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    switch (split_screen_half_counter) {
                        case 1:
                            register_code16(LCA(KC_Q));
                            unregister_code16(LCA(KC_Q));
                        break;
                        case 2:
                            register_code16(LCA(KC_W));
                            unregister_code16(LCA(KC_W));
                        break;
                        case 3:
                            register_code16(LCA(KC_A));
                            unregister_code16(LCA(KC_A));
                        break;
                        case 4:
                            register_code16(LCA(KC_S));
                            unregister_code16(LCA(KC_S));
                        break;
                    }
                    if (split_screen_half_counter < 4) {
                        split_screen_half_counter++;
                    } else {
                        split_screen_half_counter = 1;
                    }
                }
            } 
        return false;
        case SPLIT_SCREEN_UPDOWN:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    switch (split_screen_updown_counter) {
                        case 1:
                            register_code16(LCA(KC_U));
                            unregister_code16(LCA(KC_U));
                        break;
                        case 2:
                            register_code16(LCA(KC_I));
                            unregister_code16(LCA(KC_I));
                        break;
                        case 3:
                            register_code16(LCA(KC_J));
                            unregister_code16(LCA(KC_J));
                        break;
                        case 4:
                            register_code16(LCA(KC_K));
                            unregister_code16(LCA(KC_K));
                        break;
                    }
                    if (split_screen_updown_counter < 4) {
                        split_screen_updown_counter++;
                    } else {
                        split_screen_updown_counter = 1;
                    }
                }
            } 
        return false;
        case SPLIT_SCREEN_THIRD:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    switch (split_screen_third_counter) {
                        case 1:
                            register_code16(LCA(KC_D));
                            unregister_code16(LCA(KC_D));
                        break;
                        case 2:
                            register_code16(LCA(KC_E));
                            unregister_code16(LCA(KC_E));
                        break;
                        case 3:
                            register_code16(LCA(KC_F));
                            unregister_code16(LCA(KC_F));
                        break;
                        case 4:
                            register_code16(LCA(KC_G));
                            unregister_code16(LCA(KC_G));
                        break;
                        case 5:
                            register_code16(LCA(KC_T));
                            unregister_code16(LCA(KC_T));
                        break;
                        case 6:
                            register_code16(LCA(KC_H));
                            unregister_code16(LCA(KC_H));
                        break;
                    }
                    if (split_screen_third_counter < 6) {
                        split_screen_third_counter++;
                    } else {
                        split_screen_third_counter = 1;
                    }
                }
            } 
        return false;
        case COLOR_PICKER:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(SGUI(KC_7));
                } else {
                    register_code16(SGUI(KC_C));
                }
            } else {
                if(is_unicode_macos) {
                    unregister_code16(SGUI(KC_7));
                } else {
                    unregister_code16(SGUI(KC_C));
                }
            }
        return false;
        case SCREEN_CAPTURE:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(LCAG(KC_O));
                } else {
                    register_code16(SGUI(KC_S));
                }
            } else {
                if(is_unicode_macos) {
                    unregister_code16(LCAG(KC_O));
                } else {
                    unregister_code16(SGUI(KC_S));
                }
            }
        return false;
        case SCREEN_RECORDER:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(SGUI(KC_5));
                } else {
                    register_code16(SGUI(KC_R));
                }
            } else {
                if(is_unicode_macos) {
                    unregister_code16(SGUI(KC_5));
                } else {
                    unregister_code16(SGUI(KC_R));
                }
            }
        return false;
        case SCREEN_CHANGE:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(LGUI(KC_TAB));
                } else {
                    register_code16(KC_LALT);
                    register_code16(KC_TAB);
                }
                screen_change_active = true;
            } else {
                if(is_unicode_macos) {
                    unregister_code16(LGUI(KC_TAB));
                } else {
                    unregister_code16(KC_TAB);
                }
            }
        return false;
        case APPS_SEARCH:
            if (record->event.pressed) {
                if(is_unicode_macos) {
                    register_code16(LGUI(KC_SPC));
                } else {
                    register_code16(LALT(KC_SPC));
                }
            } else {
                if(is_unicode_macos) {
                    unregister_code16(LGUI(KC_SPC));
                } else {
                    unregister_code16(LALT(KC_SPC));
                }
            }
        return false;
        case RGBRST:
            #ifdef RGBLIGHT_ENABLE
                if (record->event.pressed) {
                    eeconfig_update_rgblight_default();
                    rgblight_enable();
                    RGB_current_mode = rgblight_config.mode;
                }
            #endif
            #ifdef RGB_MATRIX_ENABLE
                if (record->event.pressed) {
                    eeconfig_update_rgb_matrix_default();
                    rgb_matrix_enable();
                }
            #endif
        return false;
        case TD(TILD_BSLS):
        case TD(ROUNDBRCKTS):
        case TD(CURLYBRCKTS):
        case TD(SQUAREBRCKTS):
        case TD(ANGLEBRCKTS):
        case TD(SQUOT_DQUOT):
        case TD(SRCHFILES_SRCHREPOS):
        case TD(DLR_EURO):
        case TD(ENE_ENIE):
        case TD(A_ACUTE_ACCENT):
        case TD(E_ACUTE_ACCENT):
        case TD(I_ACUTE_ACCENT):
        case TD(O_ACUTE_ACCENT):
        case TD(U_ACUTE_ACCENT):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
        break;
        case TD(MAXREST_CHDISPLAY):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_maxrestchdisplay_os_t *tap_hold = (tap_dance_tap_hold_maxrestchdisplay_os_t *)action->user_data;
                if (screen_is_maximized) {
                    if(is_unicode_macos) {
                        tap_code16(tap_hold->tap_macos_maximized);
                        tap_hold->held_macos = tap_hold->tap_macos_maximized;
                    } else {
                        tap_code16(tap_hold->tap_win_maximized);
                        tap_hold->held_win = tap_hold->tap_win_maximized;
                    }
                } else {
                    if(is_unicode_macos) {
                        tap_code16(tap_hold->tap_macos);
                        tap_hold->held_macos = tap_hold->tap_macos;
                    } else {
                        tap_code16(tap_hold->tap_win);
                        tap_hold->held_win = tap_hold->tap_win;
                    }
                }
                screen_is_maximized = !screen_is_maximized;
            }
        break;
        case TD(DESKTOPLEFT):
        case TD(DESKTOPRIGHT):
        case TD(APPS_WORKSPACES):
        case TD(CREATEWSP_REMOVEWSP):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_os_t *tap_hold = (tap_dance_tap_hold_os_t *)action->user_data;
                if(is_unicode_macos) {
                    tap_code16(tap_hold->tap_macos);
                } else {
                    tap_code16(tap_hold->tap_win);
                }
            }
        break;
        case TD(MORDINAL_FORDINAL):
        case TD(EXC_QUESTION):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_others_t *tap_hold = (tap_dance_tap_hold_others_t *)action->user_data;
                send_unicode_string(tap_hold->tap);
            }
        break;
        case KC_ENT:
        case KC_ESC:
            if (screen_change_active) {
                unregister_code16(KC_LALT);
            }
    }
    return true;
}