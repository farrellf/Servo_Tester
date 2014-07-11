// Written by Farrell Farahbod
// Last revised on 2014-07-09
// This file is released into the public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "config.h"

enum Selected2posOption {MODE, POS1, POS2, WAIT, PERIOD, CYCLES} currentOption;
volatile uint16_t twoPos_pos1 = 9000;
volatile uint16_t twoPos_pos2 = 21000;
volatile uint16_t twoPos_wait = 18;
volatile uint16_t twoPos_period = 10;
volatile uint32_t twoPos_cycles = 0;

void draw2posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us    VOLT: 0.123V*
	 *POS2: 1500.0us    CH1:  1.234A*
	 *WAIT:     1.0s    CH2:  2.345A*
	 *PERIOD:   10ms    CH3:  3.456A*
	 *CYCLES:      0    CH4:  4.567A*
	 *                              *
	 *                              *
	 *>> COMPLETED       0 CYCLES <<*
	 *                              *
	 ********************************/

	cyclesCompleted = 0;
	currentOption = MODE;
	lcd_set_color3(0, 20, 23); // change color for selected mode "Sweep"
	lcd_set_color2(0, 7, 29); // change color for unselected modes
	lcd_set_color1(0, 0, 4);  // change color for selected option "MODE:"
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", twoPos_pos1, voltage);
	lcd_printf(0, 2, "POS2: %p41us    CH1:  %p13A", twoPos_pos2, ch1current);
	lcd_printf(0, 3, "WAIT:   %p31s    CH2:  %p13A", twoPos_wait, ch2current);
	lcd_printf(0, 4, "PERIOD:   %p20ms    CH3:  %p13A", twoPos_period, ch3current);
	lcd_printf(0, 5, "CYCLES:%p70    CH4:  %p13A", twoPos_cycles, ch4current);
	lcd_printf(0, 6, "                              ");
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
	lcd_printf(0, 9, "                              ");

	while(currentScreen == TWO_POS); // wait for events or screen change
}

void notify2posScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = THREE_POS;
			else if(currentOption == POS1 && twoPos_pos1 > SERVO_PULSE_MIN)
				twoPos_pos1 -= 5;
			else if(currentOption == POS2 && twoPos_pos2 > SERVO_PULSE_MIN)
				twoPos_pos2 -= 5;
			else if(currentOption == PERIOD && twoPos_period > SERVO_PERIOD_MIN)
				{twoPos_period--; TIM2->ARR = (twoPos_period * 2000) - 1;}
			else if(currentOption == WAIT && twoPos_wait > SERVO_WAIT_MIN)
				twoPos_wait--;
			else if(currentOption == CYCLES && twoPos_cycles > 0)
				twoPos_cycles--;
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = ONE_POS;
			else if(currentOption == POS1 && twoPos_pos1 < SERVO_PULSE_MAX)
				twoPos_pos1 += 5;
			else if(currentOption == POS2 && twoPos_pos2 < SERVO_PULSE_MAX)
				twoPos_pos2 += 5;
			else if(currentOption == PERIOD && twoPos_period < SERVO_PERIOD_MAX)
				{twoPos_period++; TIM2->ARR = (twoPos_period * 2000) - 1;}
			else if(currentOption == WAIT && twoPos_wait < SERVO_WAIT_MAX)
				twoPos_wait++;
			else if(currentOption == CYCLES && twoPos_cycles < SERVO_CYCLES_MAX)
				twoPos_cycles++;
			break;
			
		case UP_BUTTON_PRESSED:

			if(currentOption == MODE) {
				currentOption = CYCLES;
				lcd_set_color1(5, 0, 6);
			} else if(currentOption == POS1) {
				currentOption = MODE;
				lcd_set_color1(0, 0, 4);
			} else if(currentOption == POS2) {
				currentOption = POS1;
				lcd_set_color1(1, 0, 4);
			} else if(currentOption == WAIT) {
				currentOption = POS2;
				lcd_set_color1(2, 0, 4);
			} else if(currentOption == PERIOD) {
				currentOption = WAIT;
				lcd_set_color1(3, 0, 4);
			} else if(currentOption == CYCLES) {
				currentOption = PERIOD;
				lcd_set_color1(4, 0, 6);
			}
			break;
		
		case DOWN_BUTTON_PRESSED:

			if(currentOption == MODE) {
				currentOption = POS1;
				lcd_set_color1(1, 0, 4);
			} else if(currentOption == POS1) {
				currentOption = POS2;
				lcd_set_color1(2, 0, 4);
			} else if(currentOption == POS2) {
				currentOption = WAIT;
				lcd_set_color1(3, 0, 4);
			} else if(currentOption == WAIT) {
				currentOption = PERIOD;
				lcd_set_color1(4, 0, 6);
			} else if(currentOption == PERIOD) {
				currentOption = CYCLES;
				lcd_set_color1(5, 0, 6);
			} else if(currentOption == CYCLES) {
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
	lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", twoPos_pos1, voltage);
	lcd_printf(0, 2, "POS2: %p41us    CH1:  %p13A", twoPos_pos2, ch1current);
	lcd_printf(0, 3, "WAIT:   %p31s    CH2:  %p13A", twoPos_wait, ch2current);
	lcd_printf(0, 4, "PERIOD:   %p20ms    CH3:  %p13A", twoPos_period, ch3current);
	lcd_printf(0, 5, "CYCLES:%p70    CH4:  %p13A", twoPos_cycles, ch4current);
	lcd_printf(0, 6, "                              ");
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
	lcd_printf(0, 9, "                              ");
}
