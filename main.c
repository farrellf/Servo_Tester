// Written by Farrell Farahbod
// Last revised on 2014-07-09
// This file is released into the public domain

#include "stm32f0xx.h"
#include "f0lib/f0lib.h"
#include "config.h"

volatile enum Screen currentScreen = SWEEP;
volatile uint32_t cyclesCompleted = 0;
volatile uint16_t currentServoPulse = 9000;
volatile uint32_t voltage = 123;
volatile uint32_t ch1current = 1234;
volatile uint32_t ch2current = 2345;
volatile uint32_t ch3current = 3456;
volatile uint32_t ch4current = 4567;

int main() {
	// Setup the LCD
	lcd_tft1p4705_setup(PORT_A, PC13, PC10, PC11, PC12, PC14); // db0-15 gpio port, cs, rs, wr, rd, reset

	// Fill the LCD with the background color
	for(uint32_t loop = 0; loop < (320*480); loop++) {
		lcd_write_register(0x0022, 0x0000);
	}

	// Setup the four push buttons
	gpio_setup(PD2, OUTPUT, PUSH_PULL, FIFTY_MHZ, NO_PULL, AF0); // common
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
	timer_pwm_setup(TIM2, 24, currentServoPulse/5, FOUR_CHANNELS, PB10, PB10, PB10, PB10); // only using ch3 right now

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
