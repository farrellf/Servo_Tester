// Written by Farrell Farahbod
// Last revised on 2014-07-09
// This file is released into the public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "config.h"

enum Selected1posOption {MODE, POS, PERIOD} currentOption;
volatile uint16_t onePos_position = 15000;
volatile uint16_t onePos_period = 10;

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
	lcd_set_color3(0, 26, 29); // change color for selected mode "1pos"
	lcd_set_color2(0, 7, 29);  // change color for unselected modes
	lcd_set_color1(0, 0, 4);   // change color for selected option "MODE:"
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", onePos_position, voltage);
	lcd_printf(0, 2, "PERIOD:   %p20ms    CH1:  %p13A", onePos_period, ch1current);
	lcd_printf(0, 3, "                  CH2:  %p13A", ch2current);
	lcd_printf(0, 4, "                  CH3:  %p13A", ch3current);
	lcd_printf(0, 5, "                  CH4:  %p13A", ch4current);
	lcd_printf(0, 6, "                              ");
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, "                              ");
	lcd_printf(0, 9, "                              ");

	while(currentScreen == ONE_POS); // wait for events or screen change
}

void notify1posScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = TWO_POS;
			else if(currentOption == POS && onePos_position > 8000)
				onePos_position -= 5;
			else if(currentOption == PERIOD && onePos_period > 3)
				{onePos_period--; TIM2->ARR = (onePos_period * 2000) - 1;}
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = SWEEP;
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
		
		case CH1_CURRENT_READING:

			break;
		
		case CH2_CURRENT_READING:

			break;
		
		case CH3_CURRENT_READING:

			break;
		
		case CH4_CURRENT_READING:

			break;

		case VOLTAGE_READING:

			break;
		
	}

	// redraw
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", onePos_position, voltage);
	lcd_printf(0, 2, "PERIOD:   %p20ms    CH1:  %p13A", onePos_period, ch1current);
	lcd_printf(0, 3, "                  CH2:  %p13A", ch2current);
	lcd_printf(0, 4, "                  CH3:  %p13A", ch3current);
	lcd_printf(0, 5, "                  CH4:  %p13A", ch4current);
	lcd_printf(0, 6, "                              ");
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, "                              ");
	lcd_printf(0, 9, "                              ");
}
