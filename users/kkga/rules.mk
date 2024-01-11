VPATH += keyboards/gboards

COMBO_ENABLE = yes
MOUSEKEY_ENABLE = no

EXTRAFLAGS += -flto

SRC += kkga.c
SRC += features/oneshot.c
SRC += features/swapper.c
SRC += features/caps_word.c


# # Sparkfun Pro Micro RP2040 MCU settings
# PLATFORM_KEY := chibios
# MCU := RP2040
# BOARD := QMK_PM2040
# BOOTLOADER := rp2040
#
# # These are defaults based on what has been implemented for RP2040 boards
# SERIAL_DRIVER ?= vendor
# WS2812_DRIVER ?= vendor
# BACKLIGHT_DRIVER ?= software
