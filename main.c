// Written by Farrell Farahbod
// Last revised on 2014-07-01
// This file is released into the public domain

#include "stm32f0xx.h"
#include "f0lib/f0lib.h"
//#include "core_cm0.h"
//#include "uc1608fb.h"
//#include "interrupts.h"
//#include "screens.h"

/*********************************************************************************
 * PROGRAM STATE                                                                 *
 *********************************************************************************/
enum Mode {SWEEP, THREE_POS, TWO_POS, ONE_POS};
enum SelectedOption	{MODE,
					SWEEP_START, SWEEP_STOP, SWEEP_CYCLES, SWEEP_RATE, SWEEP_STEP,
					THREEPOS_1POS, THREEPOS_2POS, THREEPOS_3POS, THREEPOS_RATE, THREEPOS_WAIT, THREEPOS_CYCLES,
					TWOPOS_1POS, TWOPOS_2POS, TWOPOS_RATE, TWOPOS_WAIT, TWOPOS_CYCLES,
					ONEPOS_POSITION, ONEPOS_RATE};

// Default state for each mode
volatile uint16_t sweep_start = 9000;				// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t sweep_stop = 21000;				// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint32_t sweep_cycles = 0;					// 0 = no limit.					valid range = 0 - 9999999
volatile uint16_t sweep_rate = 10;					// framerate in milliseconds.		valud range = 3-30
volatile uint16_t sweep_step = 70;					// in tenths of a microsecond.		valid range = 1 - 9999 (0.1 - 999.9us)

volatile uint16_t threePos_firstPosition = 9000;	// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t threePos_secondPosition = 15000;	// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t threePos_thirdPosition = 21000;	// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t threePos_rate = 10;				// framerate in milliseconds.		valud range = 3-30
volatile uint16_t threePos_wait = 10;				// in tenths of a second.			valid range = 1 - 999
volatile uint32_t threePos_cycles = 0;				// 0 = no limit.					valid range = 0 - 9999999

volatile uint16_t twoPos_firstPosition = 9000;		// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t twoPos_secondPosition = 21000;	// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t twoPos_rate = 10;					// framerate in milliseconds.		valud range = 3-30
volatile uint16_t twoPos_wait = 18;					// in tenths of a second.			valid range = 1 - 999
volatile uint32_t twoPos_cycles = 0;				// 0 = no limit.					valid range = 0 - 9999999

volatile uint16_t onePos_position = 15000;			// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint16_t onePos_rate = 10;					// framerate in milliseconds.		valud range = 3-30

// Defaults
volatile enum Mode currentMode = SWEEP;
volatile enum SelectedOption currentlySelectedOption = MODE;
volatile uint32_t cyclesCompleted = 0;
volatile uint16_t currentServoPulse = 9000;			// in tenths of a microsecond.		valid range = 8000 - 22000 (800 - 2200us)
volatile uint8_t drawUnderline = 0;					// 0 or 1 to draw or not draw an underline below the currently selected item

/*********************************************************************************
 * MAIN                                                                          *
 *********************************************************************************/
int main() {
	// setup the LCD
	lcd_tft1p4705_setup(PORT_A, PC13, PC10, PC11, PC12, PC14); // DB0-15 gpio port, cs, rs, wr, rd, reset

	// fill the LCD with a black background
	for(uint32_t loop = 0; loop < (320*480); loop++) {
		lcd_write_register(0x0022, 0x0000);
	}

	// draw some text
	lcd_printf(0, 0, "Hello world!");								// Hello world!
	lcd_printf(0, 1, "3 as a signed 16bit int is %d", 3);			// ... +00003
	lcd_printf(0, 2, "69 as an unsigned 32bit int is %u", 69);		// ... 0000000069
	lcd_printf(0, 3, "1234 as binary is %b", 1234);					// ... 0000 0000 0000 0000 0000 0100 1101 0010
	lcd_printf(0, 4, "\"myString\" as a string is %s", "myString"); // ... myString
	lcd_printf(0, 5, "98765 divided by 10 is %p51", 98765);			// ... 09876.5

	// old code base
	/*
	lcd_init(B4, B5, B6, B7, B8, B9, B10, B11, B12, B13);	// CD, WR1, D7, D6, D5, D4, D3, D2, D1, D0
	
	// To optimize this project for the slower F0 MCU, the lcd_write_register() function has the pins
	// hard-coded. If you change the pins, you must modify lcd_write_register() accordingly.
	
	//lcd_write_pixbuf();									// splash screen
	//delay(1500000);										// 1.5 seconds
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;						// Enable GPIOA clock
	GPIOA->MODER |= GPIO_MODER_MODER2_1;					// Pin A2 = Alt Function		LEFT ARROW PUSHBUTTON
	GPIOA->AFR[0] |= 0b0000 << 8;							// AF0 for pin A2 (TIM15)		LEFT ARROW PUSHBUTTON
	GPIOA->MODER |= GPIO_MODER_MODER15_1;					// Pin A15 = Alt Function		RIGHT ARROW PUSHBUTTON
	GPIOA->AFR[1] |= 0b0010 << 28;							// AF2 for pin A15 (TIM2)		RIGHT ARROW PUSHBUTTON
	GPIOA->MODER |= GPIO_MODER_MODER4_1;					// Pin A4 = Alt Function		UP ARROW PUSHBUTTON
	GPIOA->AFR[0] |= 0b0100 << 16;							// AF4 for pin A4 (TIM14)		UP ARROW PUSHBUTTON
	GPIOA->MODER |= GPIO_MODER_MODER6_1;					// Pin A6 = Alt Function		DOWN ARROW PUSHBUTTON
	GPIOA->AFR[0] |= 0b0101 << 24;							// AF5 for pin A6 (TIM16)		DOWN ARROW PUSHBUTTON
	GPIOA->MODER |= GPIO_MODER_MODER8_1;					// Pin A8 = Alt Function		PWM
	GPIOA->AFR[1] |= 0b0010 << 0;							// AF2 for pin A8 (TIM1)		PWM
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8;				// 50MHz for pin A8				PWM

	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;					// Enable clock					LEFT ARROW PUSHBUTTON
	TIM15->PSC = 20999;										// Prescaler = 20999
	TIM15->ARR = 999;										// Auto-reload = 999
	TIM15->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;			// TS=101 to trigger on TI1
	TIM15->SMCR |= TIM_SMCR_SMS_2;							// SMS=100 for reset on TI1
	TIM15->EGR |= TIM_EGR_UG;								// Force update
	TIM15->SR &= ~TIM_SR_UIF;								// Clear the update flag
	TIM15->DIER |= TIM_DIER_UIE;							// Enable interrupt on update event
	NVIC_EnableIRQ(TIM15_IRQn);								// Enable IRQ
	TIM15->CR1 |= TIM_CR1_CEN;								// Enable counter
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;						// Enable clock					RIGHT ARROW PUSHBUTTON
	TIM2->PSC = 20499;										// Prescaler = 20499
	TIM2->ARR = 999;										// Auto-reload = 999
	TIM2->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;			// TS=101 to trigger on TI1
	TIM2->SMCR |= TIM_SMCR_SMS_2;							// SMS=100 for reset on TI1
	TIM2->EGR |= TIM_EGR_UG;								// Force update
	TIM2->SR &= ~TIM_SR_UIF;								// Clear the update flag
	TIM2->DIER |= TIM_DIER_UIE;								// Enable interrupt on update event
	NVIC_EnableIRQ(TIM2_IRQn);								// Enable IRQ
	TIM2->CR1 |= TIM_CR1_CEN;								// Enable counter
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;					// Enable clock					UP ARROW PUSHBUTTON
	TIM14->PSC = 1499;										// Prescaler = 1499
	TIM14->ARR = 999;										// Auto-reload = 999
	TIM14->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;			// TS=101 to trigger on TI1
	TIM14->SMCR |= TIM_SMCR_SMS_2;							// SMS=100 for reset on TI1
	TIM14->EGR |= TIM_EGR_UG;								// Force update
	TIM14->SR &= ~TIM_SR_UIF;								// Clear the update flag
	TIM14->DIER |= TIM_DIER_UIE;							// Enable interrupt on update event
	NVIC_EnableIRQ(TIM14_IRQn);								// Enable IRQ
	TIM14->CR1 |= TIM_CR1_CEN;								// Enable counter
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;					// Enable clock					DOWN ARROW PUSHBUTTON
	TIM16->PSC = 1499;										// Prescaler = 1499
	TIM16->ARR = 999;										// Auto-reload = 999
	TIM16->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;			// TS=101 to trigger on TI1
	TIM16->SMCR |= TIM_SMCR_SMS_2;							// SMS=100 for reset on TI1
	TIM16->EGR |= TIM_EGR_UG;								// Force update
	TIM16->SR &= ~TIM_SR_UIF;								// Clear the update flag
	TIM16->DIER |= TIM_DIER_UIE;							// Enable interrupt on update event
	NVIC_EnableIRQ(TIM16_IRQn);								// Enable IRQ
	TIM16->CR1 |= TIM_CR1_CEN;								// Enable counter

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;						// Enable TIM6 clock			BLINKING UNDERLINE
	TIM6->PSC = 25999;										// Set prescaler to 25999
	TIM6->ARR = 799;										// Set auto-reload to 799
	TIM6->EGR |= TIM_EGR_UG;								// Force update
	TIM6->SR &= ~TIM_SR_UIF;								// Clear the update flag
	TIM6->DIER |= TIM_DIER_UIE;								// Enable interrupt on update event
	NVIC_EnableIRQ(TIM6_DAC_IRQn);							// Enable TIM6 IRQ
	TIM6->CR1 |= TIM_CR1_CEN;								// Enable TIM6 counter
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;						// Enable clock					PWM
	TIM1->PSC = 23;											// Prescaler = 23 (1 tick per half microsecond)
	TIM1->ARR = (sweep_rate * 2000) - 1;					// Auto-reload = PWM frequency in half microseconds
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;		// OC1M = 110 for PWM Mode 1 output on ch1
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;							// Output 1 preload enable
	TIM1->CR1 |= TIM_CR1_ARPE;								// Auto-reload preload enable
	TIM1->CCER |= TIM_CCER_CC1E;							// Enable output for ch1
	TIM1->CCR1 = currentServoPulse / 5;						// CCR1 = Duty cycle in half microseconds
	TIM1->EGR |= TIM_EGR_UG;								// Force update
	TIM1->SR &= ~TIM_SR_UIF;								// Clear the update flag
	TIM1->DIER |= TIM_DIER_UIE;								// Enable interrupt on update event
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);				// Enable IRQ
	TIM1->BDTR |= TIM_BDTR_MOE;								// Main output enable -- needed for advanced control timers only.
	TIM1->CR1 |= TIM_CR1_CEN;								// Enable counter
	
	// Draw screen based on current state
	while(1) {
		if(currentMode == SWEEP)
			drawSweepScreen();
		else if(currentMode == THREE_POS)
			draw3posScreen();
		else if(currentMode == TWO_POS)
			draw2posScreen();
		else if(currentMode == ONE_POS)
			draw1posScreen();
	}
	*/
}
