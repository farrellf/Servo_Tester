// Written by Farrell Farahbod
// Last revised on 2014-07-03
// This file is released into the public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "externs.h"

void drawSweepScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *START: 900.0us   RATE:    10ms*
	 *STOP: 2100.0us   STEP:   5.0ms*
	 *               CYCLES:       0*
	 *>> COMPLETED       0 CYCLES <<*
	 ********************************/
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "START:%p41us   RATE:    %p20ms", sweep_start, sweep_rate);
	lcd_printf(0, 2, "STOP: %p41us   STEP: %p31us", sweep_stop, sweep_step);
	lcd_printf(0, 3, "               CYCLES: %p70", sweep_cycles);
	lcd_printf(0, 4, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
}

void draw3posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us   RATE:    10ms*
	 *POS2: 1500.0us   WAIT:    1.0s*
	 *POS3: 2100.0us CYCLES:       0*
	 *>> COMPLETED       0 CYCLES <<*
	 ********************************/
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "POS1: %p41us   RATE:    %p20ms", threePos_firstPosition, threePos_rate);
	lcd_printf(0, 2, "POS2: %p41us   WAIT:   %p21s", threePos_secondPosition, threePos_wait);
	lcd_printf(0, 3, "POS3: %p41us CYCLES: %p70", threePos_thirdPosition, threePos_cycles);
	lcd_printf(0, 4, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
}

void draw2posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us   RATE:    10ms*
	 *POS2: 2100.0us   WAIT:    1.0s*
	 *               CYCLES:       0*
	 *>> COMPLETED       0 CYCLES <<*
	 ********************************/
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "POS1: %p41us   RATE:    %p20ms", twoPos_firstPosition, twoPos_rate);
	lcd_printf(0, 2, "POS2: %p41us   WAIT:   %p21s", twoPos_secondPosition, twoPos_wait);
	lcd_printf(0, 3, "               CYCLES: %p70", twoPos_cycles);
	lcd_printf(0, 4, ">> COMPLETED %p70 CYCLES <<", cyclesCompleted);
}


void draw1posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS:   900.0us                *
	 *RATE:     10ms                *
	 *                              *
	 *                              *
	 ********************************/
	lcd_printf(0, 0, "MODE:  Sweep  3pos  2pos  1pos");
	lcd_printf(0, 1, "POS1: %p41us                ", onePos_position);
	lcd_printf(0, 2, "RATE:     %p20ms                ", onePos_rate);
	lcd_printf(0, 3, "                              ");
	lcd_printf(0, 4, "                              ");
}
