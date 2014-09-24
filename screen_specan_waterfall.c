// Written by: Farrell Farahbod
// Last revised: 2014-09-23
// License: public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "f0lib/f0lib_rf_cc2500.h"
#include "externs.h"

void drawSpecAnWaterfallScreen() {
	/********************************
	 *2.4GHz SpecAn (Waterfall View)*
	 *                              *
	 *                              *
	 *                              *
	 *                              *
	 *                              *
	 *                              *
	 *                              *
	 *                              *
	 *^2400 MHz            2483 MHz^*
	 ********************************/

	lcd_set_color3(0, 0, 29); // make "2.4GHz SpecAn (Waterfall View)" white
	lcd_printf(0, 0, "2.4GHz SpecAn (Waterfall View)");
	lcd_printf(0, 1, "                              ");
	lcd_printf(0, 2, "                              ");
	lcd_printf(0, 3, "                              ");
	lcd_printf(0, 4, "                              ");
	lcd_printf(0, 5, "                              ");
	lcd_printf(0, 6, "                              ");
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, "                              ");
	lcd_printf(0, 9, "^2400 MHz            2483 MHz^");

	// reconfigure the CC2500 for use as a spectrum analyzer
	cc2500_send_strobe(SRES); // reset
	for(volatile uint32_t i = 0; i < 99999; i++); // wait for reset to finish
	cc2500_write_register(IOCFG2,	0x0B);
	cc2500_write_register(IOCFG0,	0x0C);
	cc2500_write_register(PKTCTRL0,	0x12);
	cc2500_write_register(FSCTRL1,	0x0C);
	cc2500_write_register(FREQ2,	0x5C);
	cc2500_write_register(FREQ1,	0x4E);
	cc2500_write_register(FREQ0,	0xC4);
	cc2500_write_register(MDMCFG4,	0x2E);
	cc2500_write_register(MDMCFG3,	0x3B);
	cc2500_write_register(MDMCFG2,	0x70);
	cc2500_write_register(MDMCFG1,	0x43);
	cc2500_write_register(MDMCFG0,	0xB8);
	cc2500_write_register(DEVIATN,	0x00);
	cc2500_write_register(MCSM0,	0x18);
	cc2500_write_register(FOCCFG,	0x1D);
	cc2500_write_register(BSCFG,	0x1C);
	cc2500_write_register(AGCCTRL2,	0xC7);
	cc2500_write_register(AGCCTRL0,	0xB0);
	cc2500_write_register(FREND1,	0xB6);
	cc2500_write_register(FSCAL3,	0xEA);
	cc2500_write_register(FSCAL1,	0x00);
	cc2500_write_register(FSCAL0,	0x19);
	cc2500_write_register(PATABLE,	0xFF);
	
	lcd_pixel_stream_start();

	// draw waterfall view
	uint32_t row = 40;
	while(currentScreen == SPEC_AN_WATERFALL) {
							
		uint16_t colors[16] = {	0b0001000000000101, //<rrr><gggg><bbb>
								0b0001100000001001,
								0b0011000000001001,
								0b0100100000001010,
								0b0101100000001001,
								0b0110000000101000,
								0b0110100001100110,
								0b0111000010000011,
								0b0111100011000000,
								0b0111100100000000,
								0b1000000100100000,
								0b1000000101100000,
								0b1000000110100001,
								0b1000000111000011,
								0b1000000111101001,
								0b1000001000010000};

		#define CHANNEL_COUNT 240
		#define ROW_COUNT 284 // leave room for bottom row of text

		for(uint32_t channel = 0; channel < CHANNEL_COUNT; channel++) {
			cc2500_send_strobe(SIDLE);
			cc2500_set_channel(channel);
			cc2500_enter_rx_mode();
			int16_t rssi = cc2500_get_rssi();
			for(volatile uint32_t i = 0; i < 4800; i++); // not sure why this delay is required

			lcd_pixel_stream_start();
			lcd_pixel_stream_move_cursor(2*channel, row);
			lcd_pixel_stream_write_pixel(colors[rssi/16]);
			lcd_pixel_stream_write_pixel(colors[rssi/16]);
			lcd_pixel_stream_move_cursor(2*channel+1, row);
			lcd_pixel_stream_write_pixel(colors[rssi/16]);
			lcd_pixel_stream_write_pixel(colors[rssi/16]);
			lcd_pixel_stream_stop();
		}
		row+=2;
		if(row >= ROW_COUNT) row = 40;
	}
}

void notifySpecAnWaterfallScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			currentScreen = GAME;
			return;
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			currentScreen = SPEC_AN_LIVE;
			return;
			break;
			
		case UP_BUTTON_PRESSED:

			break;
		
		case DOWN_BUTTON_PRESSED:

			break;
		
	}

}
