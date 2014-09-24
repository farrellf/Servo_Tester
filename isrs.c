// Written by: Farrell Farahbod
// Last revised: 2014-09-16
// License: public domain

#include "stm32f0xx.h"
#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "f0lib/f0lib_timers.h"
#include "externs.h"
#include "settings.h"

void notifyCurrentScreen(enum Event event) {
	if(currentScreen == SWEEP)
		notifySweepScreen(event);
	else if(currentScreen == THREE_POS)
		notify3posScreen(event);
	else if(currentScreen == TWO_POS)
		notify2posScreen(event);
	else if(currentScreen == ONE_POS)
		notify1posScreen(event);
	else if(currentScreen == SPEC_AN_LIVE)
		notifySpecAnLiveScreen(event);
	else if(currentScreen == SPEC_AN_WATERFALL)
		notifySpecAnWaterfallScreen(event);
	else if(currentScreen == CALIBRATION)
		notifyCalibrationScreen(event);
	else if(currentScreen == GAME)
		notifyGameScreen(event);
}

/*
 * ADC ISR
 *
 * Reads and processes inputs from the touchscreen, current amps, battery voltage divider, and ADC/DAC breakout pin.
 * Touchscreen inputs are debounced and button press events are generated if appropriate.
 */
void ADC1_COMP_IRQHandler() {

	// Deadtime must be inserted when switching between X and Y axes of the touchscreen
	#define TOUCHSCREEN_WAIT 50
	
	static enum {CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9} state = CH0;
	static int32_t xl, xr, yu, yd;

	#define SAMPLE_SIZE 32
	static uint32_t voltageSamples[SAMPLE_SIZE] = {0};
	static uint32_t ch1currentSamples[SAMPLE_SIZE] = {0};
	static uint32_t ch2currentSamples[SAMPLE_SIZE] = {0};
	static uint32_t ch3currentSamples[SAMPLE_SIZE] = {0};
	static uint32_t ch4currentSamples[SAMPLE_SIZE] = {0};
	static uint32_t sample = 0;

	switch(state) {
		case CH0: // YD (reads x position)
			gpio_set_mode(TOUCHSCREEN_XL_PIN, ANALOG);
			gpio_set_mode(TOUCHSCREEN_XR_PIN, ANALOG);
			
			gpio_set_mode(TOUCHSCREEN_YU_PIN, OUTPUT);
			gpio_low(TOUCHSCREEN_YU_PIN);
			gpio_set_mode(TOUCHSCREEN_YD_PIN, OUTPUT);
			gpio_high(TOUCHSCREEN_YD_PIN);
			
			for(volatile uint32_t i = 0; i < TOUCHSCREEN_WAIT; i++);
			yd = ADC1->DR;
			state = CH1;
			break;

		case CH1: // XL (reads y position)
			gpio_set_mode(TOUCHSCREEN_YU_PIN, ANALOG);
			gpio_set_mode(TOUCHSCREEN_YD_PIN, ANALOG);
			
			gpio_set_mode(TOUCHSCREEN_XL_PIN, OUTPUT);
			gpio_low(TOUCHSCREEN_XL_PIN);
			gpio_set_mode(TOUCHSCREEN_XR_PIN, OUTPUT);
			gpio_high(TOUCHSCREEN_XR_PIN);
			
			for(volatile uint32_t i = 0; i < TOUCHSCREEN_WAIT; i++);
			xl = ADC1->DR;
			state = CH2;
			break;

		case CH2: // YU (reads x position)
			gpio_set_mode(TOUCHSCREEN_XL_PIN, ANALOG);
			gpio_set_mode(TOUCHSCREEN_XR_PIN, ANALOG);
			
			gpio_set_mode(TOUCHSCREEN_YU_PIN, OUTPUT);
			gpio_low(TOUCHSCREEN_YU_PIN);
			gpio_set_mode(TOUCHSCREEN_YD_PIN, OUTPUT);
			gpio_high(TOUCHSCREEN_YD_PIN);
			
			for(volatile uint32_t i = 0; i < TOUCHSCREEN_WAIT; i++);
			yu = ADC1->DR;
			state = CH3;
			break;

		case CH3: // XR (reads y position)
			gpio_set_mode(TOUCHSCREEN_YU_PIN, ANALOG);
			gpio_set_mode(TOUCHSCREEN_YD_PIN, ANALOG);
			
			gpio_set_mode(TOUCHSCREEN_XL_PIN, OUTPUT);
			gpio_low(TOUCHSCREEN_XL_PIN);
			gpio_set_mode(TOUCHSCREEN_XR_PIN, OUTPUT);
			gpio_high(TOUCHSCREEN_XR_PIN);
			
			for(volatile uint32_t i = 0; i < TOUCHSCREEN_WAIT; i++);
			xr = ADC1->DR;
			state = CH4;
			break;

		case CH4: // breakout pin
			breakoutPinRaw = ADC1->DR;
			state = CH5;
			break;

		case CH5: // voltageRaw voltage
			voltageSamples[sample] = ADC1->DR;
			state = CH6;
			break;

		case CH6: // ch1 current
			ch1currentSamples[sample] = ADC1->DR;
			state = CH7;
			break;

		case CH7: // ch2 current
			ch2currentSamples[sample] = ADC1->DR;
			state = CH8;
			break;

		case CH8: // ch3 current
			ch3currentSamples[sample] = ADC1->DR;
			state = CH9;
			break;

		case CH9: // ch4 current
			ch4currentSamples[sample] = ADC1->DR;
			state = CH0;
			break;
	}

	// convert from raw touchscreen ADC readings to a calibrated 0-3000 range for both X and Y
	if(currentScreen != GAME) {
		touchXraw = (int32_t) xl;
		touchYraw = (int32_t) yd;
	} else {
		touchXraw = (int32_t) yd;
		touchYraw = (int32_t) TOUCH_MAX_VALUE - xl;
	}
	touchX = (touchXraw - touchMinXCalibrationFactor) * TOUCH_MAX_VALUE / (touchMaxXCalibrationFactor - touchMinXCalibrationFactor);
	touchY = (touchYraw - touchMinYCalibrationFactor) * TOUCH_MAX_VALUE / (touchMaxYCalibrationFactor - touchMinYCalibrationFactor);

	// track if a touch is held down (for repeat "button press" generation)
	static uint32_t iteration = 1;
	static uint32_t repetitionRate = 512;
	static enum {LEFT, RIGHT, UP, DOWN, NONE} currentLocation, previousLocation = NONE;

	// speed up "button presses" when in the game screen
	if(currentScreen == GAME && repetitionRate == 512)
		repetitionRate = 128;
	
	// generate "button presses"
	uint8_t touchscreenTouched = touchXraw > TOUCH_MIN_RAW_VALUE && touchYraw > TOUCH_MIN_RAW_VALUE;
	if(touchscreenTouched) {
		iteration++;
		if(iteration % repetitionRate == 0 || iteration == 64) {
			if(repetitionRate > 10) repetitionRate-=10;
			uint8_t upperRight = touchY > TOUCH_MAX_VALUE*12/10 - (touchX*14/10);
			uint8_t lowerLeft  = touchY < TOUCH_MAX_VALUE*12/10 - (touchX*14/10);
			uint8_t upperLeft  = touchY > (touchX*14/10) - (TOUCH_MAX_VALUE*2/10);
			uint8_t lowerRight = touchY < (touchX*14/10) - (TOUCH_MAX_VALUE*2/10);

			// update previous and current touch locations
			previousLocation = currentLocation;
			if(upperLeft && lowerLeft)
				currentLocation = LEFT;
			else if(upperRight && lowerRight)
				currentLocation = RIGHT;
			else if(lowerLeft && lowerRight)
				currentLocation = DOWN;
			else if(upperLeft && upperRight)
				currentLocation = UP;

			if(previousLocation != currentLocation && previousLocation != NONE) {
				// spurious reading or touch slid into other quadrant
				previousLocation = currentLocation = NONE;
				iteration = 1;
				repetitionRate = 512;
				return;
			}
			
			if(currentLocation == LEFT)
				notifyCurrentScreen(LEFT_BUTTON_PRESSED);
			else if(currentLocation == RIGHT)
				notifyCurrentScreen(RIGHT_BUTTON_PRESSED);
			else if(currentLocation == DOWN)
				notifyCurrentScreen(DOWN_BUTTON_PRESSED);
			else if(currentLocation == UP)
				notifyCurrentScreen(UP_BUTTON_PRESSED);
		}
	} else {
		iteration = 1;
		repetitionRate = 512;
	}

	// update globals with rolling averages after every 8th sample
	if(sample % 8 == 0) {
		uint32_t avg = 0;

		// voltage
		for(uint32_t i = 0; i < SAMPLE_SIZE; i++)
			avg += voltageSamples[i];
		avg /= SAMPLE_SIZE;
		voltageRaw = avg;
		voltage = avg * 5000 / voltageCalibrationFactor;

		// ch1 current
		avg = 0;
		for(uint32_t i = 0; i < SAMPLE_SIZE; i++)
			avg += ch1currentSamples[i];
		avg /= SAMPLE_SIZE;
		current1raw = avg;
		current1 = avg * 1000 / current1CalibrationFactor;

		// ch2 current
		avg = 0;
		for(uint32_t i = 0; i < SAMPLE_SIZE; i++)
			avg += ch2currentSamples[i];
		avg /= SAMPLE_SIZE;
		current2raw = avg;
		current2 = avg * 1000 / current2CalibrationFactor;

		// ch3 current
		avg = 0;
		for(uint32_t i = 0; i < SAMPLE_SIZE; i++)
			avg += ch3currentSamples[i];
		avg /= SAMPLE_SIZE;
		current3raw = avg;
		current3 = avg * 1000 / current3CalibrationFactor;

		// ch4 current
		avg = 0;
		for(uint32_t i = 0; i < SAMPLE_SIZE; i++)
			avg += ch4currentSamples[i];
		avg /= SAMPLE_SIZE;
		current4raw = avg;
		current4 = avg * 1000 / current4CalibrationFactor;
	}
		
	if(sample == SAMPLE_SIZE - 1 && state == CH0)
		sample = 0;
	else if(state == CH0)
		sample++;

	// for debugging:
	//lcd_printf(0, 8, "xl=%p40xr=%p40yu=%p40yd=%p40", xl, xr, yu, yd);
	//lcd_printf(0, 9, "touchX = %p40  touchY = %p40", touchX, touchY);
}


// EXTI and TIM3 ISRs share this variable:
volatile uint32_t buttonHeldDown = 0;


/*
 * EXTI ISR (for push buttons)
 */
void EXTI4_15_IRQHandler() {

	#define SWITCH_BOUNCE_TIME 25000

	// only debounce inital button presses
	if(!buttonHeldDown)
		for(volatile uint32_t i = 0; i < SWITCH_BOUNCE_TIME; i++);

	// exti11 (left button)
	if((EXTI->PR & (1<<11)) != 0) {	// interrupt occured on EXTI 11
		EXTI->PR = (1<<11); // handling EXTI 11

		if((GPIOA->IDR & (1<<11)) != 0) 
			return;

		// notify screen
		notifyCurrentScreen(LEFT_BUTTON_PRESSED);
	}

	// exti12 (right button)
	if((EXTI->PR & (1<<12)) != 0) {	// interrupt occured on EXTI 12
		EXTI->PR = (1<<12); // handling EXTI 12

		if((GPIOA->IDR & (1<<12)) != 0) 
			return;

		// notify screen
		notifyCurrentScreen(RIGHT_BUTTON_PRESSED);
	}

	// exti6 (up button)
	if((EXTI->PR & (1<<6)) != 0) {	// interrupt occured on EXTI 6
		EXTI->PR = (1<<6);			// handling EXTI 6

		if((GPIOF->IDR & (1<<6)) != 0) 
			return;

		// notify screen
		notifyCurrentScreen(UP_BUTTON_PRESSED);
	}

	// exti7 (down button)
	if((EXTI->PR & (1<<7)) != 0) {	// interrupt occured on EXTI 7
		EXTI->PR = (1<<7);			// handling EXTI 7

		if((GPIOF->IDR & (1<<7)) != 0) 
			return;

		// notify screen
		notifyCurrentScreen(DOWN_BUTTON_PRESSED);
	}

	// have TIM3 check if a button is held down after some period of time, TIM3 will trigger an EXTI if needed.
	static uint32_t waitTime = 256; // ms
	if(!buttonHeldDown) waitTime = 256;
	if(buttonHeldDown && waitTime > 1) waitTime -= waitTime / 8;
	timer_one_pulse_setup(TIM3, waitTime);
	
}

/*
 * TIM3 is used to generate repeat button presses when a button is held down.
 */
void TIM3_IRQHandler(void) {
	if(TIM3->SR & TIM_SR_UIF != 0) { // If update flag is set
		TIM3->SR &= ~TIM_SR_UIF; // Interrupt has been handled

		// trigger an exti if a button is held down
		if((GPIOA->IDR & (1 << 11)) == 0) {
			buttonHeldDown = 1;
			EXTI->SWIER = EXTI_SWIER_SWIER11;
		} else if((GPIOF->IDR & (1 << 6)) == 0) {
			buttonHeldDown = 1;
			EXTI->SWIER = EXTI_SWIER_SWIER6;
		} else if((GPIOF->IDR & (1 << 7)) == 0) {
			buttonHeldDown = 1;
			EXTI->SWIER = EXTI_SWIER_SWIER7;
		} else if((GPIOA->IDR & (1 << 12)) == 0) {
			buttonHeldDown = 1;
			EXTI->SWIER = EXTI_SWIER_SWIER12;
		} else {
			buttonHeldDown = 0;
		}
	}
}

// ISR for PWM output
void TIM2_IRQHandler() {
	static uint8_t direction = 0; // 0 = increasing, 1 = decreasing
	static uint8_t currentPosition = 0; // 0 = pos1, 1 = pos2, 2 = pos3
	static uint64_t timeElapsed = 0; // time in half microseconds
	
	if(TIM2->SR & TIM_SR_UIF != 0) { // if update flag is set
		if(currentScreen == SWEEP) {
			if(sweep_cycles == 0 || cyclesCompleted < sweep_cycles) {
				if(direction == 0 && currentServoPulse < sweep_pos2) {
					TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = (currentServoPulse / 5) + (sweep_step / 5);
					currentServoPulse += sweep_step;
				} else if(direction == 0 && currentServoPulse >= sweep_pos2) {
					direction = 1;
				} else if(direction == 1 && currentServoPulse > sweep_pos1) {
					TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = (currentServoPulse / 5) - (sweep_step / 5);
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
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = threePos_pos2 / 5;
						currentServoPulse = threePos_pos2;
					} else if(direction == 0 && currentPosition == 1) {
						currentPosition = 2;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = threePos_pos3 / 5;
						currentServoPulse = threePos_pos3;
					} else if(direction == 0 && currentPosition == 2) {
						direction = 1;
						currentPosition = 1;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = threePos_pos2 / 5;
						currentServoPulse = threePos_pos2;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = threePos_pos2 / 5;
						currentServoPulse = threePos_pos2;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = threePos_pos1 / 5;
						currentServoPulse = threePos_pos1;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 2) {
						currentPosition = 1;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = threePos_pos2 / 5;
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
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = twoPos_pos2 / 5;
						currentServoPulse = twoPos_pos2;
					} else if(direction == 0 && currentPosition == 1) {
						direction = 1;
						currentPosition = 0;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = twoPos_pos1 / 5;
						currentServoPulse = twoPos_pos1;
						cyclesCompleted++;
					} else if(direction == 1 && currentPosition == 0) {
						direction = 0;
						currentPosition = 1;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = twoPos_pos2 / 5;
						currentServoPulse = twoPos_pos2;
					} else if(direction == 1 && currentPosition == 1) {
						currentPosition = 0;
						TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = twoPos_pos1 / 5;
						currentServoPulse = twoPos_pos1;
					}
				}
			}
		} else if(currentScreen == ONE_POS) {
			TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = onePos_position / 5;
			currentServoPulse = onePos_position;
		}
	}
	TIM2->SR &= ~TIM_SR_UIF; // interrupt has been handled
}
