// Written by: Farrell Farahbod
// Last revised: 2014-09-15
// License: public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "f0lib/f0lib_flash.h"
#include "externs.h"

struct PersistentData pdStruct;

enum SelectedCalibrationOption {MODE, TOUCHSCREEN, VOLTAGE, CH1CURR, CH2CURR, CH3CURR, CH4CURR, SAVE} currentOption;

void drawCalibrationScreen() {
	/********************************
	 *MODE:   SYSTEM CALIBRATION    *
	 *TOUCHSCREEN =  X  0000  Y 0000*
	 *VOLTAGE     = 5V  0123  0.123V*
	 *CH1CURRENT  = 1A  1234  1.234A*
	 *CH2CURRENT  = 1A  2345  2.345A*
	 *CH3CURRENT  = 1A  3456  3.456A*
	 *CH4CURRENT  = 1A  4567  4.567A*
	 *                              *
	 *>SAVE CHANGES TO FLASH MEMORY<*
	 *To discard changes: power off.*
	 ********************************/

	currentOption = MODE;
	lcd_set_color3(0, 5, 29); // make "SYSTEM CALIBRATION" white
	lcd_set_color1(0, 0, 4);  // make "MODE:" red

	lcd_printf(0, 0, "MODE:   SYSTEM CALIBRATION    ");
	lcd_printf(0, 1, "TOUCHSCREEN =  X  %p40  Y %p40", touchX, touchY);
	lcd_printf(0, 2, "VOLTAGE     = 5V  %p40  %p13V", voltageRaw, voltage);
	lcd_printf(0, 3, "CH1CURRENT  = 1A  %p40  %p13A", current1raw, current1);
	lcd_printf(0, 4, "CH2CURRENT  = 1A  %p40  %p13A", current2raw, current2);
	lcd_printf(0, 5, "CH3CURRENT  = 1A  %p40  %p13A", current3raw, current3);
	lcd_printf(0, 6, "CH4CURRENT  = 1A  %p40  %p13A", current4raw, current4);
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, ">SAVE CHANGES TO FLASH MEMORY<");
	lcd_printf(0, 9, "To discard changes: power off.");
	while(currentScreen == CALIBRATION) {
		// redraw
		lcd_printf(0, 0, "MODE:   SYSTEM CALIBRATION    ");
		lcd_printf(0, 1, "TOUCHSCREEN =  X  %p40  Y %p40", touchX, touchY);
		lcd_printf(0, 2, "VOLTAGE     = 5V  %p40  %p13V", voltageRaw, voltage);
		lcd_printf(0, 3, "CH1CURRENT  = 1A  %p40  %p13A", current1raw, current1);
		lcd_printf(0, 4, "CH2CURRENT  = 1A  %p40  %p13A", current2raw, current2);
		lcd_printf(0, 5, "CH3CURRENT  = 1A  %p40  %p13A", current3raw, current3);
		lcd_printf(0, 6, "CH4CURRENT  = 1A  %p40  %p13A", current4raw, current4);
		lcd_printf(0, 7, "                              ");
		lcd_printf(0, 8, ">SAVE CHANGES TO FLASH MEMORY<");
		//lcd_printf(0, 9, "To discard changes: power off.");
	}

}

void notifyCalibrationScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = ONE_POS;
			if(currentOption == TOUCHSCREEN)
				touchMinXCalibrationFactor = touchXraw;
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = GAME;
			else if(currentOption == TOUCHSCREEN)
				touchMaxXCalibrationFactor = touchXraw;
			else if(currentOption == VOLTAGE)
				voltageCalibrationFactor = voltageRaw;
			else if(currentOption == CH1CURR)
				current1CalibrationFactor = current1raw;
			else if(currentOption == CH2CURR)
				current2CalibrationFactor = current2raw;
			else if(currentOption == CH3CURR)
				current3CalibrationFactor = current3raw;
			else if(currentOption == CH4CURR)
				current4CalibrationFactor = current4raw;
			else if(currentOption == SAVE) {
				pdStruct.voltageCalibrationFactor = voltageCalibrationFactor;
				pdStruct.current1CalibrationFactor = current1CalibrationFactor;
				pdStruct.current2CalibrationFactor = current2CalibrationFactor;
				pdStruct.current3CalibrationFactor = current3CalibrationFactor;
				pdStruct.current4CalibrationFactor = current4CalibrationFactor;
				pdStruct.touchMinXCalibrationFactor = touchMinXCalibrationFactor;
				pdStruct.touchMaxXCalibrationFactor = touchMaxXCalibrationFactor;
				pdStruct.touchMinYCalibrationFactor = touchMinYCalibrationFactor;
				pdStruct.touchMaxYCalibrationFactor = touchMaxYCalibrationFactor;
				if(flash_erase_page(63)) {
					if(flash_write_page(63, &pdStruct, sizeof(pdStruct))) {
						lcd_printf(0, 9, "         >>> SAVED <<<        ");
						currentOption = MODE;
						lcd_set_color1(0, 0, 4);
					} else {
						lcd_printf(0, 9, "         >>> ERROR <<<        ");
						currentOption = MODE;
						lcd_set_color1(0, 0, 4);
					}
				} else {
					lcd_printf(0, 9, "         >>> ERROR <<<        ");
					currentOption = MODE;
					lcd_set_color1(0, 0, 4);
				}
			}
			break;
			
		case UP_BUTTON_PRESSED:

			if(currentOption == MODE) {
				currentOption = SAVE;
				lcd_set_color1(8, 0, 29);
			} else if(currentOption == SAVE) {
				currentOption = CH4CURR;
				lcd_set_color1(6, 0, 15);
			} else if(currentOption == CH4CURR) {
				currentOption = CH3CURR;
				lcd_set_color1(5, 0, 15);
			} else if(currentOption == CH3CURR) {
				currentOption = CH2CURR;
				lcd_set_color1(4, 0, 15);
			} else if(currentOption == CH2CURR) {
				currentOption = CH1CURR;
				lcd_set_color1(3, 0, 15);
			} else if(currentOption == CH1CURR) {
				currentOption = VOLTAGE;
				lcd_set_color1(2, 0, 15);
			} else if(currentOption == VOLTAGE) {
				currentOption = TOUCHSCREEN;
				lcd_set_color1(1, 0, 15);
			} else if(currentOption == TOUCHSCREEN) {
				touchMaxYCalibrationFactor = touchYraw;
			}
			break;
		
		case DOWN_BUTTON_PRESSED:

			if(currentOption == MODE) {
				currentOption = TOUCHSCREEN;
				lcd_set_color1(1, 0, 15);
			} else if(currentOption == TOUCHSCREEN) {
				touchMinYCalibrationFactor = touchYraw;
				currentOption = VOLTAGE;
				lcd_set_color1(2, 0, 15);
			} else if(currentOption == VOLTAGE) {
				currentOption = CH1CURR;
				lcd_set_color1(3, 0, 15);
			} else if(currentOption == CH1CURR) {
				currentOption = CH2CURR;
				lcd_set_color1(4, 0, 15);
			} else if(currentOption == CH2CURR) {
				currentOption = CH3CURR;
				lcd_set_color1(5, 0, 15);
			} else if(currentOption == CH3CURR) {
				currentOption = CH4CURR;
				lcd_set_color1(6, 0, 15);
			} else if(currentOption == CH4CURR) {
				currentOption = SAVE;
				lcd_set_color1(8, 0, 29);
			} else if(currentOption == SAVE) {
				currentOption = MODE;
				lcd_set_color1(0, 0, 4);
			}
			break;
		
	}

}
