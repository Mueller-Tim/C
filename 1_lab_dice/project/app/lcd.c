/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module lcd
 * --               Performs all the interactions with the lcd
 * --
 * -- $Id: lcd.c 5144 2020-09-01 06:17:21Z ruan $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdio.h>

/* user includes */
#include "lcd.h"
#include "reg_ctboard.h"

/* macros */
#define LCD_ADDR_LINE1      0u
#define LCD_ADDR_LINE2      20u

#define NR_OF_CHAR_PER_LINE 20u

#define LCD_CLEAR           "                    "

/// STUDENTS: To be programmed

void lcd_write_value(uint8_t slot_nr, uint8_t value){
		char text[3];
		int length = snprintf(text, sizeof(text), "%d", value);
		if(length == 2){
			CT_LCD->ASCII[(slot_nr-1) * 3] = text[0];
			CT_LCD->ASCII[(slot_nr-1) * 3 + 1] = text[1];
		} else{
			CT_LCD->ASCII[(slot_nr-1) * 3 + 1] = text[0];
		}
		
}

void lcd_write_total(uint8_t total_value){
	char text[NR_OF_CHAR_PER_LINE + 1]; // + 1 wichtig damit der puffer gross genug ist
	int length = snprintf(text, sizeof(text), "total throws  %d", total_value); 
	for(int i = 0; i < length; i++){
		CT_LCD->ASCII[LCD_ADDR_LINE2 + i] = text[i];
	}
}

void hal_ct_lcd_clear(void){
	for( int i = 0; i < NR_OF_CHAR_PER_LINE; i++){
		CT_LCD->ASCII[i] = ' ';
	}
	CT_LCD->BG.GREEN = 65535;
}


/// END: To be programmed
