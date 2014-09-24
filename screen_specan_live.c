// Written by: Farrell Farahbod
// Last revised: 2014-09-15
// License: public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "f0lib/f0lib_rf_cc2500.h"
#include "externs.h"

void drawSpecAnLiveScreen() {
	/********************************
	 *2.4GHz SpecAn   (Live View)   *
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

	lcd_set_color3(0, 0, 29); // make "2.4GHz SpecAn (Live View)" white
	lcd_printf(0, 0, "2.4GHz SpecAn   (Live View)   ");
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

	// draw live view
	while(currentScreen == SPEC_AN_LIVE) {
		#define CHANNEL_COUNT 240
		#define HISTOGRAM_HEIGHT 240
		int16_t y;

		// iterate over all channels and draw the histogram
		for(uint8_t channel = 0; channel < CHANNEL_COUNT; channel++) {
			cc2500_send_strobe(SIDLE);
			cc2500_set_channel(channel);
			cc2500_enter_rx_mode();
			int16_t rssi_avg = cc2500_get_rssi();
			for(volatile uint32_t i = 0; i < 3000; i++); // not sure why this is required
			
			lcd_pixel_stream_move_cursor(channel*2, 40);
			for(y = HISTOGRAM_HEIGHT - 1; y >= 0; y--) {
				if(y <= rssi_avg)
					lcd_pixel_stream_write_pixel(0b1111100000000000);
				else
					lcd_pixel_stream_write_pixel(0);
			}
			lcd_pixel_stream_move_cursor(channel*2+1, 40);
			for(y = HISTOGRAM_HEIGHT - 1; y >= 0; y--) {
				if(y <= rssi_avg)
					lcd_pixel_stream_write_pixel(0b1111100000000000);
				else
					lcd_pixel_stream_write_pixel(0);
			}
		}
	}
	lcd_pixel_stream_stop();
}

void notifySpecAnLiveScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			currentScreen = SPEC_AN_WATERFALL;
			return;
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			currentScreen = SWEEP;
			return;
			break;
			
		case UP_BUTTON_PRESSED:

			break;
		
		case DOWN_BUTTON_PRESSED:

			break;
		
	}

}
