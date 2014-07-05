/*********************************************************************************
 * EXTERNS FOR ALL OF THE GLOBAL VARIABLES AND LCD DATA                          *
 *********************************************************************************/

#include "stm32f0xx.h"

enum Screen {SWEEP, THREE_POS, TWO_POS, ONE_POS};
enum SelectedOption	{MODE,
					SWEEP_START, SWEEP_STOP, SWEEP_CYCLES, SWEEP_RATE, SWEEP_STEP,
					THREEPOS_1POS, THREEPOS_2POS, THREEPOS_3POS, THREEPOS_RATE, THREEPOS_WAIT, THREEPOS_CYCLES,
					TWOPOS_1POS, TWOPOS_2POS, TWOPOS_RATE, TWOPOS_WAIT, TWOPOS_CYCLES,
					ONEPOS_POSITION, ONEPOS_RATE};
					
// Current State
extern volatile enum Screen currentScreen;
extern volatile enum SelectedOption currentlySelectedOption;				
extern volatile uint32_t cyclesCompleted;
extern volatile uint16_t currentServoPulse;
extern volatile uint8_t drawUnderline;

// Options for the Sweep mode
extern volatile uint16_t sweep_start;
extern volatile uint16_t sweep_stop;
extern volatile uint32_t sweep_cycles;
extern volatile uint16_t sweep_rate;
extern volatile uint16_t sweep_step;

// Options for the 3Pos (three position) mode
extern volatile uint16_t threePos_firstPosition;
extern volatile uint16_t threePos_secondPosition;
extern volatile uint16_t threePos_thirdPosition;
extern volatile uint16_t threePos_rate;
extern volatile uint16_t threePos_wait;
extern volatile uint32_t threePos_cycles;

// Options for the 2Pos (two position) mode
extern volatile uint16_t twoPos_firstPosition;
extern volatile uint16_t twoPos_secondPosition;
extern volatile uint16_t twoPos_rate;
extern volatile uint16_t twoPos_wait;
extern volatile uint32_t twoPos_cycles;

// Options for the 1Pos (one position) mode
extern volatile uint16_t onePos_position;
extern volatile uint16_t onePos_rate;
