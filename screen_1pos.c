// Written by: Farrell Farahbod
// Last revised: 2014-09-15
// License: public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "externs.h"
#include "settings.h"

enum Selected1posOption {MODE, POS, PERIOD} currentOption;

void draw1posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us    VOLT: 0.123V*
	 *PERIOD    10ms    CH1:  1.234A*
	 *                  CH2:  2.345A*
	 *                  CH3:  3.456A*
	 *                  CH4:  4.567A*
	 *                              *
	 *                              *
	 *                              *
	 *                              *
	 ********************************/

	cyclesCompleted = 0;
	currentOption = MODE;
	lcd_set_color3(0, 26, 29); // make "1pos" white
	lcd_set_color2(0, 7, 29);  // make unselected modes gray
	lcd_set_color1(0, 0, 4);   // make "MODE:" red

	while(currentScreen == ONE_POS) {
		// redraw
		lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
		lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", onePos_position, voltage);
		lcd_printf(0, 2, "PERIOD:   %p20ms    CH1:  %p13A", onePos_period, current1);
		lcd_printf(0, 3, "                  CH2:  %p13A", current2);
		lcd_printf(0, 4, "                  CH3:  %p13A", current3);
		lcd_printf(0, 5, "                  CH4:  %p13A", current4);
		lcd_printf(0, 6, "                              ");
		lcd_printf(0, 7, "                              ");
		lcd_printf(0, 8, "                              ");
		uint32_t barPosition = TIM2->CCR1;						// range = 1600 - 4400
		barPosition -= SERVO_PULSE_MIN / 5;						// range = 0 - 2800
		barPosition *= 459;										// range = 0 - 1 285 200
		barPosition /= (SERVO_PULSE_MAX - SERVO_PULSE_MIN) / 5;	// range = 0 - 459
		lcd_draw_bar(9, barPosition);
	}

}

void notify1posScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				{currentScreen = TWO_POS; return;}
			else if(currentOption == POS && onePos_position > 8000)
				onePos_position -= 5;
			else if(currentOption == PERIOD && onePos_period > 3)
				{onePos_period--; TIM2->ARR = (onePos_period * 2000) - 1;}
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				{currentScreen = CALIBRATION; return;}
			else if(currentOption == POS && onePos_position < 22000)
				onePos_position += 5;
			else if(currentOption == PERIOD && onePos_period < 30)
				{onePos_period++; TIM2->ARR = (onePos_period * 2000) - 1;}
			break;
			
		case UP_BUTTON_PRESSED:

			if(currentOption == MODE) {
				currentOption = PERIOD;
				lcd_set_color1(2, 0, 6);
			} else if(currentOption == POS) {
				currentOption = MODE;
				lcd_set_color1(0, 0, 4);
			} else if(currentOption == PERIOD) {
				currentOption = POS;
				lcd_set_color1(1, 0, 4);
			}
			break;
		
		case DOWN_BUTTON_PRESSED:

			if(currentOption == MODE) {
				currentOption = POS;
				lcd_set_color1(1, 0, 4);
			} else if(currentOption == POS) {
				currentOption = PERIOD;
				lcd_set_color1(2, 0, 6);
			} else if(currentOption == PERIOD) {
				currentOption = MODE;
				lcd_set_color1(0, 0, 4);
			}
			break;
		
	}

}
