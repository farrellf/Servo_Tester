// Written by Farrell Farahbod
// Last revised on 2014-07-09
// This file is released into the public domain

#include "stm32f0xx.h"

#include "config.h"

// ISR for push buttons (EXTI 4,5,6,7)
#define SWITCH_BOUNCE_TIME 250000
void EXTI4_15_IRQHandler() {
	if((EXTI->PR & (1<<4)) != 0) {	// interrupt occured on EXTI 4
		EXTI->PR = (1<<4);			// handling EXTI 4

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<4)) != 0) 
			return;
			
		// exti4 (left button)
		if(currentScreen == SWEEP)
			notifySweepScreen(LEFT_BUTTON_PRESSED);
		else if(currentScreen == THREE_POS)
			notify3posScreen(LEFT_BUTTON_PRESSED);
		else if(currentScreen == TWO_POS)
			notify2posScreen(LEFT_BUTTON_PRESSED);
		else if(currentScreen == ONE_POS)
			notify1posScreen(LEFT_BUTTON_PRESSED);
	}

	if((EXTI->PR & (1<<5)) != 0) {	// interrupt occured on EXTI 5
		EXTI->PR = (1<<5);			// handling EXTI 5

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<5)) != 0) 
			return;

		// exti5 (up button)
		if(currentScreen == SWEEP)
			notifySweepScreen(UP_BUTTON_PRESSED);
		else if(currentScreen == THREE_POS)
			notify3posScreen(UP_BUTTON_PRESSED);
		else if(currentScreen == TWO_POS)
			notify2posScreen(UP_BUTTON_PRESSED);
		else if(currentScreen == ONE_POS)
			notify1posScreen(UP_BUTTON_PRESSED);
	}

	if((EXTI->PR & (1<<6)) != 0) {	// interrupt occured on EXTI 6
		EXTI->PR = (1<<6);			// handling EXTI 6

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<6)) != 0) 
			return;

		// exti6 (down button)
		if(currentScreen == SWEEP)
			notifySweepScreen(DOWN_BUTTON_PRESSED);
		else if(currentScreen == THREE_POS)
			notify3posScreen(DOWN_BUTTON_PRESSED);
		else if(currentScreen == TWO_POS)
			notify2posScreen(DOWN_BUTTON_PRESSED);
		else if(currentScreen == ONE_POS)
			notify1posScreen(DOWN_BUTTON_PRESSED);
	}

	if((EXTI->PR & (1<<7)) != 0) {	// interrupt occured on EXTI 7
		EXTI->PR = (1<<7);			// handling EXTI 7

		// switch debounce
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);
		if((GPIOB->IDR & (1<<7)) != 0) 
			return;

		// exti7 (right button)
		if(currentScreen == SWEEP)
			notifySweepScreen(RIGHT_BUTTON_PRESSED);
		else if(currentScreen == THREE_POS)
			notify3posScreen(RIGHT_BUTTON_PRESSED);
		else if(currentScreen == TWO_POS)
			notify2posScreen(RIGHT_BUTTON_PRESSED);
		else if(currentScreen == ONE_POS)
			notify1posScreen(RIGHT_BUTTON_PRESSED);
	}
}

// ISR for PWM output
void TIM2_IRQHandler() {
	static uint8_t direction = 0; // 0 = increasing, 1 = decreasing
	static uint8_t currentPosition = 0; // 0 = pos1, 1 = pos2, 2 = pos3
	static uint64_t timeElapsed = 0; // time in half microseconds
	
	if(TIM2->SR & TIM_SR_UIF != 0) {						// If update flag is set
		if(currentScreen == SWEEP) {
			if(sweep_cycles == 0 || cyclesCompleted < sweep_cycles) {
				if(direction == 0 && currentServoPulse < sweep_pos2) {
					TIM2->CCR3 = (currentServoPulse / 5) + (sweep_step / 5);
					currentServoPulse += sweep_step;
				} else if(direction == 0 && currentServoPulse >= sweep_pos2) {
					direction = 1;
				} else if(direction == 1 && currentServoPulse > sweep_pos1) {
					TIM2->CCR3 = (currentServoPulse / 5) - (sweep_step / 5);
					currentServoPulse -= sweep_step;
				} else if(direction == 1 && currentServoPulse <= sweep_pos1) {
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
						TIM2->CCR3 = threePos_pos2 / 5;
						currentServoPulse = threePos_pos2;
					} else if(direction == 0 && currentPosition == 1) {
						currentPosition = 2;
						TIM2->CCR3 = threePos_pos3 / 5;
						currentServoPulse = threePos_pos3;
					} else if(direction == 0 && currentPosition == 2) {
						direction = 1;
						currentPosition = 1;
						TIM2->CCR3 = threePos_pos2 / 5;
						currentServoPulse = threePos_pos2;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM2->CCR3 = threePos_pos2 / 5;
						currentServoPulse = threePos_pos2;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM2->CCR3 = threePos_pos1 / 5;
						currentServoPulse = threePos_pos1;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 2) {
						currentPosition = 1;
						TIM2->CCR3 = threePos_pos2 / 5;
						currentServoPulse = threePos_pos2;
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
						TIM2->CCR3 = twoPos_pos2 / 5;
						currentServoPulse = twoPos_pos2;
					} else if(direction == 0 && currentPosition == 1) {
						direction = 1;
						currentPosition = 0;
						TIM2->CCR3 = twoPos_pos1 / 5;
						currentServoPulse = twoPos_pos1;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM2->CCR3 = twoPos_pos2 / 5;
						currentServoPulse = twoPos_pos2;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM2->CCR3 = twoPos_pos1 / 5;
						currentServoPulse = twoPos_pos1;
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
