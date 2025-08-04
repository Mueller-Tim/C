/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module adc.
 * --
 * --
 * -- $Id: adc.c 5610 2023-02-03 09:22:02Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "adc.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE  (0x00000400)

/* Configuring pin for ADC: PF.6 */
#define GPIOF_MODER_ANALOG (0x3 << 12)


/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

uint16_t filter_array[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t filter_index = 0;

/// END: To be programmed


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void adc_init(void)
{
    /* Enable peripheral clocks */
    RCC->AHB1ENR |= PERIPH_GPIOF_ENABLE;
    RCC->APB2ENR |= PERIPH_ADC3_ENABLE;

    /* Configure PF.6 as input */
    GPIOF->MODER |= GPIOF_MODER_ANALOG;

    /* ADC common init */
    ADCCOM->CCR = 0;        // TSVREF = '0'    -> Temp sensor disabled
                            // VBATE = '0'     -> VBAT disabled
                            // ADCPRE = '00'   -> APB2 / 2 -> 21 MHz
                            // DMA = '00'      -> DMA disabled
                            // DELAY = '0000'  -> Delay 5 cycles
                            // MULTI = '00000' -> ADC independent mode

    /* Configure ADC3 */
    
    /// STUDENTS: To be programmed
		ADC3->CR1 = 0x0;					// 12 bit reselution, no scan
		ADC3->CR2 = 0x1;					// single conv., enable ADC, right align
	
		ADC3->SMPR1 = 0x0;
		ADC3->SMPR2 = (0x2 << (3*4));	// sample time = 28 cylces (=binary code 010) for channel 6
		
		ADC3->SQR1 = 0x0;					// L = '000' -> sequence length: 1
		ADC3->SQR2 = 0x0;
		ADC3->SQR3 = 0x4;					// ch6 is first in sequence

    /// END: To be programmed
}


/*
 *  See header file
 */
uint16_t adc_get_value(adc_resolution_t resolution)
{
    uint16_t adc_value;

    /// STUDENTS: To be programmed
	
		ADC3->CR1 &= 0xFcFFFFFF;
		ADC3->CR1 |= resolution;
		
		ADC3->CR2 |= (0x1 <<30); 			//start conversion
		while(!(ADC3->SR & 0x2)){ 		//wait while conversion not finished
		}	
		adc_value = ADC3->DR;					// show on 7-segement display



    /// END: To be programmed

    return adc_value;
}


/*
 *  See header file
 */
uint16_t adc_filter_value(uint16_t adc_value)
{
    uint16_t filtered_value = 0;
		uint32_t sum = 0;

    /// STUDENTS: To be programmed
		filter_array[filter_index] = adc_value;
	
		filter_index = (filter_index + 1) % 16;
		
		for(uint8_t i = 0; i < 16; i++){
			sum += filter_array[i];
		}	
		
		filtered_value = sum / 16;

    /// END: To be programmed

    return filtered_value;
}
