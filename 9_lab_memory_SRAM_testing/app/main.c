/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Application for testing external memory
 * --
 * -- $Id: main.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------
 */

#include <stdint.h>
#include "hal_rcc.h"
#include "hal_fmc.h"
#include "hal_ct_lcd.h"
#include "reg_ctboard.h"
#include "hal_ct_buttons.h"
#include "hal_ct_seg7.h"

#define NR_OF_DATA_LINES           8
#define NR_OF_ADDRESS_LINES       11
#define CHECKER_BOARD           0xAA
#define INVERSE_CHECKER_BOARD   0x55
#define WALKING_ONES_ADDRESS    0x7FF

/* Set-up the macros (#defines) for your test */
/// STUDENTS: To be programmed

#define BASE_ADDRESS 0x64000000


/// END: To be programmed

int main(void)
{
    hal_fmc_sram_init_t init;
    hal_fmc_sram_timing_t timing;
    
    /* add your required automatic (local) variables here */ 
    /// STUDENTS: To be programmed

		volatile uint8_t *data_bus_value = (volatile uint8_t *) BASE_ADDRESS;


    /// END: To be programmed

    init.address_mux = DISABLE;                             // setup peripheral
    init.type = HAL_FMC_TYPE_SRAM;
    init.width = HAL_FMC_WIDTH_8B;
    init.write_enable = ENABLE;

    timing.address_setup = 0xFF;                            // all in HCLK
                                                            // cycles
    timing.address_hold = 0xFF;
    timing.data_setup = 0xFF;

    hal_fmc_init_sram(HAL_FMC_SRAM_BANK2, init, timing);    // init external bus
                                                            // bank 2 (NE2)
                                                            // asynch
    
    /* Data Bus Test - Walking ONES test */
    /// STUDENTS: To be programmed

		for(uint8_t i = 0; i < NR_OF_DATA_LINES; i++){
			*data_bus_value = 1 << i;
			if(*data_bus_value != 1 << i){
				CT_LED->BYTE.LED7_0 |= 1 << i;
			}
		}


    /// END: To be programmed
    
    /* Address Bus Test 
     * (1)  Write default values
     *
     *      Write the memory at all the power of 2 addresses (including 0x0000)
     *      to the default value of CHECKER_BOARD
     *
     * (2)  Perform tests
     *
     *      Select one power of 2 addresses after the other as test_address 
     *      (starting from the highest all the way down to 0x0000) --> 
     *          - Write the memory at test_address to INVERSE_CHECKER_BOARD
     *          - For all the power of 2 addresses including 0x0000
     *              o Read the memory content
     *              o Verify that the read value is either
     *                  (a) equal to CHECKER_BOARD in case a different address 
     *                      than test_addressed has been read or
     *                  (b) equal to INVERSE_CHECKER_BOARD in case the address
     *                      at test_address has been read
     *              o Errors found shall be indicated on LED31--16
     */
    
    /// STUDENTS: To be programmed
		
		
		
		uint16_t failing_address_pattern = 0;
		uint16_t address = (uint16_t) 0x01 << NR_OF_ADDRESS_LINES;
		while(address) {
			address >>= 1;
			(*((uint8_t *)(0x64000000 + address))) = CHECKER_BOARD;
		}

		uint16_t test_address = (uint16_t) 0x01 << NR_OF_ADDRESS_LINES;
		while(test_address) {
			test_address >>= 1;
			
			(*((uint8_t *)(0x64000000 + test_address))) = INVERSE_CHECKER_BOARD;
			address = (uint16_t) 0x01 << NR_OF_ADDRESS_LINES;

			while(address) {
				address >>= 1;
				uint8_t read_value = (*((uint8_t *)(0x64000000 + address)));
				if(address == test_address && read_value != INVERSE_CHECKER_BOARD) {
					failing_address_pattern |= test_address;
				}
				if (address != test_address && read_value != CHECKER_BOARD) {
					failing_address_pattern |= test_address;
				}
			}
			(*((uint8_t *)(0x64000000 + test_address))) = CHECKER_BOARD;
		}
		
		CT_LED->HWORD.LED31_16 = failing_address_pattern;



		
		
		
		
     /// END: To be programmed
    
    /* Device Test 
     * (1) Fill the whole memory with known increment pattern.
     *          Address     Data
     *          0x000       0x01
     *          0x001       0x02
     *          .....       ....
     *          0x0FE       0xFF
     *          0x0FF       0x00
     *          0x100       0x01
     *          .....       ....
     *
     * (2) First test: Read back each location and check pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     *     Bitwise invert  the pattern in each location for the second test
     *
     * (3) Second test: Read back each location and check for new pattern.
     *     In case of error, write address with wrong data to 7-segment and
     *     wait for press on button T0.
     */
    /// STUDENTS: To be programmed


		uint8_t test = 0;
		for(test_address = 0; test_address < 0x800; test_address++) {
			test++;
			(*((uint8_t *)(0x64000000 + test_address))) = test;
		}
		
		test = 0;
		for(test_address = 0; test_address < 0x800; test_address++) {
			test++;
			if(test != (*((uint8_t *)(0x64000000 + test_address)))) {
				CT_SEG7->BIN.HWORD = test_address;
				while(!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0)) {}
			}
			(*((uint8_t *)(0x64000000 + test_address))) = ~test;
		}
		
		test = 0;
		for(test_address = 0; test_address < 0x800; test_address++) {
			test++;
			if((uint8_t)(~test) != (*((uint8_t *)(0x64000000 + test_address)))) {
				CT_SEG7->BIN.HWORD = test_address;
				while(!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0)) {}
			}
		}


    /// END: To be programmed
    
    // Write 'End'
    CT_SEG7->RAW.BYTE.DS0 = 0xA1;
    CT_SEG7->RAW.BYTE.DS1 = 0xAB;
    CT_SEG7->RAW.BYTE.DS2 = 0x86;
    CT_SEG7->RAW.BYTE.DS3 = 0xFF;
    
    while(1){
    }

}
