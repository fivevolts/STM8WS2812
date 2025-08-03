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
 * Example tests (pick only one):
 *  1 : X/Y line scan on a 8x8 panel
 *  2 : 4 quadrants RGB+white
 *  3 : r/g/b/w blink cycle
 *  4 : running dot
 *  5 : Random comet
 *  6 : Light on, fade out
 *  7 : Random dimming
 */

// #define __EXAMPLE_1
// #define __EXAMPLE_2
// #define __EXAMPLE_3
// #define __EXAMPLE_4
// #define __EXAMPLE_5
// #define __EXAMPLE_6
#define __EXAMPLE_7



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
uint16_t xy_to_idx( uint16_t row, uint16_t col)
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
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t row = 0;
	uint16_t col = 0;
	uint8_t c = 0;
	uint8_t dec = 0;

	/*-------------------------------------------------
	 * Create the RGB array for NB_LED LEDs
	 * Initialise the array
	 *-------------------------------------------------*/
	RGB_typedef *led_panel = malloc( NB_LED * sizeof(RGB_typedef));
	RGB_typedef single_dot = {5, 2, 3 };

	i = 0;
	for( i = 0; i < NB_LED ; i++)
	{	
		led_panel[i].r = 0;
		led_panel[i].g = 0;
		led_panel[i].b = 0;
	}

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


	delay_ms(1000);


	STM8WS2812_plain_color_fill(single_dot);
	delay_ms(1000);


	while (1)
	{

		/*----------------------------------------------------
		 * X/Y line scan
		 *----------------------------------------------------*/
#ifdef __EXAMPLE_1
			for( i = 0; i < NB_LED ; i++)
			{	
				led_panel[i].r = 0;
				led_panel[i].g = 0;
				led_panel[i].b = 0;
			}
			if ( col == MAX_COL - 1)
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

			delay_ms(1);
			STM8WS2812_send_led_rgb_array(led_panel);
#endif
		

		/*----------------------------------------------------
		 * RGBW quadrant
		 *----------------------------------------------------*/
#ifdef __EXAMPLE_2
			c = 1;
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

			led_panel[5].r = 0;
			led_panel[5].g = 0;
			led_panel[5].b = 0;
		

			delay_ms(50);
			STM8WS2812_send_led_rgb_array(led_panel);
#endif
		
		
		/*----------------------------------------------------
		 * r/g/b/w blink cycle
		 *----------------------------------------------------*/
#ifdef __EXAMPLE_3
			c = 1;
			i = 0;
			for( i = 0; i < NB_LED ; i++)
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
			}
			delay_ms(500);
			STM8WS2812_send_led_rgb_array(led_panel);
			dec++;
			if ( dec > 4 )
				dec = 0;
#endif

		/*----------------------------------------------------
		 * Running dot
		 *----------------------------------------------------*/
#ifdef __EXAMPLE_4
			i = 0;
			for( i = 0; i < NB_LED ; i++)
			{
				led_panel[i].r = 0;
				led_panel[i].g = 0;
				led_panel[i].b = 0;
			}
			led_panel[j].r   = 1;
			led_panel[j].g   = 4;
			led_panel[j++].b = 5;
			
			
			delay_ms(50);
			STM8WS2812_send_led_rgb_array(led_panel);
			if ( j >= NB_LED )
				j = 0;
#endif

		/*----------------------------------------------------
		 * Random comet
		 *----------------------------------------------------*/
#ifdef __EXAMPLE_5
			
			// j = rand()%NB_LED;
			// led_panel[j].r   = 20;
			// led_panel[j].g   = 20;
			// led_panel[j].b =   20;

			// Row
			// Col

			do {
				switch(rand()%5)
				{
						case 1 : 
							if (row == MAX_ROW-1) row--;
							else                  row++;
						break;
						case 2 : 
							if (col == MAX_COL-1) col--;
							else                  col++;
						break;
						case 3 :
							if (row == 0) row++;
							else          row--;
						break;
						case 4 : 
							if (col == 0) col++;
							else          col--;
						break;
				}
			} while (led_panel[xy_to_idx(row, col)].r != 0);

			i = 0;
			for( i = 0; i < NB_LED ; i++)
			{
				if ( led_panel[i].r != 0 ) led_panel[i].r -= 2;
				if ( led_panel[i].g != 0 ) led_panel[i].g -= 2;
				if ( led_panel[i].b != 0 ) led_panel[i].b -= 2;
				
				/*led_panel[i].r >>= 1;
				led_panel[i].g >>= 1;
				led_panel[i].b >>= 1;
				*/
			}


			led_panel[xy_to_idx(row, col)].r   = 6;
			led_panel[xy_to_idx(row, col)].g   = 6;
			led_panel[xy_to_idx(row, col)].b   = 6;
		
			STM8WS2812_send_led_rgb_array(led_panel);
			delay_ms(1);
#endif

		/*----------------------------------------------------
		 *  Light on, fade out
		 *----------------------------------------------------*/
#ifdef __EXAMPLE_6
			
			dec = 20;
			if ( rand()%20 == 1 )
			{
				for( i = 0; i < NB_LED ; i++)
				{
					if ( led_panel[i].r != 0 ) led_panel[i].r -= 1;
					if ( led_panel[i].g != 0 ) led_panel[i].g -= 1;
					if ( led_panel[i].b != 0 ) led_panel[i].b -= 1;
				}
			}
		
			if ( rand()%50 == 1 )
			{
				do
				{
					j = rand()%NB_LED;
				} while ( led_panel[j].r != 0 && led_panel[j].g != 0 && led_panel[j].b != 0 );
				led_panel[j].r   = dec;
				led_panel[j].g   = dec;
				led_panel[j].b =   dec;
			}
		
			//delay_ms(1);
			STM8WS2812_send_led_rgb_array(led_panel);
#endif
		/*----------------------------------------------------
		 *  Random dimming
		 *----------------------------------------------------*/
#ifdef __EXAMPLE_7
			dec = 20;
			if ( rand()%4 == 1 )
			{
				j = rand()%NB_LED;
				if ( led_panel[j].r < dec ) led_panel[j].r++;
				j = rand()%NB_LED;
				if ( led_panel[j].g < dec ) led_panel[j].g++;
				j = rand()%NB_LED;
				if ( led_panel[j].b < dec ) led_panel[j].b++;
		} else {
				j = rand()%NB_LED;
				if ( led_panel[j].r != 0 ) led_panel[j].r--;
				j = rand()%NB_LED;
				if ( led_panel[j].g != 0 ) led_panel[j].g--;
				j = rand()%NB_LED;
				if ( led_panel[j].b != 0 ) led_panel[j].b--;
		}
		
		delay_ms(50);
		STM8WS2812_send_led_rgb_array(led_panel);
#endif



	}
}



