// Written by Farrell Farahbod
// Last revised on 2014-07-03
// This file is released into the public domain

#include "stm32f0xx.h"
#include "f0lib/f0lib.h"
#include "screens.h"

enum Screen {SWEEP, THREE_POS, TWO_POS, ONE_POS};
enum SelectedOption	{MODE,
					SWEEP_START, SWEEP_STOP, SWEEP_CYCLES, SWEEP_RATE, SWEEP_STEP,
					THREEPOS_1POS, THREEPOS_2POS, THREEPOS_3POS, THREEPOS_RATE, THREEPOS_WAIT, THREEPOS_CYCLES,
					TWOPOS_1POS, TWOPOS_2POS, TWOPOS_RATE, TWOPOS_WAIT, TWOPOS_CYCLES,
					ONEPOS_POSITION, ONEPOS_RATE};

// Defaults
volatile enum Screen currentScreen = SWEEP;
volatile enum SelectedOption currentlySelectedOption = MODE;
volatile uint32_t cyclesCompleted = 0;
volatile uint16_t currentServoPulse = 9000;			// valid range = 8000 - 22000 (800 - 2200us)
volatile uint8_t drawUnderline = 0;					// 0 or 1 to draw or not draw an underline below the currently selected item

// Default state for each mode
volatile uint16_t sweep_start = 9000;				// valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t sweep_stop = 21000;				// valid range = 8000 - 22000 (800 - 2200us)
volatile uint32_t sweep_cycles = 0;					// valid range = 0 - 9999999, 0 = no limit
volatile uint16_t sweep_rate = 10;					// valud range = 3 - 30 (3 - 30ms)
volatile uint16_t sweep_step = 70;					// valid range = 1 - 9999 (0.1 - 999.9us)

volatile uint16_t threePos_firstPosition = 9000;	// valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t threePos_secondPosition = 15000;	// valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t threePos_thirdPosition = 21000;	// valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t threePos_rate = 10;				// valid range = 3 - 30 (3 - 30ms)
volatile uint16_t threePos_wait = 10;				// valid range = 1 - 999 (0.1 - 99.9s)
volatile uint32_t threePos_cycles = 0;				// valid range = 0 - 9999999, 0 = no limit

volatile uint16_t twoPos_firstPosition = 9000;		// valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t twoPos_secondPosition = 21000;	// valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t twoPos_rate = 10;					// valud range = 3 - 30 (3 - 30ms)
volatile uint16_t twoPos_wait = 18;					// valid range = 1 - 999 (0.1 - 99.9s)
volatile uint32_t twoPos_cycles = 0;				// valid range = 0 - 9999999, 0 = no limit

volatile uint16_t onePos_position = 15000;			// valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t onePos_rate = 10;					// valud range = 3 - 30 (3 - 30ms)

int main() {
	// Setup the LCD
	lcd_tft1p4705_setup(PORT_A, PC13, PC10, PC11, PC12, PC14); // db0-15 gpio port, cs, rs, wr, rd, reset

	// Fill the LCD with the background color
	for(uint32_t loop = 0; loop < (320*480); loop++) {
		lcd_write_register(0x0022, 0x0000);
	}

	// Setup the four push buttons
	gpio_setup(PD2, OUTPUT, PUSH_PULL, FIFTY_MHZ, NO_PULL, AF0); // common wire
	gpio_low(PD2);
	gpio_setup(PB4, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0); // left button
	exti_setup(PB4, FALLING_EDGE);
	gpio_setup(PB5, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0); // up button
	exti_setup(PB5, FALLING_EDGE);
	gpio_setup(PB6, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0); // down button
	exti_setup(PB6, FALLING_EDGE);
	gpio_setup(PB7, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0); // right button
	exti_setup(PB7, FALLING_EDGE);

	// Setup the timer
	timer_pwm_setup(TIM2, 24, (sweep_rate * 2000), FOUR_CHANNELS, PB10, PB10, PB10, PB10); //ch3
//////////////////// FIXME: enable interrupt for timer //////////////////////	
	// Draw the current screen
	while(1) {
		if(currentScreen == SWEEP)
			drawSweepScreen();
		else if(currentScreen == THREE_POS)
			draw3posScreen();
		else if(currentScreen == TWO_POS)
			draw2posScreen();
		else if(currentScreen == ONE_POS)
			draw1posScreen();
	}
}
