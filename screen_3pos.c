// Written by: Farrell Farahbod
// Last revised: 2014-09-15
// License: public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "externs.h"
#include "settings.h"

enum Selected3PosOption {MODE, POS1, POS2, POS3, WAIT, PERIOD, CYCLES} currentOption;

void draw3posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us    VOLT: 0.123V*
	 *POS2: 1500.0us    CH1:  1.234A*
	 *POS3: 2100.0us    CH2:  2.345A*
	 *WAIT:     1.0s    CH3:  3.456A*
	 *PERIOD:   10ms    CH4:  4.567A*
	 *CYCLES:      0                *
	 *                              *
	 *>> COMPLETED       0 CYCLES <<*
	 *                              *
	 ********************************/

	cyclesCompleted = 0;
	currentOption = MODE;
	lcd_set_color3(0, 14, 17); // make "3pos" white
	lcd_set_color2(0, 7, 29);  // make unselected modes gray
	lcd_set_color1(0, 0, 4);   // make "MODE:" red
	
	while(currentScreen == THREE_POS) {
		// redraw
		lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
		lcd_printf(0, 1, "POS1: %p41us    VOLT: %p13V", threePos_pos1, voltage);
		lcd_printf(0, 2, "POS2: %p41us    CH1:  %p13A", threePos_pos2, current1);
		lcd_printf(0, 3, "POS3: %p41us    CH2:  %p13A", threePos_pos3, current2);
		lcd_printf(0, 4, "WAIT:   %p31s    CH3:  %p13A", threePos_wait, current3);
		lcd_printf(0, 5, "PERIOD:   %p20ms    CH4:  %p13A", threePos_period, current4);
		lcd_printf(0, 6, "CYCLES:%p70                ", threePos_cycles);
		lcd_printf(0, 7, "                              ");
		lcd_printf(0, 8, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
		uint32_t barPosition = TIM2->CCR1;						// range = 1600 - 4400
		barPosition -= SERVO_PULSE_MIN / 5;						// range = 0 - 2800
		barPosition *= 459;										// range = 0 - 1 285 200
		barPosition /= (SERVO_PULSE_MAX - SERVO_PULSE_MIN) / 5;	// range = 0 - 459
		lcd_draw_bar(9, barPosition);
	}
	
}

void notify3posScreen(enum Event event) {
	switch(event) {
		
		case LEFT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				{currentScreen = SWEEP; return;}
			else if(currentOption == POS1 && threePos_pos1 > SERVO_PULSE_MIN)
				threePos_pos1 -= 5;
			else if(currentOption == POS2 && threePos_pos2 > SERVO_PULSE_MIN)
				threePos_pos2 -= 5;
			else if(currentOption == POS3 && threePos_pos3 > SERVO_PULSE_MIN)
				threePos_pos3 -= 5;
			else if(currentOption == PERIOD && threePos_period > SERVO_PERIOD_MIN)
				{threePos_period--; TIM2->ARR = (threePos_period * 2000) - 1;}
			else if(currentOption == WAIT && threePos_wait > SERVO_WAIT_MIN)
				threePos_wait--;
			else if(currentOption == CYCLES && threePos_cycles > 0)
				threePos_cycles--;
			break;
			
		case RIGHT_BUTTON_PRESSED:
		
			if(currentOption == MODE)
				{currentScreen = TWO_POS; return;}
			else if(currentOption == POS1 && threePos_pos1 < SERVO_PULSE_MAX)
				threePos_pos1 += 5;
			else if(currentOption == POS2 && threePos_pos2 < SERVO_PULSE_MAX)
				threePos_pos2 += 5;
			else if(currentOption == POS3 && threePos_pos3 < SERVO_PULSE_MAX)
				threePos_pos3 += 5;
			else if(currentOption == PERIOD && threePos_period < SERVO_PERIOD_MAX)
				{threePos_period++; TIM2->ARR = (threePos_period * 2000) - 1;}
			else if(currentOption == WAIT && threePos_wait < SERVO_WAIT_MAX)
				threePos_wait++;
			else if(currentOption == CYCLES && threePos_cycles < SERVO_CYCLES_MAX)
				threePos_cycles++;
			break;
			
		case UP_BUTTON_PRESSED:

			if(currentOption == MODE) {
				currentOption = CYCLES;
				lcd_set_color1(3, 15, 21);
			} else if(currentOption == POS1) {
				currentOption = MODE;
				lcd_set_color1(0, 0, 4);
			} else if(currentOption == POS2) {
				currentOption = POS1;
				lcd_set_color1(1, 0, 4);
			} else if(currentOption == POS3) {
				currentOption = POS2;
				lcd_set_color1(2, 0, 4);
			} else if(currentOption == WAIT) {
				currentOption = POS3;
				lcd_set_color1(3, 0, 4);
			} else if(currentOption == PERIOD) {
				currentOption = WAIT;
				lcd_set_color1(4, 0, 4);
			} else if(currentOption == CYCLES) {
				currentOption = PERIOD;
				lcd_set_color1(5, 0, 6);
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
				currentOption = POS3;
				lcd_set_color1(3, 0, 4);
			} else if(currentOption == POS3) {
				currentOption = WAIT;
				lcd_set_color1(4, 0, 4);
			} else if(currentOption == WAIT) {
				currentOption = PERIOD;
				lcd_set_color1(5, 0, 6);
			} else if(currentOption == PERIOD) {
				currentOption = CYCLES;
				lcd_set_color1(6, 0, 6);
			} else if(currentOption == CYCLES) {
				currentOption = MODE;
				lcd_set_color1(0, 0, 4);
			}
			break;
		
	}

}
