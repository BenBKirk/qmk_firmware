MOUSEKEY_ENABLE = yes    # Mouse keys
RGBLIGHT_ENABLE = no    # Enable WS2812 RGB underlight.
OLED_ENABLE     = yes
# OLED_DRIVER     = SSD1306
LTO_ENABLE      = yes
COMBO_ENABLE    = yes

# Sparkfun Pro Micro RP2040 MCU settings
PLATFORM_KEY := chibios
MCU := RP2040
BOARD := QMK_PM2040
BOOTLOADER := rp2040

# These are defaults based on what has been implemented for RP2040 boards
SERIAL_DRIVER ?= vendor
WS2812_DRIVER ?= vendor
BACKLIGHT_DRIVER ?= software

# Enable trackpoint
PS2_ENABLE = yes
PS2_MOUSE_ENABLE = yes
# POINTING_DEVICE_ENABLE = yes
# POINTING_DEVICE_DRIVER = adns9800
PS2_DRIVER = vendor