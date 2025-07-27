/**********************************************************
 * STM8WS1812 - WS2812 LED Programming over SPI
 * 
 * File: STM8WS2812.h
 * Description: include file for library / functions
 * 
 * Author: fivevolts
 * Date: 2025-07-27
 * Version: 1.0.0
 * 
 * Copyright (c) 2025 fivevolts
 * 
 * This program is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation, either version 3 of the License, or any
 * later version.
 * 
 **********************************************************/


// Decoding stream for SPI equivalent to WS2812's 1 and 0
#define WS_1   ((uint8_t)0b01111100)
#define WS_0   ((uint8_t)0b01110000)
#define NB_LED ((uint8_t)64)


// Decoding stream for SPI equivalent to WS2812's 1 and 0
// const	uint8_t ws_1 = 0b01111100;
// const	uint8_t ws_0 = 0b01110000;
// RGB structuture 3 x 8 unsigned bits

typedef struct RGB_typedef {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RGB_typedef;



void STM8WS2812_send_led_single(RGB_typedef ) ;
void STM8WS2812_send_led_rgb_array(RGB_typedef * ) ;
void STM8WS2812_switchoff_all(void ) ;
void STM8WS2812_wait_spi_sr(void );
