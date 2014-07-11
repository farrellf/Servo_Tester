// Written by Farrell Farahbod
// Last revised on 2014-07-09
// This file is released into the public domain

#include "stm32f0xx.h"

#define SERVO_PULSE_MIN 8000      // 800.0us
#define SERVO_PULSE_MAX 22000     // 2200.0us
#define SERVO_PERIOD_MIN 3        // 3ms (f=333Hz)
#define SERVO_PERIOD_MAX 30       // 30ms (f=33Hz)
#define SERVO_STEP_MIN 1          // 0.1us
#define SERVO_STEP_MAX 9999       // 999.9us
#define SERVO_WAIT_MIN 1          // 0.1s
#define SERVO_WAIT_MAX 999        // 99.9s
#define SERVO_CYCLES_MAX 9999999

enum Event {LEFT_BUTTON_PRESSED,
            RIGHT_BUTTON_PRESSED,
            UP_BUTTON_PRESSED,
            DOWN_BUTTON_PRESSED,
            CH1_CURRENT_READING,
            CH2_CURRENT_READING,
            CH3_CURRENT_READING,
            CH4_CURRENT_READING,
            VOLTAGE_READING};

void drawSweepScreen(void);
void notifySweepScreen(enum Event event);

void draw3posScreen(void);
void notify3posScreen(enum Event event);

void draw2posScreen(void);
void notify2posScreen(enum Event event);

void draw1posScreen(void);
void notify1posScreen(enum Event event);

	
// General State
enum Screen {SWEEP, THREE_POS, TWO_POS, ONE_POS};
extern volatile enum Screen currentScreen;			
extern volatile uint32_t cyclesCompleted;
extern volatile uint16_t currentServoPulse;

// Options for the Sweep mode
extern volatile uint16_t sweep_pos1;
extern volatile uint16_t sweep_pos2;
extern volatile uint32_t sweep_cycles;
extern volatile uint16_t sweep_period;
extern volatile uint16_t sweep_step;

// Options for the 3Pos (three position) mode
extern volatile uint16_t threePos_pos1;
extern volatile uint16_t threePos_pos2;
extern volatile uint16_t threePos_pos3;
extern volatile uint16_t threePos_period;
extern volatile uint16_t threePos_wait;
extern volatile uint32_t threePos_cycles;

// Options for the 2Pos (two position) mode
extern volatile uint16_t twoPos_pos1;
extern volatile uint16_t twoPos_pos2;
extern volatile uint16_t twoPos_period;
extern volatile uint16_t twoPos_wait;
extern volatile uint32_t twoPos_cycles;

// Options for the 1Pos (one position) mode
extern volatile uint16_t onePos_position;
extern volatile uint16_t onePos_period;

// Voltage and current sensing
extern volatile uint32_t voltage;
extern volatile uint32_t ch1current;
extern volatile uint32_t ch2current;
extern volatile uint32_t ch3current;
extern volatile uint32_t ch4current;
