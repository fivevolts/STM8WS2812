/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */


#define _COSMIC_

#include <stdlib.h>
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "delay.h"
#include "STM8WS2812.h"


/*
 * Example tests:
 *  1 : X/Y line scan on a 8x8 panel
 *  2 : 4 quadrants RGB+white
 *  3 : r/g/b/w blink cycle
 *  4 : running dot
 */
#define EXAMPLE_TEST ((uint8_t)1)

/*
 * Peripheral initialisation functions
 */
void do_init_CLK(void )
{

  CLK_DeInit();

	CLK_HSICmd(ENABLE); 
  while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);

	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
   	DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	while (CLK_GetSYSCLKSource() != CLK_SOURCE_HSI);

  CLK_ClockSwitchCmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
	  DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);

}

void do_init_GPIO(void )
{
	GPIO_DeInit(GPIOC);
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void do_init_SPI(void )
{
	SPI_DeInit();
	SPI_Init(	SPI_FIRSTBIT_MSB, 
						SPI_BAUDRATEPRESCALER_2, 
						SPI_MODE_MASTER,
						SPI_CLOCKPOLARITY_LOW, 
						SPI_CLOCKPHASE_1EDGE, 
						SPI_DATADIRECTION_1LINE_TX, 
						SPI_NSS_SOFT, 0);
	
	SPI_Cmd(ENABLE);
}


// Return array index from row and column
uint8_t xy_to_idx( uint8_t row, uint8_t col)
{
    if (row % 2 == 0) {
        // Even rows: left to right
        return row * MAX_ROW + col;
    } else {
        // Odd rows: right to left
        return row * MAX_ROW + (MAX_COL - 1 - col);
    }
}


void main()
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t dec = 0;
	uint8_t row = 0;
	uint8_t col = 0;
	uint8_t c = 0;

	/*-------------------------------------------------
	 * Create the RGB array for NB_LED LEDs
	 * Initialise the array
	 *-------------------------------------------------*/
	RGB_typedef *led_panel = malloc(NB_LED * sizeof(RGB_typedef));

	i = 0;
	do
	{	
		led_panel[i].r = 0;
		led_panel[i].g = 0;
		led_panel[i].b = 0;
	} while (i++ != NB_LED);

	/*-------------------------------------------------
	 * Initialise peripheral
	 *-------------------------------------------------*/
	do_init_CLK();
	do_init_GPIO();
	do_init_SPI();


	/*-------------------------------------------------
	 * Switch off ALL NB_LED LEDs in the strip/panel
	 *-------------------------------------------------*/
	STM8WS2812_switchoff_all();


	while (1)
	{

		/*----------------------------------------------------
		 * X/Y line scan
		 *----------------------------------------------------*/
		if ( EXAMPLE_TEST == 1 )
		{
			do
			{	
				led_panel[i].r = 0;
				led_panel[i].g = 0;
				led_panel[i].b = 0;
			} while (i++ != NB_LED);
			if ( col == MAX_COL - 1 )
				dec = 1;
			if ( col == 0 )
				dec = 0;
			for( row = 0 ; row < MAX_ROW; row++)
			{
				led_panel[xy_to_idx(row, col)].r = 3;
				led_panel[xy_to_idx(row, col)].g = 2;
				led_panel[xy_to_idx(row, col)].b = 1;
				led_panel[xy_to_idx(col, row)].r = 3;
				led_panel[xy_to_idx(col, row)].g = 2;
				led_panel[xy_to_idx(col, row)].b = 1;
			}
			if ( dec == 1 )
				col--;
			else
				col++;

			delay_ms(10);
			STM8WS2812_send_led_rgb_array(led_panel);
	  }
		

		/*----------------------------------------------------
		 * RGBW quadrant
		 *----------------------------------------------------*/
		if ( EXAMPLE_TEST == 2 )
		{
			c = 3;
			// Q 1/4 : Red
			for( row = 0 ; row < MAX_ROW/2; row++)
				for( col = 0 ; col < MAX_COL/2; col++)
					led_panel[xy_to_idx(row, col)].r = c;
			// Q 2/4 : Green
			for( row = MAX_ROW/2 ; row < MAX_ROW; row++)
				for( col = 0 ; col < MAX_COL/2; col++)
					led_panel[xy_to_idx(row, col)].g = c;
			// Q 3/4 : Blue
			for( row = 0 ; row < MAX_ROW/2; row++)
				for( col = MAX_COL/2 ; col < MAX_COL; col++)
					led_panel[xy_to_idx(row, col)].b = c;
			/// Q 4/4 : White
			for( row = MAX_ROW/2 ; row < MAX_ROW; row++)
				for( col = MAX_COL/2 ; col < MAX_COL; col++)
				{
					led_panel[xy_to_idx(row, col)].r = c;
					led_panel[xy_to_idx(row, col)].g = c;
					led_panel[xy_to_idx(row, col)].b = c;
				}
				delay_ms(4);
				STM8WS2812_send_led_rgb_array(led_panel);
		}
		
		
		/*----------------------------------------------------
		 * r/g/b/w blink cycle
		 *----------------------------------------------------*/
		if ( EXAMPLE_TEST == 3 )
		{
			c = 1;
			i = 0;
			do
			{
				switch (dec)
				{
					case 0 :
						led_panel[i].r = 0;
						led_panel[i].g = 0;
						led_panel[i].b = 0;
					break;
					case 1 :
						led_panel[i].r = c;
						led_panel[i].g = 0;
						led_panel[i].b = 0;
					break;
					case 2 :
						led_panel[i].r = 0;
						led_panel[i].g = c;
						led_panel[i].b = 0;
					break;
					case 3 :
						led_panel[i].r = 0;
						led_panel[i].g = 0;
						led_panel[i].b = c;
					break;
					case 4 :
						led_panel[i].r = c;
						led_panel[i].g = c;
						led_panel[i].b = c;
					break;
				}
			} while (i++ != NB_LED);
			delay_ms(500);
			STM8WS2812_send_led_rgb_array(led_panel);
			dec++;
			if ( dec > 4 )
				dec = 0;
		}

		/*----------------------------------------------------
		 * Running dot
		 *----------------------------------------------------*/
		if ( EXAMPLE_TEST == 4 )
		{
			i = 0;
			do
			{
				led_panel[i].r = 0;
				led_panel[i].g = 0;
				led_panel[i].b = 0;
			} while (i++ != NB_LED);
			led_panel[j].r   = 1;
			led_panel[j].g   = 4;
			led_panel[j++].b = 5;
			
			
			delay_ms(50);
			STM8WS2812_send_led_rgb_array(led_panel);
			if ( j >= NB_LED )
				j = 0;
		}

	}
}



