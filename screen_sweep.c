// Written by Farrell Farahbod
// Last revised on 2014-07-09
// This file is released into the public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "config.h"

enum SelectedSweepOption {MODE, POS1, POS2, STEP, PERIOD, CYCLES} currentOption;
volatile uint16_t sweep_pos1 = 9000;
volatile uint16_t sweep_pos2 = 21000;
volatile uint16_t sweep_step = 70;
volatile uint16_t sweep_period = 10;
volatile uint32_t sweep_cycles = 0;

void drawSweepScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us    VOLT: 0.123V*
	 *POS2: 2100.0us    CH1:  1.234A*
	 *STEP:    5.0us    CH2:  2.345A*
	 *PERIOD:   10ms    CH3:  3.456A*
	 *CYCLES:      0    CH4:  4.567A*
	 *                              *
	 *                              *
	 *>> COMPLETED       0 CYCLES <<*
	 *                              *
	 ********************************/

	cyclesCompleted = 0;
	currentOption = MODE;
	lcd_set_color3(0, 7, 11); // change color for selected mode "Sweep"
	lcd_set_color2(0, 7, 29); // change color for unselected modes
	lcd_set_color1(0, 0, 4);  // change color for selected option "MODE:"
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", sweep_pos1, voltage);
	lcd_printf(0, 2, "POS2: %p41us    CH1:  %p13A", sweep_pos2, ch1current);
	lcd_printf(0, 3, "STEP:   %p21us    CH2:  %p13A", sweep_step, ch2current);
	lcd_printf(0, 4, "PERIOD:   %p20ms    CH3:  %p13A", sweep_period, ch3current);
	lcd_printf(0, 5, "CYCLES:%p70    CH4:  %p13A", sweep_cycles, ch4current);
	lcd_printf(0, 6, "                              ");
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
	lcd_printf(0, 9, "                              ");

	while(currentScreen == SWEEP); // wait for events or screen change
}

void notifySweepScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = ONE_POS;
			else if(currentOption == POS1 && sweep_pos1 > SERVO_PULSE_MIN)
				sweep_pos1 -= 5;
			else if(currentOption == POS2 && sweep_pos2 > SERVO_PULSE_MIN)
				sweep_pos2 -= 5;
			else if(currentOption == PERIOD && sweep_period > SERVO_PERIOD_MIN)
				{sweep_period--; TIM2->ARR = (sweep_period * 2000) - 1;}
			else if(currentOption == STEP && sweep_step > SERVO_STEP_MIN)
				sweep_step -= 5;
			else if(currentOption == CYCLES && sweep_cycles > 0)
				sweep_cycles--;
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				currentScreen = THREE_POS;
			else if(currentOption == POS1 && sweep_pos1 < SERVO_PULSE_MAX)
				sweep_pos1 += 5;
			else if(currentOption == POS2 && sweep_pos2 < SERVO_PULSE_MAX)
				sweep_pos2 += 5;
			else if(currentOption == PERIOD && sweep_period < SERVO_PERIOD_MAX)
				{sweep_period++; TIM2->ARR = (sweep_period * 2000) - 1;}
			else if(currentOption == STEP && sweep_step < SERVO_STEP_MAX)
				sweep_step += 5;
			else if(currentOption == CYCLES && sweep_cycles < SERVO_CYCLES_MAX)
				sweep_cycles++;	
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
			} else if(currentOption == STEP) {
				currentOption = POS2;
				lcd_set_color1(2, 0, 4);
			} else if(currentOption == PERIOD) {
				currentOption = STEP;
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
				currentOption = STEP;
				lcd_set_color1(3, 0, 4);
			} else if(currentOption == STEP) {
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
	lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", sweep_pos1, voltage);
	lcd_printf(0, 2, "POS2: %p41us    CH1:  %p13A", sweep_pos2, ch1current);
	lcd_printf(0, 3, "STEP:   %p21us    CH2:  %p13A", sweep_step, ch2current);
	lcd_printf(0, 4, "PERIOD:   %p20ms    CH3:  %p13A", sweep_period, ch3current);
	lcd_printf(0, 5, "CYCLES:%p70    CH4:  %p13A", sweep_cycles, ch4current);
	lcd_printf(0, 6, "                              ");
	lcd_printf(0, 7, "                              ");
	lcd_printf(0, 8, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
	lcd_printf(0, 9, "                              ");
}
