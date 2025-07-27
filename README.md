# STM8WS2812
STM8 microcrontroller SPI based library for WS2812 LED strip/panel

It uses the internal 16 MHz CPU clock and master SPI at 8MHz
to create the required timing for the WS2812 LED.

The 0 and 1 are constructed by sending respectively 0b01110000 (WS_1)
or 0b01111100  (WS_0) on the SPI data line MOSI (Port C6).

SCK is left unconnected.

STM8WS2812_send_led_single function sends a continuous
burst of 24 bits without reaching the RET timing (<50us).

STM8WS2812_send_led_rgb_array function sends a complete array
of RGB values for NB_LED LEDs with a final RET signal (0 during 100us)

Three examples for ST Visual Develop and Cosmic compiler
are provided for the STM8S003 by setting EXAMPLE_TEST (main.c) to:
 - 1 : X/Y line scan on a 8x8 panel
 - 2 : 4 quadrants RGB+white
 - 3 : r/g/b/w blink cycle

