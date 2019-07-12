ARDUINO_DIR  = /usr/share/arduino

TARGET       = main
ARDUINO_LIBS =
BOARD_TAG    = nano
#ISP_PROG     = avrispmkii

MCU          = atmega328p
F_CPU        = 16000000

# Avrdude code
ARDUINO_PORT = /dev/ttyUSB0
AVRDUDE_ARD_BAUDRATE = 57600
AVRDUDE_OPTS = -v
AVRDUDE_ARD_PROGRAMMER = arduino

include /usr/share/arduino/Arduino.mk

