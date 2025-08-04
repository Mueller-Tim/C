/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - SPI Display
 * -- Description : Contains the implementations of the functions
 * --               to write and read frames from and to the TFT-LCD
 * --               display EAeDIPTFT43-A.
 * --
 * -- $Id: lcd_io.c 4720 2019-03-04 10:11:31Z akdi $
 * ------------------------------------------------------------------
 */
#include "lcd_io.h"
#include "hal_spi.h"
#include "hal_sbuf.h"

#define ACK_CHAR         (uint8_t)0x06
#define DC1_CHAR         (uint8_t)0x11
#define DC2_CHAR         (uint8_t)0x12
#define ESC_CHAR         (uint8_t)0x1B
#define ONE_CHAR         (uint8_t)0x01

#define NOTHING_RECEIVED (uint8_t)0
enum { SUCCESS = 0, ERRORCODE = 1 };

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */
static void send_read_display_buffer_request(void);


/* ------------------------------------------------------------------
 * -- Function implementations
 * ------------------------------------------------------------------
 */

/*
 * according to description in header file
 */
void init_display_interface(void)
{
    hal_spi_init();
    hal_sbuf_init();
}


/*
 * according to description in header file
 */
uint8_t read_display_buffer(uint8_t *readBuffer)
{
    /// STUDENTS: To be programmed
	
		if(hal_sbuf_get_state()){
			send_read_display_buffer_request();
			uint8_t rec_ack_byte = hal_spi_read_write(0x00);
			if(rec_ack_byte != ACK_CHAR){
				return 0;
			}
			uint8_t rec_dc1_byte = hal_spi_read_write(0x00);
			if(rec_dc1_byte != DC1_CHAR){
				return 0;
			}
			uint8_t bcc = 0;
			uint8_t rec_len_byte = hal_spi_read_write(0x00);
			for(uint8_t i = 0; i < rec_len_byte; i++){
				uint8_t rec_daten_byte = hal_spi_read_write(0x00);
				readBuffer[i] = rec_daten_byte;
				bcc = bcc + rec_daten_byte;
			}
			bcc = bcc + rec_dc1_byte + rec_len_byte;
			
			if(bcc%256 != hal_spi_read_write(0x00)){
				return 0;
			}
			return *readBuffer;
			
		} else{
			return 0;
		}
		
		
		


    /// END: To be programmed
}

/*
 * The function receives a command buffer in the form of a character string.
 * It adds the required protocol overhead and uses the SPI to write the
 * complete frame to the display. Afterwards it waits for the acknowledge
 * character from the display.
 *
 * The transmitted frame on the SPI consists of the required control characters
 * (DC1, len and ESC) followed by the contents of the specified command buffer
 * and the checksum (bcc).
 *
 * The function returns zero if the operation is successful, i.e. if an
 * acknowledge character is received from the display; It returns
 * one otherwise, i.e. if no acknowledge character is received from the display
 */
/*
 * according to description in header file
 */
uint8_t write_cmd_to_display(const uint8_t *cmdBuffer, uint8_t length)
{
    /// STUDENTS: To be programmed

		uint8_t bcc = DC1_CHAR + length + 1 + ESC_CHAR;
		hal_spi_read_write(DC1_CHAR);
		hal_spi_read_write(length + 1);
		hal_spi_read_write(ESC_CHAR);
		for(uint8_t i = 0; i < length; i++){
			hal_spi_read_write(cmdBuffer[i]);
			bcc += cmdBuffer[i];
		}
		hal_spi_read_write(bcc%256);
		
		uint8_t rec_byte = hal_spi_read_write(0x00);
		return rec_byte == ACK_CHAR ? SUCCESS : ERRORCODE;
    /// END: To be programmed
}


/*
 * Assemble and send a packet to trigger the reading of the display buffer
 * Uses the sequence "<DC2>, 0x01, 0x53, checksum" according to datasheet
 */
static void send_read_display_buffer_request(void)
{
    /// STUDENTS: To be programmed
	
		uint8_t bcc = DC2_CHAR + 0x01 + 0x53;
		hal_spi_read_write(DC2_CHAR);
		hal_spi_read_write(0x01);
		hal_spi_read_write(0x53);
		hal_spi_read_write(bcc%256);
		
	

    /// END: To be programmed
}

