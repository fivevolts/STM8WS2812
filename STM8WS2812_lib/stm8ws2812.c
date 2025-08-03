/**********************************************************
 * STM8WS1812 - WS2812 LED  Programming over SPI
 * 
 * File: STM8WS2812.c
 * Description: Main library / functions
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
#include <stdlib.h>
#include "stm8s.h"
#include "STM8WS2812.h"
#include "delay.h"

/* ------------------------------------------------
 * Side function to wait for the SPI and avoid overflow
 * ------------------------------------------------ */
void STM8WS2812_wait_spi_sr(void )
{
	while ((SPI->SR & (uint8_t)SPI_FLAG_TXE) == (uint8_t)RESET );
}


/* ------------------------------------------------
 * Program only one LED
 * ------------------------------------------------ */
void STM8WS2812_send_led_single(RGB_typedef rgb_led )
{
		// uint8_t value = 0;
		uint8_t value_r = rgb_led.r;
		uint8_t value_g = rgb_led.g;
		uint8_t value_b = rgb_led.b;
		//--
		SPI->DR = ( value_g & ( 1 << 7 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_g & ( 1 << 6 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_g & ( 1 << 5 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_g & ( 1 << 4 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_g & ( 1 << 3 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_g & ( 1 << 2 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_g & ( 1 << 1 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_g )              ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r & ( 1 << 7 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r & ( 1 << 6 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r & ( 1 << 5 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r & ( 1 << 4 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r & ( 1 << 3 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r & ( 1 << 2 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r & ( 1 << 1 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_r )              ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b & ( 1 << 7 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b & ( 1 << 6 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b & ( 1 << 5 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b & ( 1 << 4 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b & ( 1 << 3 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b & ( 1 << 2 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b & ( 1 << 1 ) ) ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();
		SPI->DR = ( value_b )              ? WS_1 : WS_0; STM8WS2812_wait_spi_sr();

		/* NO RET code here! This function is called multiple
		 * time for the entire LED strip/panel  */
}



/* ------------------------------------------------
 * Send the full LED strip/panel value from RGB array
 * ------------------------------------------------ */
void STM8WS2812_send_led_rgb_array(RGB_typedef *led_panel)
{
	uint8_t i = 0;
	for( i = 0; i < NB_LED ; i++)
	{
		STM8WS2812_send_led_single(led_panel[i]);
	}
	delay_10us(10); /* RET code 100us */

}



/* ------------------------------------------------
 * Switch off the LED strip/panel by sending all 0
 * ------------------------------------------------ */
void STM8WS2812_switchoff_all(void )
{
	uint8_t i = 0;
	uint8_t j = 0;

	for( i = 0; i < NB_LED ; i++)
	{
		for( j = 0; j < 24; j++)
		{
			SPI->DR = WS_0;
			while ((SPI->SR & (uint8_t)SPI_FLAG_TXE) == (uint8_t)RESET );
		}
	}

	delay_10us(10); /* RET code 100 us */
}


/* ------------------------------------------------
 * Plain Color Fill
 * ------------------------------------------------ */
void STM8WS2812_plain_color_fill(RGB_typedef rgb_led )
{
	uint8_t i = 0;
	uint8_t j = 0;

	for( i = 0; i < NB_LED ; i++)
	{
		STM8WS2812_send_led_single( rgb_led );
	}

	delay_10us(10); /* RET code 100 us */
}
