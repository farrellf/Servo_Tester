/*********************************************************************************
 * A VERY SMALL LIBRARY TO MAKE GPIO OPERATIONS A LITTLE EASIER.                 *
 *********************************************************************************/

// IF THE delay() FUCNTION WILL BE USED, YOU MUST CALL SysTick_Config(SystemCoreClock / 1000000);
// BEFORE THE FIRST USE OF delay() OR ELSE AN INFINITE LOOP WILL OCCUR.

#include "simple_gpio.h"

volatile uint32_t micros = 0;

void SysTick_Handler() {
	micros++;
}

void EnablePin(enum GPIO_PIN pin) {
	// Enable GPIOx port clock, and set pin type as output
	
	char port = pin / 16;
	if(port == 1) { // port B
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
		GPIOB->MODER |= (1 << (2*(pin % 16)));
	} else if(port == 0) { // port A
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		GPIOA->MODER |= (1 << (2*(pin % 16)));
	} else if(port == 2) { // port C
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		GPIOC->MODER |= (1 << (2*(pin % 16)));
	} else if(port == 3) { // port D
		RCC->AHBENR |= RCC_AHBENR_GPIODEN;
		GPIOD->MODER |= (1 << (2*(pin % 16)));
	} else if(port == 5) { // port F
		RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
		GPIOF->MODER |= (1 << (2*(pin % 16)));
	}
}

void SetPin(enum GPIO_PIN pin) {
	// Change pin state to high

	char port = pin / 16;
	if(port == 0) { // port A
		GPIOA->BSRR = (1 << (pin % 16));
	} else if(port == 1) { // port B
		GPIOB->BSRR = (1 << (pin % 16));
	} else if(port == 2) { // port C
		GPIOC->BSRR = (1 << (pin % 16));
	} else if(port == 3) { // port D
		GPIOD->BSRR = (1 << (pin % 16));
	} else if(port == 5) { // port F
		GPIOF->BSRR = (1 << (pin % 16));
	}
}

void ClearPin(enum GPIO_PIN pin) {
	// Change pin state to low

	char port = pin / 16;
	if(port == 0) { // port A
		GPIOA->BSRR = (1 << (pin % 16) + 16);
	} else if(port == 1) { // port B
		GPIOB->BSRR= (1 << (pin % 16) + 16);
	} else if(port == 2) { // port C
		GPIOC->BSRR = (1 << (pin % 16) + 16);
	} else if(port == 3) { // port D
		GPIOD->BSRR = (1 << (pin % 16) + 16);
	} else if(port == 5) { // port F
		GPIOF->BSRR = (1 << (pin % 16) + 16);
	}
}

void delay(unsigned long duration) {
	// Wait for the specified number of microseconds
	
	unsigned long startTime = micros;
	while(startTime + duration > micros);
}
