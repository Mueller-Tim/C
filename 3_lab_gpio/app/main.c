/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab GPIO
 * -- Description : Configure and use GPIO port B as output and 
 * --               GPIO port A is input.
 * --               Lab version without additional hardware 
 * --               except for three wires.
 * --
 * -- $Id: main.c ostt $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include <reg_stm32f4xx.h>

#include "reg_ctboard.h"
#include "gpiocheck.h"

/* user macros */

#define MASK_0_BITS  0x0000
#define MASK_1_BITS  0x0001
#define MASK_2_BITS	 0x0003
#define MASK_3_BITS  0x0007
#define MASK_6_BITS  0x003F


void reset_GPIO(void);


/* ----------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------- */

int main(void)
{
    uint16_t data_gpio_in;              // use to read input values from gpio
    uint8_t data_dip_switch;            // use to read values from dip switches
		uint8_t state_gpio;

    GPIOA_ENABLE();                     // peripheral clk enable
    GPIOB_ENABLE();                     // peripheral clk enable
		
		//Reset the GPIOA and GPIOB to it's base value
		reset_GPIO();

    /* configure GPIO pins
     * clear register bits: GPIOx->xxxx &= ~(clear_mask << bit_pos);
     * set register bits:   GPIOx->xxxx |=  (set_value << bit_pos);
     * On GPIOA and GPIOB only pins 11 down to 0 are available to the user. 
     * Pins 15 down to 12 are used for system functions of the discovery board, 
     * e.g. connection of the debugger.
     * These pins must not be reconfigured. 
     * OTHERWISE THE DEBUGGER CANNOT BE USED ANY MORE!!!
     */
    /// STUDENTS: To be programmed

		// GPIO A Input -----------------------------------------------------
		
		GPIOA->MODER &= ~(MASK_6_BITS << 0);
		
		GPIOA->MODER |= (MASK_0_BITS << 0);
		
		GPIOA->PUPDR &= ~(MASK_6_BITS <<0);
		
		GPIOA->PUPDR |= (MASK_2_BITS <<1);
		
		// ------------------------------------------------------------------
		
		// GPIO B Output ----------------------------------------------------
		
		GPIOB->MODER &= ~(MASK_6_BITS <<0);
		
		GPIOB->MODER |= (MASK_1_BITS << 0);
		GPIOB->MODER |= (MASK_1_BITS << 2);
		GPIOB->MODER |= (MASK_1_BITS << 4);
		
		GPIOB->OTYPER &= ~(MASK_3_BITS <<0);
		
		GPIOB->OTYPER |= (MASK_2_BITS <<1);
		
		GPIOB->PUPDR &= ~(MASK_6_BITS <<0);
		
		GPIOB->PUPDR |= (MASK_1_BITS <<4);
		
		GPIOB->OSPEEDR &= ~(MASK_6_BITS <<0);
		
		GPIOB->OSPEEDR |= (MASK_1_BITS <<2);
		GPIOB->OSPEEDR |= (MASK_1_BITS <<5);
		
		
		// ------------------------------------------------------------------



    /// END: To be programmed
		
		/*Check if GPIO configuration is correct to prevent hardware damage*/
		state_gpio = check_GPIO();
		
		/*check if there is no error or just a port speed register error*/
		if(state_gpio != 0 && state_gpio != 7 && state_gpio != 8)
		{
			/*Reset config to prevent damage and ensure accessibilty by programmer*/
			reset_GPIO();
			/*wait until reprogramming*/
			while(1){}
		}
    while (1) {
        /* implement tasks 6.1 to 6.2 below */
        /// STUDENTS: To be programmed

				CT_LED->BYTE.LED15_8 = CT_DIPSW->BYTE.S15_8 & MASK_3_BITS;
			
				GPIOB->ODR = CT_DIPSW->BYTE.S15_8 & MASK_3_BITS;
				
				CT_LED->BYTE.LED7_0 = GPIOB->ODR & MASK_3_BITS;
			
				CT_LED->BYTE.LED23_16 = GPIOA->IDR & MASK_3_BITS;


        /// END: To be programmed

    }
}
void reset_GPIO(void)
{
    /* Reset GPIOA specific values */
    GPIOA->MODER = 0xa8000000;           // mode register
    GPIOA->OSPEEDR = 0x00000000;         // output speed register
    GPIOA->PUPDR = 0x64000000;           // pull resistor register
    GPIOA->OTYPER = 0x00000000;          // type register (pp / f. gate)
    GPIOA->AFRL = 0x00000000;
    GPIOA->AFRH = 0x00000000;
    GPIOA->ODR = 0x00000000;             // output data register
    
    /* Reset GPIOB specific values */
    GPIOB->MODER = 0x00000280;           // mode register
    GPIOB->OSPEEDR = 0x000000c0;         // output speed register
    GPIOB->PUPDR = 0x00000100;           // pull resistor register
    GPIOB->OTYPER = 0x00000000;          // type register (pp / f. gate)
    GPIOB->AFRL = 0x00000000;
    GPIOB->AFRH = 0x00000000;
    GPIOB->ODR = 0x00000000;             // output data register	
}
