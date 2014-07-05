#include "externs.h"
#include "f0lib/f0lib_lcd_tft1p4705.h"

#define SERVO_PULSE_MIN 8000      // 800.0us
#define SERVO_PULSE_MAX 22000     // 2200.0us
#define SERVO_PERIOD_MIN 3        // 3ms (f=333Hz)
#define SERVO_PERIOD_MAX 30       // 30ms (f=33Hz)
#define SERVO_STEP_MIN 1          // 0.1us
#define SERVO_STEP_MAX 9999       // 999.9us
#define SERVO_WAIT_MIN 1          // 0.1s
#define SERVO_WAIT_MAX 999        // 99.9s
#define SERVO_CYCLES_MAX 9999999
#define SWITCH_BOUNCE_TIME 250000

void EXTI4_15_IRQHandler() {

	/////////////////////////////////
	// LEFT BUTTON PRESSED, EXTI 4 //
	/////////////////////////////////
	
	if((EXTI->PR & (1<<4)) != 0) {	// interrupt occured on EXTI 4
		EXTI->PR = (1<<4);			// handling EXTI 4

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<4)) != 0) 
			return;

		switch(currentScreen) {

			// left button pressed while on the Sweep screen
			case SWEEP:
				if(currentlySelectedOption == MODE)
					{currentScreen = ONE_POS; lcd_tft1p4705_set_selected_mode(0, 26, 29); cyclesCompleted = 0;}
				else if(currentlySelectedOption == SWEEP_START && sweep_start > SERVO_PULSE_MIN)
					sweep_start -= 5;
				else if(currentlySelectedOption == SWEEP_STOP && sweep_stop > SERVO_PULSE_MIN)
					sweep_stop -= 5;
				else if(currentlySelectedOption == SWEEP_CYCLES && sweep_cycles > 0)
					sweep_cycles--;
				else if(currentlySelectedOption == SWEEP_RATE && sweep_rate > SERVO_PERIOD_MIN)
					{sweep_rate--; TIM2->ARR = (sweep_rate * 2000) - 1;}
				else if(currentlySelectedOption == SWEEP_STEP && sweep_step > SERVO_STEP_MIN)
					sweep_step -= 5;
				break;

			// left button pressed while on the 3Pos screen
			case THREE_POS:
				if(currentlySelectedOption == MODE)
					{currentScreen = SWEEP; lcd_tft1p4705_set_selected_mode(0, 7, 11); cyclesCompleted = 0;}
				else if(currentlySelectedOption == THREEPOS_1POS && threePos_firstPosition > 8000)
					threePos_firstPosition -= 5;
				else if(currentlySelectedOption == THREEPOS_2POS && threePos_secondPosition > 8000)
					threePos_secondPosition -= 5;
				else if(currentlySelectedOption == THREEPOS_3POS && threePos_thirdPosition > 8000)
					threePos_thirdPosition -= 5;
				else if(currentlySelectedOption == THREEPOS_RATE && threePos_rate > 3)
					{threePos_rate--; TIM2->ARR = (threePos_rate * 2000) - 1;}
				else if(currentlySelectedOption == THREEPOS_WAIT && threePos_wait > 1)
					threePos_wait--;
				else if(currentlySelectedOption == THREEPOS_CYCLES && threePos_cycles > 0)
					threePos_cycles--;
				break;

			// left button pressed while on the 2Pos screen
			case TWO_POS:
				if(currentlySelectedOption == MODE)
					{currentScreen = THREE_POS; lcd_tft1p4705_set_selected_mode(0, 14, 17); cyclesCompleted = 0;}
				else if(currentlySelectedOption == TWOPOS_1POS && twoPos_firstPosition > 8000)
					twoPos_firstPosition -= 5;
				else if(currentlySelectedOption == TWOPOS_2POS && twoPos_secondPosition > 8000)
					twoPos_secondPosition -= 5;
				else if(currentlySelectedOption == TWOPOS_RATE && twoPos_rate > 3)
					{twoPos_rate--; TIM2->ARR = (twoPos_rate * 2000) - 1;}
				else if(currentlySelectedOption == TWOPOS_WAIT && twoPos_wait > 1)
					twoPos_wait--;
				else if(currentlySelectedOption == TWOPOS_CYCLES && twoPos_cycles > 0)
					twoPos_cycles--;
				break;

			// left button pressed while on the 1Pos screen
			case ONE_POS:
				if(currentlySelectedOption == MODE)
					{currentScreen = TWO_POS; lcd_tft1p4705_set_selected_mode(0, 20, 23); cyclesCompleted = 0;}
				else if(currentlySelectedOption == ONEPOS_POSITION && onePos_position > 8000)
					onePos_position -= 5;
				else if(currentlySelectedOption == ONEPOS_RATE && onePos_rate > 3)
					{onePos_rate--; TIM2->ARR = (onePos_rate * 2000) - 1;}
				break;
		}
				
	} // end of exti4 (left button)

	///////////////////////////////
	// UP BUTTON PRESSED, EXTI 5 //
	///////////////////////////////

	if((EXTI->PR & (1<<5)) != 0) {	// interrupt occured on EXTI 5
		EXTI->PR = (1<<5);			// handling EXTI 5

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<5)) != 0) 
			return;

		switch(currentScreen) {

			// up button pressed while on the Sweep screen
			case SWEEP:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = SWEEP_CYCLES;
					lcd_tft1p4705_set_selected_option(3, 15, 21);
				} else if(currentlySelectedOption == SWEEP_START) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				} else if(currentlySelectedOption == SWEEP_STOP) {
					currentlySelectedOption = SWEEP_START;
					lcd_tft1p4705_set_selected_option(1, 0, 5);
				} else if(currentlySelectedOption == SWEEP_RATE) {
					currentlySelectedOption = SWEEP_STOP;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == SWEEP_STEP) {
					currentlySelectedOption = SWEEP_RATE;
					lcd_tft1p4705_set_selected_option(1, 17, 21);
				} else if(currentlySelectedOption == SWEEP_CYCLES) {
					currentlySelectedOption = SWEEP_STEP;
					lcd_tft1p4705_set_selected_option(2, 17, 21);
				}
				break;

			// up button pressed while on the 3Pos screen
			case THREE_POS:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = THREEPOS_CYCLES;
					lcd_tft1p4705_set_selected_option(3, 15, 21);
				} else if(currentlySelectedOption == THREEPOS_1POS) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				} else if(currentlySelectedOption == THREEPOS_2POS) {
					currentlySelectedOption = THREEPOS_1POS;
					lcd_tft1p4705_set_selected_option(1, 0, 4);
				} else if(currentlySelectedOption == THREEPOS_3POS) {
					currentlySelectedOption = THREEPOS_2POS;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == THREEPOS_RATE) {
					currentlySelectedOption = THREEPOS_3POS;
					lcd_tft1p4705_set_selected_option(3, 0, 4);
				} else if(currentlySelectedOption == THREEPOS_WAIT) {
					currentlySelectedOption = THREEPOS_RATE;
					lcd_tft1p4705_set_selected_option(1, 17, 21);
				} else if(currentlySelectedOption == THREEPOS_CYCLES) {
					currentlySelectedOption = THREEPOS_WAIT;
					lcd_tft1p4705_set_selected_option(2, 17, 21);
				}
				break;

			// up button pressed while on the 2Pos screen
			case TWO_POS:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = TWOPOS_CYCLES;
					lcd_tft1p4705_set_selected_option(3, 15, 21);
				} else if(currentlySelectedOption == TWOPOS_1POS) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				} else if(currentlySelectedOption == TWOPOS_2POS) {
					currentlySelectedOption = TWOPOS_1POS;
					lcd_tft1p4705_set_selected_option(1, 0, 4);
				} else if(currentlySelectedOption == TWOPOS_RATE) {
					currentlySelectedOption = TWOPOS_2POS;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == TWOPOS_WAIT) {
					currentlySelectedOption = TWOPOS_RATE;
					lcd_tft1p4705_set_selected_option(1, 17, 21);
				} else if(currentlySelectedOption == TWOPOS_CYCLES) {
					currentlySelectedOption = TWOPOS_WAIT;
					lcd_tft1p4705_set_selected_option(2, 17, 21);
				}
				break;

			// up button pressed while on the 1Pos screen
			case ONE_POS:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = ONEPOS_RATE;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == ONEPOS_POSITION) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				} else if(currentlySelectedOption == ONEPOS_RATE) {
					currentlySelectedOption = ONEPOS_POSITION;
					lcd_tft1p4705_set_selected_option(1, 0, 4);
				}
				break;
		}
		
	} // end of exti5 (up button)

	/////////////////////////////////
	// DOWN BUTTON PRESSED, EXTI 6 //
	/////////////////////////////////

	if((EXTI->PR & (1<<6)) != 0) {	// interrupt occured on EXTI 6
		EXTI->PR = (1<<6);			// handling EXTI 6

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<6)) != 0) 
			return;

		switch(currentScreen) {

			// down button pressed while on the Sweep screen
			case SWEEP:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = SWEEP_START;
					lcd_tft1p4705_set_selected_option(1, 0, 5);
				} else if(currentlySelectedOption == SWEEP_START) {
					currentlySelectedOption = SWEEP_STOP;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == SWEEP_STOP) {
					currentlySelectedOption = SWEEP_RATE;
					lcd_tft1p4705_set_selected_option(1, 17, 21);
				} else if(currentlySelectedOption == SWEEP_RATE) {
					currentlySelectedOption = SWEEP_STEP;
					lcd_tft1p4705_set_selected_option(2, 17, 21);
				} else if(currentlySelectedOption == SWEEP_STEP) {
					currentlySelectedOption = SWEEP_CYCLES;
					lcd_tft1p4705_set_selected_option(3, 15, 21);
				} else if(currentlySelectedOption == SWEEP_CYCLES) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				}
				break;

			// down button pressed while on the 3Pos screen
			case THREE_POS:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = THREEPOS_1POS;
					lcd_tft1p4705_set_selected_option(1, 0, 4);
				} else if(currentlySelectedOption == THREEPOS_1POS) {
					currentlySelectedOption = THREEPOS_2POS;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == THREEPOS_2POS) {
					currentlySelectedOption = THREEPOS_3POS;
					lcd_tft1p4705_set_selected_option(3, 0, 4);
				} else if(currentlySelectedOption == THREEPOS_3POS) {
					currentlySelectedOption = THREEPOS_RATE;
					lcd_tft1p4705_set_selected_option(1, 17, 21);
				} else if(currentlySelectedOption == THREEPOS_RATE) {
					currentlySelectedOption = THREEPOS_WAIT;
					lcd_tft1p4705_set_selected_option(2, 17, 21);
				} else if(currentlySelectedOption == THREEPOS_WAIT) {
					currentlySelectedOption = THREEPOS_CYCLES;
					lcd_tft1p4705_set_selected_option(3, 15, 21);
				} else if(currentlySelectedOption == THREEPOS_CYCLES) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				}
				break;

			// down button pressed while on the 2Pos screen
			case TWO_POS:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = TWOPOS_1POS;
					lcd_tft1p4705_set_selected_option(1, 0, 4);
				} else if(currentlySelectedOption == TWOPOS_1POS) {
					currentlySelectedOption = TWOPOS_2POS;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == TWOPOS_2POS) {
					currentlySelectedOption = TWOPOS_RATE;
					lcd_tft1p4705_set_selected_option(1, 17, 21);
				} else if(currentlySelectedOption == TWOPOS_RATE) {
					currentlySelectedOption = TWOPOS_WAIT;
					lcd_tft1p4705_set_selected_option(2, 17, 21);
				} else if(currentlySelectedOption == TWOPOS_WAIT) {
					currentlySelectedOption = TWOPOS_CYCLES;
					lcd_tft1p4705_set_selected_option(3, 15, 21);
				} else if(currentlySelectedOption == TWOPOS_CYCLES) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				}
				break;

			// down button pressed while on the 1Pos screen
			case ONE_POS:
				if(currentlySelectedOption == MODE) {
					currentlySelectedOption = ONEPOS_POSITION;
					lcd_tft1p4705_set_selected_option(1, 0, 4);
				} else if(currentlySelectedOption == ONEPOS_POSITION) {
					currentlySelectedOption = ONEPOS_RATE;
					lcd_tft1p4705_set_selected_option(2, 0, 4);
				} else if(currentlySelectedOption == ONEPOS_RATE) {
					currentlySelectedOption = MODE;
					lcd_tft1p4705_set_selected_option(0, 0, 4);
				}
				break;
		}

	} // end of exti6 (down button)

	//////////////////////////////////
	// RIGHT BUTTON PRESSED, EXTI 7 //
	//////////////////////////////////

	if((EXTI->PR & (1<<7)) != 0) {	// interrupt occured on EXTI 7
		EXTI->PR = (1<<7);			// handling EXTI 7

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<7)) != 0) 
			return;

		switch(currentScreen) {

			// right button pressed while on the Sweep screen
			case SWEEP:
				if(currentlySelectedOption == MODE && currentScreen == SWEEP)
					{currentScreen = THREE_POS; lcd_tft1p4705_set_selected_mode(0, 14, 17); cyclesCompleted = 0;}
				else if(currentlySelectedOption == SWEEP_START && sweep_start < 22000)
					sweep_start += 5;
				else if(currentlySelectedOption == SWEEP_STOP && sweep_stop < 22000)
					sweep_stop += 5;
				else if(currentlySelectedOption == SWEEP_CYCLES && sweep_cycles < 9999999)
					sweep_cycles++;
				else if(currentlySelectedOption == SWEEP_RATE && sweep_rate < 30)
					{sweep_rate++; TIM2->ARR = (sweep_rate * 2000) - 1;}
				else if(currentlySelectedOption == SWEEP_STEP && sweep_step < 9999)
					sweep_step += 5;
				break;

			// right button pressed while on the 3Pos screen
			case THREE_POS:
				if(currentlySelectedOption == MODE && currentScreen == THREE_POS)
					{currentScreen = TWO_POS; lcd_tft1p4705_set_selected_mode(0, 20, 23); cyclesCompleted = 0;}
				else if(currentlySelectedOption == THREEPOS_1POS && threePos_firstPosition < 22000)
					threePos_firstPosition += 5;
				else if(currentlySelectedOption == THREEPOS_2POS && threePos_secondPosition < 22000)
					threePos_secondPosition += 5;
				else if(currentlySelectedOption == THREEPOS_3POS && threePos_thirdPosition < 22000)
					threePos_thirdPosition += 5;
				else if(currentlySelectedOption == THREEPOS_RATE && threePos_rate < 30)
					{threePos_rate++; TIM2->ARR = (threePos_rate * 2000) - 1;}
				else if(currentlySelectedOption == THREEPOS_WAIT && threePos_wait < 999)
					threePos_wait++;
				else if(currentlySelectedOption == THREEPOS_CYCLES && threePos_cycles < 9999999)
					threePos_cycles++;
				break;

			// right button pressed while on the 2Pos screen
			case TWO_POS:
				if(currentlySelectedOption == MODE && currentScreen == TWO_POS)
					{currentScreen = ONE_POS; lcd_tft1p4705_set_selected_mode(0, 26, 29); cyclesCompleted = 0;}
				else if(currentlySelectedOption == TWOPOS_1POS && twoPos_firstPosition < 22000)
					twoPos_firstPosition += 5;
				else if(currentlySelectedOption == TWOPOS_2POS && twoPos_secondPosition < 22000)
					twoPos_secondPosition += 5;
				else if(currentlySelectedOption == TWOPOS_RATE && twoPos_rate < 30)
					{twoPos_rate++; TIM2->ARR = (twoPos_rate * 2000) - 1;}
				else if(currentlySelectedOption == TWOPOS_WAIT && twoPos_wait < 999)
					twoPos_wait++;
				else if(currentlySelectedOption == TWOPOS_CYCLES && twoPos_cycles < 9999999)
					twoPos_cycles++;
				break;

			// right button pressed while on the 1Pos screen
			case ONE_POS:
				if(currentlySelectedOption == MODE && currentScreen == ONE_POS)
					{currentScreen = SWEEP; lcd_tft1p4705_set_selected_mode(0, 7, 11); cyclesCompleted = 0;}
				else if(currentlySelectedOption == ONEPOS_POSITION && onePos_position < 22000)
					onePos_position += 5;
				else if(currentlySelectedOption == ONEPOS_RATE && onePos_rate < 30)
					{onePos_rate++; TIM2->ARR = (onePos_rate * 2000) - 1;}
				break;
		}

	} // end of exti7 (right button)
}

/*********************************************************************************
 * ISR FOR PWM OUTPUT                                                            *
 *********************************************************************************/
void TIM2_IRQHandler() {
	static uint8_t direction = 0; // 0 = increasing, 1 = decreasing
	static uint8_t currentPosition = 0; // 0 = pos1, 1 = pos2, 2 = pos3
	static uint64_t timeElapsed = 0; // time in half microseconds
	
	if(TIM2->SR & TIM_SR_UIF != 0) {						// If update flag is set
		if(currentScreen == SWEEP) {
			if(sweep_cycles == 0 || cyclesCompleted < sweep_cycles) {
				if(direction == 0 && currentServoPulse < sweep_stop) {
					TIM2->CCR3 = (currentServoPulse / 5) + (sweep_step / 5);
					currentServoPulse += sweep_step;
				} else if(direction == 0 && currentServoPulse >= sweep_stop) {
					direction = 1;
				} else if(direction == 1 && currentServoPulse > sweep_start) {
					TIM2->CCR3 = (currentServoPulse / 5) - (sweep_step / 5);
					currentServoPulse -= sweep_step;
				} else if(direction == 1 && currentServoPulse <= sweep_start) {
					direction = 0;
					cyclesCompleted++;
				}
			}
		} else if(currentScreen == THREE_POS) {
			if(threePos_cycles == 0 || cyclesCompleted < threePos_cycles) {
				timeElapsed += TIM2->ARR;
				if(timeElapsed >= threePos_wait * (uint64_t)200000) { // time to change pos
					timeElapsed = 0;
					if(direction == 0 && currentPosition == 0) {
						currentPosition = 1;
						TIM2->CCR3 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					} else if(direction == 0 && currentPosition == 1) {
						currentPosition = 2;
						TIM2->CCR3 = threePos_thirdPosition / 5;
						currentServoPulse = threePos_thirdPosition;
					} else if(direction == 0 && currentPosition == 2) {
						direction = 1;
						currentPosition = 1;
						TIM2->CCR3 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM2->CCR3 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM2->CCR3 = threePos_firstPosition / 5;
						currentServoPulse = threePos_firstPosition;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 2) {
						currentPosition = 1;
						TIM2->CCR3 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					}
				}
			}
		} else if(currentScreen == TWO_POS) {
			if(currentPosition == 2)
				currentPosition = 0; // reset
			if(twoPos_cycles == 0 || cyclesCompleted < twoPos_cycles) {
				timeElapsed += TIM2->ARR;
				if(timeElapsed >= twoPos_wait * (uint64_t)200000) { // time to change pos
					timeElapsed = 0;
					if(direction == 0 && currentPosition == 0) {
						currentPosition = 1;
						TIM2->CCR3 = twoPos_secondPosition / 5;
						currentServoPulse = twoPos_secondPosition;
					} else if(direction == 0 && currentPosition == 1) {
						direction = 1;
						currentPosition = 0;
						TIM2->CCR3 = twoPos_firstPosition / 5;
						currentServoPulse = twoPos_firstPosition;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM2->CCR3 = twoPos_secondPosition / 5;
						currentServoPulse = twoPos_secondPosition;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM2->CCR3 = twoPos_firstPosition / 5;
						currentServoPulse = twoPos_firstPosition;
					}
				}
			}
		} else if(currentScreen == ONE_POS) {
			TIM2->CCR3 = onePos_position / 5;
			currentServoPulse = onePos_position;
		}
	}
	TIM2->SR &= ~TIM_SR_UIF;								// Interrupt has been handled
}
