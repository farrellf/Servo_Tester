/*********************************************************************************
 * A VERY SMALL LIBRARY TO MAKE GPIO OPERATIONS A LITTLE EASIER.                 *
 *********************************************************************************/

// IF THE delay() FUCNTION WILL BE USED, YOU MUST CALL SysTick_Config(SystemCoreClock / 1000000);
// BEFORE THE FIRST USE OF delay() OR ELSE AN INFINITE LOOP WILL OCCUR.

#include "stm32f0xx.h"

enum GPIO_PIN {	A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
				B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
				C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15,
				D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15,	/* D2 is the only valid pin in Port D */
				E0, E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12, E13, E14, E15,	/* No valid pins in Port E */
				F0, F1, F2, F3, F4, F5, F6, F7};										/* F2 and F3 are not valid pins in Port F */

// SysTick is used to keep track of how many microseconds have elapsed since the last reset.
volatile uint32_t micros;
void SysTick_Handler();

// Enable the GPIO port used by a pin, and use the pin as a general-purpose output
void EnablePin(enum GPIO_PIN pin);

// Make the pin HIGH
void SetPin(enum GPIO_PIN pin);

// Make the pin LOW
void ClearPin(enum GPIO_PIN pin);

// Waste time for x microseconds
void delay(unsigned long duration);
