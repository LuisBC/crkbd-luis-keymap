# CRKBD Luis Keymap

This directory contains a custom keymap for the Corne keyboard (CRKBD), designed for productivity, navigation, and quick access to system actions.

## Features

- Main QWERTY layer with additional layers for productivity
- Split keyboard support with state sync between halves
- RGB matrix enabled
- OLED enabled with a custom font
- Unicode support for macOS, Linux, and Windows Compose
- Tap Dance for accented characters and utility combinations
- Custom macros for emails, names, and frequently used text
- Window, desktop, split-screen, search, capture, and recording shortcuts

## Structure

- `keymap.c`: layout definition and action logic
- `config.h`: keyboard, RGB, OLED, and Unicode settings
- `rules.mk`: QMK module configuration
- `macros.example.h`: public template that must be renamed to `macros.h` before use
- `glcdfont.c`: custom font for the OLED

## Requirements

1. Install and configure QMK Firmware on your system.
2. Place this keymap inside the QMK tree, typically under:
   `keyboards/crkbd/keymaps/<keymap-name>/`
3. Build the firmware for your CRKBD hardware.

## Build and flash

In this setup, the recommended command to build and program the keyboard is:

```bash
qmk flash -kb crkbd -km luis-keymap -bl avrdude-split-left
```

This compiles the firmware for the `luis-keymap` layout on the `crkbd` keyboard and flashes it to the left half of the split using `avrdude`.

If you need to change the side or split half, adjust the `-bl` value according to your hardware configuration.

## Main layers

- `QWERTY`: base typing layer
- `LOWER`: shortcuts, navigation, windows, split screen, search, and controls
- `RAISE`: complementary functions
- `ADJUST`: RGB and keymap behavior adjustments

## Macros

Personal macros are stored in `macros.h`. To set it up locally, rename the template `macros.example.h` to `macros.h`, and if desired, replace the sample values with your own:

```bash
mv macros.example.h macros.h
```

Then edit the resulting file and update the text for each macro:

Example:

```c
#define PRIVATE_MACRO_1 "my-first-macro"
#define PRIVATE_MACRO_2 "custom-text"
```

## Customization

- Change accented characters or tap-dance combinations in `keymap.c`
- Adjust brightness, OLED timing, and RGB settings in `config.h`
- Modify shortcuts, layers, and macros to match your workflow
- Keep private data in `macros.h` and use `macros.example.h` as the shareable template

## Troubleshooting

- If the keyboard does not compile, check that the keymap path is inside the QMK tree
- If LEDs or OLED do not show, review `rules.mk` and `config.h`
- If macros or combos do not respond, check `process_record_user` in `keymap.c`

## Reference documentation

For more information about QMK, keymaps, layers, and build/flash tooling, see the official documentation:

- https://docs.qmk.fm/
- https://docs.qmk.fm/#/flashing

## Notes

This keymap is intended for daily use in work environments with multiple applications, desktops, and quick system actions. It is a highly personal configuration and can be adapted to your specific workflow and habits.
