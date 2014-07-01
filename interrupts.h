/*********************************************************************************
 * ISRs FOR THE SIX TIMERS USED                                                  *
 *********************************************************************************/

void TIM15_IRQHandler();				// Left arrow (decrease) ISR
void TIM2_IRQHandler();					// Right arrow (increase) ISR
void TIM14_IRQHandler(); 				// Up arrow ISR
void TIM16_IRQHandler();				// Down arrow ISR
void TIM6_DAC_IRQHandler();				// Blinking underline ISR
void TIM1_BRK_UP_TRG_COM_IRQHandler();	// PWM output ISR
