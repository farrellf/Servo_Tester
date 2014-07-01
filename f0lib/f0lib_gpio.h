// Authored by Farrell Farahbod, last revised on 2014-05-20
// This file is released into the public domain

#include "stm32f0xx.h"

#ifndef F0LIB_GPIO
#define F0LIB_GPIO
enum GPIO_PIN {	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
				PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
				PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
				PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
				PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
				PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15};

enum GPIO_MODE {INPUT, OUTPUT, AF, ANALOG};
enum GPIO_TYPE {PUSH_PULL, OPEN_DRAIN};
enum GPIO_SPEED {TWO_MHZ, TEN_MHZ, FIFTY_MHZ};
enum GPIO_PULL {NO_PULL, PULL_UP, PULL_DOWN};
enum GPIO_AF {AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7};
#endif

/**
 * Prepares a pin for use. This enables the appropriate GPIO clock and sets all the pin attributes.
 *
 * Example: gpio_setup(PA13, OUTPUT, PUSH_PULL, FIFTY_MHZ, NO_PULL, AF0);
 *
 * @param	pin		pin being configured
 * @param	mode	input, output, alternate function or analog
 * @param	type	push-pull or open-drain
 * @param	speed	50MHz, 10MHz or 2MHz
 * @param	pull	pull-up, pull-down or neither
 * @param	af		alternate function (must be specified, but only has an effect when mode is set to alternate function)
 */
void gpio_setup(enum GPIO_PIN	pin,
				enum GPIO_MODE	mode,
				enum GPIO_TYPE	type,
				enum GPIO_SPEED	speed,
				enum GPIO_PULL	pull,
				enum GPIO_AF	af);

/**
 * Change pin state to low. Pin mode must be set to output.
 *
 * Example: gpio_low(PA7);
 *
 * @param	pin		pin to make low
 */
void gpio_low(enum GPIO_PIN pin);

 /**
 * Change pin state to high. Pin mode must be set to output.
 *
 * Example: gpio_high(PA7);
 *
 * @param	pin		pin to make high
 */
void gpio_high(enum GPIO_PIN pin);
