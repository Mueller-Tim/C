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

/* standard includes */
#include <stdint.h>
#include "hal_ct_buttons.h"
#include <reg_ctboard.h>


/// STUDENTS: To be programmed

#define startEeprom (uint8_t*) 0x64000400


/// END: To be programmed

int main(void)
{
    /// STUDENTS: To be programmed
		
		uint8_t index = 0;
		uint16_t error = 0;
		
		
		
		while(1){
			uint8_t readValue = *(startEeprom + index);
			CT_LED->BYTE.LED23_16 = index;
			CT_LED->BYTE.LED7_0 = readValue;
			if(index != readValue){
				error ++;
				CT_SEG7->BIN.HWORD = error;	
				while(!hal_ct_button_is_pressed(HAL_CT_BUTTON_T0));
			}
			index++;
		}	
		
		
		
		

    /// END: To be programmed
}
