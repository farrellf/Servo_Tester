#include "externs.h"

/*********************************************************************************
 * ISR FOR LEFT ARROW (DECREASE) BUTTON. TIM15CH1, PA2.                          *
 *********************************************************************************/
void TIM15_IRQHandler() {
	static uint32_t callsSinceReset = 0;					// Track duration of button press

	if(TIM15->SR & TIM_SR_UIF != 0)							// If update flag is set
		if((GPIOA->IDR & (1 << 2)) != 0) {					// Pin is high
			if(currentlySelectedOption == MODE && currentMode == SWEEP) {
				currentMode = ONE_POS;
				cyclesCompleted = 0;
			} else if(currentlySelectedOption == MODE && currentMode == THREE_POS) {
				currentMode = SWEEP;
				cyclesCompleted = 0;
			} else if(currentlySelectedOption == MODE && currentMode == TWO_POS) {
				currentMode = THREE_POS;
				cyclesCompleted = 0;
			} else if(currentlySelectedOption == MODE && currentMode == ONE_POS) {
				currentMode = TWO_POS;	
				cyclesCompleted = 0;
			}
			
			else if(currentlySelectedOption == SWEEP_START && sweep_start > 8000)
				sweep_start -= 5;
			else if(currentlySelectedOption == SWEEP_STOP && sweep_stop > 8000)
				sweep_stop -= 5;
			else if(currentlySelectedOption == SWEEP_CYCLES && sweep_cycles > 0)
				sweep_cycles--;
			else if(currentlySelectedOption == SWEEP_RATE && sweep_rate > 3)
				{sweep_rate--; TIM1->ARR = (sweep_rate * 2000) - 1;}
			else if(currentlySelectedOption == SWEEP_STEP && sweep_step > 1)
				sweep_step -= 5;
				
			else if(currentlySelectedOption == THREEPOS_1POS && threePos_firstPosition > 8000)
				threePos_firstPosition -= 5;
			else if(currentlySelectedOption == THREEPOS_2POS && threePos_secondPosition > 8000)
				threePos_secondPosition -= 5;
			else if(currentlySelectedOption == THREEPOS_3POS && threePos_thirdPosition > 8000)
				threePos_thirdPosition -= 5;
			else if(currentlySelectedOption == THREEPOS_RATE && threePos_rate > 3)
				{threePos_rate--; TIM1->ARR = (threePos_rate * 2000) - 1;}
			else if(currentlySelectedOption == THREEPOS_WAIT && threePos_wait > 1)
				threePos_wait--;
			else if(currentlySelectedOption == THREEPOS_CYCLES && threePos_cycles > 0)
				threePos_cycles--;
				
			else if(currentlySelectedOption == TWOPOS_1POS && twoPos_firstPosition > 8000)
				twoPos_firstPosition -= 5;
			else if(currentlySelectedOption == TWOPOS_2POS && twoPos_secondPosition > 8000)
				twoPos_secondPosition -= 5;
			else if(currentlySelectedOption == TWOPOS_RATE && twoPos_rate > 3)
				{twoPos_rate--; TIM1->ARR = (twoPos_rate * 2000) - 1;}
			else if(currentlySelectedOption == TWOPOS_WAIT && twoPos_wait > 1)
				twoPos_wait--;
			else if(currentlySelectedOption == TWOPOS_CYCLES && twoPos_cycles > 0)
				twoPos_cycles--;
				
			else if(currentlySelectedOption == ONEPOS_POSITION && onePos_position > 8000)
				onePos_position -= 5;
			else if(currentlySelectedOption == ONEPOS_RATE && onePos_rate > 3)
				{onePos_rate--; TIM1->ARR = (onePos_rate * 2000) - 1;}

			callsSinceReset++;
			if(callsSinceReset > 5 && callsSinceReset <= 20)		TIM15->ARR = 499;
			else if(callsSinceReset > 20 && callsSinceReset <= 150)	TIM15->ARR = 79;
			else if(callsSinceReset > 150)							TIM15->ARR = 9;
		} else {											// Pin is low
			callsSinceReset = 0;
			TIM15->ARR = 999;
		}
	TIM15->SR &= ~TIM_SR_UIF;								// interrupt handled
}

/*********************************************************************************
 * ISR FOR RIGHT ARROW BUTTON. TIM2CH1, PA15.                                    *
 *********************************************************************************/
void TIM2_IRQHandler() {
	static uint32_t callsSinceReset = 0;					// Track duration of button press
	
	if(TIM2->SR & TIM_SR_UIF != 0)							// If update flag is set
		if((GPIOA->IDR & (1 << 15)) != 0) {					// Pin is high
			if(currentlySelectedOption == MODE && currentMode == SWEEP) {
				currentMode = THREE_POS;
				cyclesCompleted = 0;
			} else if(currentlySelectedOption == MODE && currentMode == THREE_POS) {
				currentMode = TWO_POS;
				cyclesCompleted = 0;
			} else if(currentlySelectedOption == MODE && currentMode == TWO_POS) {
				currentMode = ONE_POS;
				cyclesCompleted = 0;
			} else if(currentlySelectedOption == MODE && currentMode == ONE_POS) {
				currentMode = SWEEP;
				cyclesCompleted = 0;	
			}
			
			else if(currentlySelectedOption == SWEEP_START && sweep_start < 22000)
				sweep_start += 5;
			else if(currentlySelectedOption == SWEEP_STOP && sweep_stop < 22000)
				sweep_stop += 5;
			else if(currentlySelectedOption == SWEEP_CYCLES && sweep_cycles < 9999999)
				sweep_cycles++;
			else if(currentlySelectedOption == SWEEP_RATE && sweep_rate < 30)
				{sweep_rate++; TIM1->ARR = (sweep_rate * 2000) - 1;}
			else if(currentlySelectedOption == SWEEP_STEP && sweep_step < 9999)
				sweep_step += 5;
				
							
			else if(currentlySelectedOption == THREEPOS_1POS && threePos_firstPosition < 22000)
				threePos_firstPosition += 5;
			else if(currentlySelectedOption == THREEPOS_2POS && threePos_secondPosition < 22000)
				threePos_secondPosition += 5;
			else if(currentlySelectedOption == THREEPOS_3POS && threePos_thirdPosition < 22000)
				threePos_thirdPosition += 5;
			else if(currentlySelectedOption == THREEPOS_RATE && threePos_rate < 30)
				{threePos_rate++; TIM1->ARR = (threePos_rate * 2000) - 1;}
			else if(currentlySelectedOption == THREEPOS_WAIT && threePos_wait < 999)
				threePos_wait++;
			else if(currentlySelectedOption == THREEPOS_CYCLES && threePos_cycles < 9999999)
				threePos_cycles++;
				
			else if(currentlySelectedOption == TWOPOS_1POS && twoPos_firstPosition < 22000)
				twoPos_firstPosition += 5;
			else if(currentlySelectedOption == TWOPOS_2POS && twoPos_secondPosition < 22000)
				twoPos_secondPosition += 5;
			else if(currentlySelectedOption == TWOPOS_RATE && twoPos_rate < 30)
				{twoPos_rate++; TIM1->ARR = (twoPos_rate * 2000) - 1;}
			else if(currentlySelectedOption == TWOPOS_WAIT && twoPos_wait < 999)
				twoPos_wait++;
			else if(currentlySelectedOption == TWOPOS_CYCLES && twoPos_cycles < 9999999)
				twoPos_cycles++;
				
			else if(currentlySelectedOption == ONEPOS_POSITION && onePos_position < 22000)
				onePos_position += 5;
			else if(currentlySelectedOption == ONEPOS_RATE && onePos_rate < 30)
				{onePos_rate++; TIM1->ARR = (onePos_rate * 2000) - 1;}

			callsSinceReset++;
			if(callsSinceReset > 5 && callsSinceReset <= 20)		TIM2->ARR = 499;
			else if(callsSinceReset > 20 && callsSinceReset <= 150)	TIM2->ARR = 79;
			else if(callsSinceReset > 150)							TIM2->ARR = 9;			
		} else {											// Pin is low
			callsSinceReset = 0;
			TIM2->ARR = 999;
		}
	TIM2->SR &= ~TIM_SR_UIF;								// interrupt handled
}

/*********************************************************************************
 * ISR FOR UP ARROW BUTTON. TIM14CH1, PA4.                                       *
 *********************************************************************************/
void TIM14_IRQHandler() {
	
	static uint32_t callsSinceReset = 0;					// Track duration of button press

	if(TIM14->SR & TIM_SR_UIF != 0)							// If update flag is set
		if((GPIOA->IDR & (1 << 4)) != 0) {					// Pin is high
			if(callsSinceReset == 0) {
				if(currentlySelectedOption == MODE && currentMode == SWEEP)
					currentlySelectedOption = SWEEP_CYCLES;
				else if(currentlySelectedOption == MODE && currentMode == THREE_POS)
					currentlySelectedOption = THREEPOS_CYCLES;
				else if(currentlySelectedOption == MODE && currentMode == TWO_POS)
					currentlySelectedOption = TWOPOS_CYCLES;
				else if(currentlySelectedOption == MODE && currentMode == ONE_POS)
					currentlySelectedOption = ONEPOS_RATE;
					
				else if(currentlySelectedOption == SWEEP_START)
					currentlySelectedOption = MODE;
				else if(currentlySelectedOption == SWEEP_STOP)
					currentlySelectedOption = SWEEP_START;
				else if(currentlySelectedOption == SWEEP_RATE)
					currentlySelectedOption = SWEEP_STOP;
				else if(currentlySelectedOption == SWEEP_STEP)
					currentlySelectedOption = SWEEP_RATE;
				else if(currentlySelectedOption == SWEEP_CYCLES)
					currentlySelectedOption = SWEEP_STEP;
					
				else if(currentlySelectedOption == THREEPOS_1POS)
					currentlySelectedOption = MODE;
				else if(currentlySelectedOption == THREEPOS_2POS)
					currentlySelectedOption = THREEPOS_1POS;
				else if(currentlySelectedOption == THREEPOS_3POS)
					currentlySelectedOption = THREEPOS_2POS;
				else if(currentlySelectedOption == THREEPOS_RATE)
					currentlySelectedOption = THREEPOS_3POS;
				else if(currentlySelectedOption == THREEPOS_WAIT)
					currentlySelectedOption = THREEPOS_RATE;
				else if(currentlySelectedOption == THREEPOS_CYCLES)
					currentlySelectedOption = THREEPOS_WAIT;
					
				else if(currentlySelectedOption == TWOPOS_1POS)
					currentlySelectedOption = MODE;
				else if(currentlySelectedOption == TWOPOS_2POS)
					currentlySelectedOption = TWOPOS_1POS;
				else if(currentlySelectedOption == TWOPOS_RATE)
					currentlySelectedOption = TWOPOS_2POS;
				else if(currentlySelectedOption == TWOPOS_WAIT)
					currentlySelectedOption = TWOPOS_RATE;
				else if(currentlySelectedOption == TWOPOS_CYCLES)
					currentlySelectedOption = TWOPOS_WAIT;
					
				else if(currentlySelectedOption == ONEPOS_POSITION)
					currentlySelectedOption = MODE;
				else if(currentlySelectedOption == ONEPOS_RATE)
					currentlySelectedOption = ONEPOS_POSITION;
				
				drawUnderline = 1;
				TIM6->CNT = 0;
			}
			callsSinceReset++;
		} else {											// Pin is low
			callsSinceReset = 0;
		}
	TIM14->SR &= ~TIM_SR_UIF;								// interrupt handled
}

/*********************************************************************************
 * ISR FOR DOWN ARROW BUTTON. TIM16CH1, PA6.                                    *
 *********************************************************************************/
void TIM16_IRQHandler() {
	static uint32_t callsSinceReset = 0;					// Track duration of button press

	if(TIM16->SR & TIM_SR_UIF != 0)							// If update flag is set
		if((GPIOA->IDR & (1 << 6)) != 0) {					// Pin is high
			if(callsSinceReset == 0) {
				if(currentlySelectedOption == MODE && currentMode == SWEEP)
					currentlySelectedOption = SWEEP_START;
				else if(currentlySelectedOption == MODE && currentMode == THREE_POS)
					currentlySelectedOption = THREEPOS_1POS;
				else if(currentlySelectedOption == MODE && currentMode == TWO_POS)
					currentlySelectedOption = TWOPOS_1POS;
				else if(currentlySelectedOption == MODE && currentMode == ONE_POS)
					currentlySelectedOption = ONEPOS_POSITION;
					
				else if(currentlySelectedOption == SWEEP_START)
					currentlySelectedOption = SWEEP_STOP;
				else if(currentlySelectedOption == SWEEP_STOP)
					currentlySelectedOption = SWEEP_RATE;
				else if(currentlySelectedOption == SWEEP_RATE)
					currentlySelectedOption = SWEEP_STEP;
				else if(currentlySelectedOption == SWEEP_STEP)
					currentlySelectedOption = SWEEP_CYCLES;
				else if(currentlySelectedOption == SWEEP_CYCLES)
					currentlySelectedOption = MODE;
					
				else if(currentlySelectedOption == THREEPOS_1POS)
					currentlySelectedOption = THREEPOS_2POS;
				else if(currentlySelectedOption == THREEPOS_2POS)
					currentlySelectedOption = THREEPOS_3POS;
				else if(currentlySelectedOption == THREEPOS_3POS)
					currentlySelectedOption = THREEPOS_RATE;
				else if(currentlySelectedOption == THREEPOS_RATE)
					currentlySelectedOption = THREEPOS_WAIT;
				else if(currentlySelectedOption == THREEPOS_WAIT)
					currentlySelectedOption = THREEPOS_CYCLES;
				else if(currentlySelectedOption == THREEPOS_CYCLES)
					currentlySelectedOption = MODE;
					
				else if(currentlySelectedOption == TWOPOS_1POS)
					currentlySelectedOption = TWOPOS_2POS;
				else if(currentlySelectedOption == TWOPOS_2POS)
					currentlySelectedOption = TWOPOS_RATE;
				else if(currentlySelectedOption == TWOPOS_RATE)
					currentlySelectedOption = TWOPOS_WAIT;
				else if(currentlySelectedOption == TWOPOS_WAIT)
					currentlySelectedOption = TWOPOS_CYCLES;
				else if(currentlySelectedOption == TWOPOS_CYCLES)
					currentlySelectedOption = MODE;
					
				else if(currentlySelectedOption == ONEPOS_POSITION)
					currentlySelectedOption = ONEPOS_RATE;
				else if(currentlySelectedOption == ONEPOS_RATE)
					currentlySelectedOption = MODE;

				drawUnderline = 1;
				TIM6->CNT = 0;
			}
			callsSinceReset++;
		} else {											// Pin is low
			callsSinceReset = 0;
		}
	TIM16->SR &= ~TIM_SR_UIF;								// interrupt handled
}

/*********************************************************************************
 * ISR FOR BLINKING UNDERLINE                                                    *
 *********************************************************************************/
void TIM6_DAC_IRQHandler() {
	if(TIM6->SR & TIM_SR_UIF != 0) {						// If update flag is set
		if(drawUnderline == 0)
			drawUnderline = 1;
		else
			drawUnderline = 0;
	}
	TIM6->SR &= ~TIM_SR_UIF;								// Interrupt has been handled
}

/*********************************************************************************
 * ISR FOR PWM OUTPUT                                                            *
 *********************************************************************************/
void TIM1_BRK_UP_TRG_COM_IRQHandler() {
	static uint8_t direction = 0; // 0 = increasing, 1 = decreasing
	static uint8_t currentPosition = 0; // 0 = pos1, 1 = pos2, 2 = pos3
	static uint64_t timeElapsed = 0; // time in half microseconds
	
	if(TIM1->SR & TIM_SR_UIF != 0) {						// If update flag is set
		if(currentMode == SWEEP) {
			if(sweep_cycles == 0 || cyclesCompleted < sweep_cycles) {
				if(direction == 0 && currentServoPulse < sweep_stop) {
					TIM1->CCR1 = (currentServoPulse / 5) + (sweep_step / 5);
					currentServoPulse += sweep_step;
				} else if(direction == 0 && currentServoPulse >= sweep_stop) {
					direction = 1;
				} else if(direction == 1 && currentServoPulse > sweep_start) {
					TIM1->CCR1 = (currentServoPulse / 5) - (sweep_step / 5);
					currentServoPulse -= sweep_step;
				} else if(direction == 1 && currentServoPulse <= sweep_start) {
					direction = 0;
					cyclesCompleted++;
				}
			}
		} else if(currentMode == THREE_POS) {
			if(threePos_cycles == 0 || cyclesCompleted < threePos_cycles) {
				timeElapsed += TIM1->ARR;
				if(timeElapsed >= threePos_wait * (uint64_t)200000) { // time to change pos
					timeElapsed = 0;
					if(direction == 0 && currentPosition == 0) {
						currentPosition = 1;
						TIM1->CCR1 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					} else if(direction == 0 && currentPosition == 1) {
						currentPosition = 2;
						TIM1->CCR1 = threePos_thirdPosition / 5;
						currentServoPulse = threePos_thirdPosition;
					} else if(direction == 0 && currentPosition == 2) {
						direction = 1;
						currentPosition = 1;
						TIM1->CCR1 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM1->CCR1 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM1->CCR1 = threePos_firstPosition / 5;
						currentServoPulse = threePos_firstPosition;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 2) {
						currentPosition = 1;
						TIM1->CCR1 = threePos_secondPosition / 5;
						currentServoPulse = threePos_secondPosition;
					}
				}
			}
		} else if(currentMode == TWO_POS) {
			if(currentPosition == 2)
				currentPosition = 0; // reset
			if(twoPos_cycles == 0 || cyclesCompleted < twoPos_cycles) {
				timeElapsed += TIM1->ARR;
				if(timeElapsed >= twoPos_wait * (uint64_t)200000) { // time to change pos
					timeElapsed = 0;
					if(direction == 0 && currentPosition == 0) {
						currentPosition = 1;
						TIM1->CCR1 = twoPos_secondPosition / 5;
						currentServoPulse = twoPos_secondPosition;
					} else if(direction == 0 && currentPosition == 1) {
						direction = 1;
						currentPosition = 0;
						TIM1->CCR1 = twoPos_firstPosition / 5;
						currentServoPulse = twoPos_firstPosition;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM1->CCR1 = twoPos_secondPosition / 5;
						currentServoPulse = twoPos_secondPosition;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM1->CCR1 = twoPos_firstPosition / 5;
						currentServoPulse = twoPos_firstPosition;
					}
				}
			}
		} else if(currentMode == ONE_POS) {
			TIM1->CCR1 = onePos_position / 5;
			currentServoPulse = onePos_position;
		}
	}
	TIM1->SR &= ~TIM_SR_UIF;								// Interrupt has been handled
}
