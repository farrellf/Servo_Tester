/*********************************************************************************
 * DRAW THE SCREENS BASED ON THE CURRENT MODE AND VALUES STORED IN THE GLOBALS   *
 *********************************************************************************
 * Using sprintf() would make this trivial and result in more maintainable code, *
 * but including that library requires more space than some of the STM32 MCUs    *
 * support.                                                                      * 
 *********************************************************************************/

#include "screens.h"
#include "externs.h"
//#include "uc1608fb.h"

void drawSweepScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *START: 900.0us   RATE:    10ms*
	 *STOP: 2100.0us   STEP:   5.0ms*
	 *               CYCLES:       0*
	 *>> COMPLETED       0 CYCLES <<*
	 ********************************/
	/*
	lcd_write_line16(0, "MODE:  Sweep  3pos  2pos  1pos");
	
	lcd_write_line16(1, "START:");
	sweep_start >= 10000 ? lcd_write_char16(1, 6, sweep_start / 10000 + 48) : lcd_write_char16(1, 6, ' ');
	lcd_write_char16(1, 7, sweep_start / 1000 % 10 + 48);
	lcd_write_char16(1, 8, sweep_start / 100 % 10 + 48);
	lcd_write_char16(1, 9, sweep_start / 10 % 10 + 48);
	lcd_write_char16(1, 10, '.');
	lcd_write_char16(1, 11, sweep_start % 10 + 48);
	lcd_write_char16(1, 12, 'u');
	lcd_write_char16(1, 13, 's');
	lcd_write_char16(1, 14, ' ');
	lcd_write_char16(1, 15, ' ');
	lcd_write_char16(1, 16, ' ');
	lcd_write_char16(1, 17, 'R');
	lcd_write_char16(1, 18, 'A');
	lcd_write_char16(1, 19, 'T');
	lcd_write_char16(1, 20, 'E');
	lcd_write_char16(1, 21, ':');
	lcd_write_char16(1, 22, ' ');
	lcd_write_char16(1, 23, ' ');
	lcd_write_char16(1, 24, ' ');
	lcd_write_char16(1, 25, ' ');
	sweep_rate >= 10 ? lcd_write_char16(1, 26, sweep_rate / 10 + 48) : lcd_write_char16(1, 26, ' ');
	lcd_write_char16(1, 27, sweep_rate % 10 + 48);
	lcd_write_char16(1, 28, 'm');
	lcd_write_char16(1, 29, 's');
	
	lcd_write_line16(2, "STOP: ");
	sweep_stop >= 10000 ? lcd_write_char16(2, 6, sweep_stop / 10000 + 48) : lcd_write_char16(2, 6, ' ');
	lcd_write_char16(2, 7, sweep_stop / 1000 % 10 + 48);
	lcd_write_char16(2, 8, sweep_stop / 100 % 10 + 48);
	lcd_write_char16(2, 9, sweep_stop / 10 % 10 + 48);
	lcd_write_char16(2, 10, '.');
	lcd_write_char16(2, 11, sweep_stop % 10 + 48);
	lcd_write_char16(2, 12, 'u');
	lcd_write_char16(2, 13, 's');
	lcd_write_char16(2, 14, ' ');
	lcd_write_char16(2, 15, ' ');
	lcd_write_char16(2, 16, ' ');
	lcd_write_char16(2, 17, 'S');
	lcd_write_char16(2, 18, 'T');
	lcd_write_char16(2, 19, 'E');
	lcd_write_char16(2, 20, 'P');
	lcd_write_char16(2, 21, ':');
	lcd_write_char16(2, 22, ' ');
	sweep_step >= 1000 ? lcd_write_char16(2, 23, sweep_step / 1000 + 48) : lcd_write_char16(2, 23, ' ');
	sweep_step >= 100 ? lcd_write_char16(2, 24, sweep_step / 100 % 10 + 48) : lcd_write_char16(2, 24, ' ');
	sweep_step >= 10 ? lcd_write_char16(2, 25, sweep_step / 10 % 10 + 48) : lcd_write_char16(2, 25, ' ');
	lcd_write_char16(2, 26, '.');
	lcd_write_char16(2, 27, sweep_step % 10 + 48);
	lcd_write_char16(2, 28, 'u');
	lcd_write_char16(2, 29, 's');
	
	lcd_write_line16(3, "               CYCLES: ");
	sweep_cycles >= 1000000 ? lcd_write_char16(3, 23, sweep_cycles / 1000000 + 48) : lcd_write_char16(3, 23, ' ');
	sweep_cycles >= 100000 ? lcd_write_char16(3, 24, sweep_cycles / 100000 % 10 + 48) : lcd_write_char16(3, 24, ' ');
	sweep_cycles >= 10000 ? lcd_write_char16(3, 25, sweep_cycles / 10000 % 10 + 48) : lcd_write_char16(3, 25, ' ');
	sweep_cycles >= 1000 ? lcd_write_char16(3, 26, sweep_cycles / 1000 % 10 + 48) : lcd_write_char16(3, 26, ' ');
	sweep_cycles >= 100 ? lcd_write_char16(3, 27, sweep_cycles / 100 % 10 + 48) : lcd_write_char16(3, 27, ' ');
	sweep_cycles >= 10 ? lcd_write_char16(3, 28, sweep_cycles / 10 % 10 + 48) : lcd_write_char16(3, 28, ' ');
	lcd_write_char16(3, 29, sweep_cycles % 10 + 48);
	
	lcd_write_line16(4, ">> COMPLETED ");
	cyclesCompleted >= 1000000 ? lcd_write_char16(4, 13, cyclesCompleted / 1000000 + 48) : lcd_write_char16(4, 13, ' ');
	cyclesCompleted >= 100000 ? lcd_write_char16(4, 14, cyclesCompleted / 100000 % 10 + 48) : lcd_write_char16(4, 14, ' ');
	cyclesCompleted >= 10000 ? lcd_write_char16(4, 15, cyclesCompleted / 10000 % 10 + 48) : lcd_write_char16(4, 15, ' ');
	cyclesCompleted >= 1000 ? lcd_write_char16(4, 16, cyclesCompleted / 1000 % 10 + 48) : lcd_write_char16(4, 16, ' ');
	cyclesCompleted >= 100 ? lcd_write_char16(4, 17, cyclesCompleted / 100 % 10 + 48) : lcd_write_char16(4, 17, ' ');
	cyclesCompleted >= 10 ? lcd_write_char16(4, 18, cyclesCompleted / 10 % 10 + 48) : lcd_write_char16(4, 18, ' ');
	lcd_write_char16(4, 19, cyclesCompleted % 10 + 48);
	lcd_write_char16(4, 20, ' ');
	lcd_write_char16(4, 21, 'C');
	lcd_write_char16(4, 22, 'Y');
	lcd_write_char16(4, 23, 'C');
	lcd_write_char16(4, 24, 'L');
	lcd_write_char16(4, 25, 'E');
	lcd_write_char16(4, 26, 'S');
	lcd_write_char16(4, 27, ' ');
	lcd_write_char16(4, 28, '<');
	lcd_write_char16(4, 29, '<');
	
	// Underline "Sweep"
	lcd_write_underline(7,11,0);
	
	// Underline current option
	if(drawUnderline == 1)
		lcd_draw_current_underline();
	
	// 15px block to represent the current servo pulse length
	uint8_t offset = (currentServoPulse - 8000) / 62;
	uint8_t i;
	for(i = 0; i < 240; i++) {
		if(i < offset || i > offset + 14) {
			pixBuf[i][10] = 0;
			pixBuf[i][11] = 0;
		} else {
			pixBuf[i][10] = 255;
			pixBuf[i][11] = 255;
		}
	}
	lcd_write_pixbuf();
	*/
}

void draw3posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us   RATE:    10ms*
	 *POS2: 1500.0us   WAIT:    1.0s*
	 *POS3: 2100.0us CYCLES:       0*
	 *>> COMPLETED       0 CYCLES <<*
	 ********************************/
	/*
	lcd_write_line16(0, "MODE:  Sweep  3pos  2pos  1pos");
	
	lcd_write_line16(1, "POS1: ");
	threePos_firstPosition >= 10000 ? lcd_write_char16(1, 6, threePos_firstPosition / 10000 + 48) : lcd_write_char16(1, 6, ' ');
	lcd_write_char16(1, 7, threePos_firstPosition / 1000 % 10 + 48);
	lcd_write_char16(1, 8, threePos_firstPosition / 100 % 10 + 48);
	lcd_write_char16(1, 9, threePos_firstPosition / 10 % 10 + 48);
	lcd_write_char16(1, 10, '.');
	lcd_write_char16(1, 11, threePos_firstPosition % 10 + 48);
	lcd_write_char16(1, 12, 'u');
	lcd_write_char16(1, 13, 's');
	lcd_write_char16(1, 14, ' ');
	lcd_write_char16(1, 15, ' ');
	lcd_write_char16(1, 16, ' ');
	lcd_write_char16(1, 17, 'R');
	lcd_write_char16(1, 18, 'A');
	lcd_write_char16(1, 19, 'T');
	lcd_write_char16(1, 20, 'E');
	lcd_write_char16(1, 21, ':');
	lcd_write_char16(1, 22, ' ');
	lcd_write_char16(1, 23, ' ');
	lcd_write_char16(1, 24, ' ');
	lcd_write_char16(1, 25, ' ');
	threePos_rate >= 10 ? lcd_write_char16(1, 26, threePos_rate / 10 + 48) : lcd_write_char16(1, 26, ' ');
	lcd_write_char16(1, 27, threePos_rate % 10 + 48);
	lcd_write_char16(1, 28, 'm');
	lcd_write_char16(1, 29, 's');
	
	lcd_write_line16(2, "POS2: ");
	threePos_secondPosition >= 10000 ? lcd_write_char16(2, 6, threePos_secondPosition / 10000 + 48) : lcd_write_char16(2, 6, ' ');
	lcd_write_char16(2, 7, threePos_secondPosition / 1000 % 10 + 48);
	lcd_write_char16(2, 8, threePos_secondPosition / 100 % 10 + 48);
	lcd_write_char16(2, 9, threePos_secondPosition / 10 % 10 + 48);
	lcd_write_char16(2, 10, '.');
	lcd_write_char16(2, 11, threePos_secondPosition % 10 + 48);
	lcd_write_char16(2, 12, 'u');
	lcd_write_char16(2, 13, 's');
	lcd_write_char16(2, 14, ' ');
	lcd_write_char16(2, 15, ' ');
	lcd_write_char16(2, 16, ' ');
	lcd_write_char16(2, 17, 'W');
	lcd_write_char16(2, 18, 'A');
	lcd_write_char16(2, 19, 'I');
	lcd_write_char16(2, 20, 'T');
	lcd_write_char16(2, 21, ':');
	lcd_write_char16(2, 22, ' ');
	lcd_write_char16(2, 23, ' ');
	threePos_wait >= 1000 ? lcd_write_char16(2, 24, threePos_wait / 1000 + 48) : lcd_write_char16(2, 24, ' ');
	threePos_wait >= 100 ? lcd_write_char16(2, 25, threePos_wait / 100 % 10 + 48) : lcd_write_char16(2, 25, ' ');
	threePos_wait >= 10 ? lcd_write_char16(2, 26, threePos_wait / 10 % 10 + 48) : lcd_write_char16(2, 26, ' ');
	lcd_write_char16(2, 27, '.');
	lcd_write_char16(2, 28, threePos_wait % 10 + 48);
	lcd_write_char16(2, 29, 's');
	
	lcd_write_line16(3, "POS3: ");
	threePos_thirdPosition >= 10000 ? lcd_write_char16(3, 6, threePos_thirdPosition / 10000 + 48) : lcd_write_char16(3, 6, ' ');
	lcd_write_char16(3, 7, threePos_thirdPosition / 1000 % 10 + 48);
	lcd_write_char16(3, 8, threePos_thirdPosition / 100 % 10 + 48);
	lcd_write_char16(3, 9, threePos_thirdPosition / 10 % 10 + 48);
	lcd_write_char16(3, 10, '.');
	lcd_write_char16(3, 11, threePos_thirdPosition % 10 + 48);
	lcd_write_char16(3, 12, 'u');
	lcd_write_char16(3, 13, 's');
	lcd_write_char16(3, 14, ' ');
	lcd_write_char16(3, 15, 'C');
	lcd_write_char16(3, 16, 'Y');
	lcd_write_char16(3, 17, 'C');
	lcd_write_char16(3, 18, 'L');
	lcd_write_char16(3, 19, 'E');
	lcd_write_char16(3, 20, 'S');
	lcd_write_char16(3, 21, ':');
	lcd_write_char16(3, 22, ' ');
	threePos_cycles >= 1000000 ? lcd_write_char16(3, 23, threePos_cycles / 1000000 + 48) : lcd_write_char16(3, 23, ' ');
	threePos_cycles >= 100000 ? lcd_write_char16(3, 24, threePos_cycles / 100000 % 10 + 48) : lcd_write_char16(3, 24, ' ');
	threePos_cycles >= 10000 ? lcd_write_char16(3, 25, threePos_cycles / 10000 % 10 + 48) : lcd_write_char16(3, 25, ' ');
	threePos_cycles >= 1000 ? lcd_write_char16(3, 26, threePos_cycles / 1000 % 10 + 48) : lcd_write_char16(3, 26, ' ');
	threePos_cycles >= 100 ? lcd_write_char16(3, 27, threePos_cycles / 100 % 10 + 48) : lcd_write_char16(3, 27, ' ');
	threePos_cycles >= 10 ? lcd_write_char16(3, 28, threePos_cycles / 10 % 10 + 48) : lcd_write_char16(3, 28, ' ');
	lcd_write_char16(3, 29, threePos_cycles % 10 + 48);
	
	lcd_write_line16(4, ">> COMPLETED ");
	cyclesCompleted >= 1000000 ? lcd_write_char16(4, 13, cyclesCompleted / 1000000 + 48) : lcd_write_char16(4, 13, ' ');
	cyclesCompleted >= 100000 ? lcd_write_char16(4, 14, cyclesCompleted / 100000 % 10 + 48) : lcd_write_char16(4, 14, ' ');
	cyclesCompleted >= 10000 ? lcd_write_char16(4, 15, cyclesCompleted / 10000 % 10 + 48) : lcd_write_char16(4, 15, ' ');
	cyclesCompleted >= 1000 ? lcd_write_char16(4, 16, cyclesCompleted / 1000 % 10 + 48) : lcd_write_char16(4, 16, ' ');
	cyclesCompleted >= 100 ? lcd_write_char16(4, 17, cyclesCompleted / 100 % 10 + 48) : lcd_write_char16(4, 17, ' ');
	cyclesCompleted >= 10 ? lcd_write_char16(4, 18, cyclesCompleted / 10 % 10 + 48) : lcd_write_char16(4, 18, ' ');
	lcd_write_char16(4, 19, cyclesCompleted % 10 + 48);
	lcd_write_char16(4, 20, ' ');
	lcd_write_char16(4, 21, 'C');
	lcd_write_char16(4, 22, 'Y');
	lcd_write_char16(4, 23, 'C');
	lcd_write_char16(4, 24, 'L');
	lcd_write_char16(4, 25, 'E');
	lcd_write_char16(4, 26, 'S');
	lcd_write_char16(4, 27, ' ');
	lcd_write_char16(4, 28, '<');
	lcd_write_char16(4, 29, '<');
	
	// Underline "3pos"
	lcd_write_underline(14,17,0);
	
	// Underline current option
	if(drawUnderline == 1)
		lcd_draw_current_underline();
	
	// 15px block to represent the current servo pulse length
	uint8_t offset = (currentServoPulse - 8000) / 62;
	uint8_t i;
	for(i = 0; i < 240; i++) {
		if(i < offset || i > offset + 14) {
			pixBuf[i][10] = 0;
			pixBuf[i][11] = 0;
		} else {
			pixBuf[i][10] = 255;
			pixBuf[i][11] = 255;
		}
	}
	
	lcd_write_pixbuf();
	*/
}

void draw2posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS1:  900.0us   RATE:    10ms*
	 *POS2: 2100.0us   WAIT:    1.0s*
	 *               CYCLES:       0*
	 *>> COMPLETED       0 CYCLES <<*
	 ********************************/
	/*
	lcd_write_line16(0, "MODE:  Sweep  3pos  2pos  1pos");
	
	lcd_write_line16(1, "POS1: ");
	twoPos_firstPosition >= 10000 ? lcd_write_char16(1, 6, twoPos_firstPosition / 10000 + 48) : lcd_write_char16(1, 6, ' ');
	lcd_write_char16(1, 7, twoPos_firstPosition / 1000 % 10 + 48);
	lcd_write_char16(1, 8, twoPos_firstPosition / 100 % 10 + 48);
	lcd_write_char16(1, 9, twoPos_firstPosition / 10 % 10 + 48);
	lcd_write_char16(1, 10, '.');
	lcd_write_char16(1, 11, twoPos_firstPosition % 10 + 48);
	lcd_write_char16(1, 12, 'u');
	lcd_write_char16(1, 13, 's');
	lcd_write_char16(1, 14, ' ');
	lcd_write_char16(1, 15, ' ');
	lcd_write_char16(1, 16, ' ');
	lcd_write_char16(1, 17, 'R');
	lcd_write_char16(1, 18, 'A');
	lcd_write_char16(1, 19, 'T');
	lcd_write_char16(1, 20, 'E');
	lcd_write_char16(1, 21, ':');
	lcd_write_char16(1, 22, ' ');
	lcd_write_char16(1, 23, ' ');
	lcd_write_char16(1, 24, ' ');
	lcd_write_char16(1, 25, ' ');
	twoPos_rate >= 10 ? lcd_write_char16(1, 26, twoPos_rate / 10 + 48) : lcd_write_char16(1, 26, ' ');
	lcd_write_char16(1, 27, twoPos_rate % 10 + 48);
	lcd_write_char16(1, 28, 'm');
	lcd_write_char16(1, 29, 's');
	
	lcd_write_line16(2, "POS2: ");
	twoPos_secondPosition >= 10000 ? lcd_write_char16(2, 6, twoPos_secondPosition / 10000 + 48) : lcd_write_char16(2, 6, ' ');
	lcd_write_char16(2, 7, twoPos_secondPosition / 1000 % 10 + 48);
	lcd_write_char16(2, 8, twoPos_secondPosition / 100 % 10 + 48);
	lcd_write_char16(2, 9, twoPos_secondPosition / 10 % 10 + 48);
	lcd_write_char16(2, 10, '.');
	lcd_write_char16(2, 11, twoPos_secondPosition % 10 + 48);
	lcd_write_char16(2, 12, 'u');
	lcd_write_char16(2, 13, 's');
	lcd_write_char16(2, 14, ' ');
	lcd_write_char16(2, 15, ' ');
	lcd_write_char16(2, 16, ' ');
	lcd_write_char16(2, 17, 'W');
	lcd_write_char16(2, 18, 'A');
	lcd_write_char16(2, 19, 'I');
	lcd_write_char16(2, 20, 'T');
	lcd_write_char16(2, 21, ':');
	lcd_write_char16(2, 22, ' ');
	lcd_write_char16(2, 23, ' ');
	twoPos_wait >= 1000 ? lcd_write_char16(2, 24, twoPos_wait / 1000 + 48) : lcd_write_char16(2, 24, ' ');
	twoPos_wait >= 100 ? lcd_write_char16(2, 25, twoPos_wait / 100 % 10 + 48) : lcd_write_char16(2, 25, ' ');
	twoPos_wait >= 10 ? lcd_write_char16(2, 26, twoPos_wait / 10 % 10 + 48) : lcd_write_char16(2, 26, ' ');
	lcd_write_char16(2, 27, '.');
	lcd_write_char16(2, 28, twoPos_wait % 10 + 48);
	lcd_write_char16(2, 29, 's');
	
	lcd_write_line16(3, "               CYCLES: ");
	twoPos_cycles >= 1000000 ? lcd_write_char16(3, 23, twoPos_cycles / 1000000 + 48) : lcd_write_char16(3, 23, ' ');
	twoPos_cycles >= 100000 ? lcd_write_char16(3, 24, twoPos_cycles / 100000 % 10 + 48) : lcd_write_char16(3, 24, ' ');
	twoPos_cycles >= 10000 ? lcd_write_char16(3, 25, twoPos_cycles / 10000 % 10 + 48) : lcd_write_char16(3, 25, ' ');
	twoPos_cycles >= 1000 ? lcd_write_char16(3, 26, twoPos_cycles / 1000 % 10 + 48) : lcd_write_char16(3, 26, ' ');
	twoPos_cycles >= 100 ? lcd_write_char16(3, 27, twoPos_cycles / 100 % 10 + 48) : lcd_write_char16(3, 27, ' ');
	twoPos_cycles >= 10 ? lcd_write_char16(3, 28, twoPos_cycles / 10 % 10 + 48) : lcd_write_char16(3, 28, ' ');
	lcd_write_char16(3, 29, twoPos_cycles % 10 + 48);
	
	lcd_write_line16(4, ">> COMPLETED ");
	cyclesCompleted >= 1000000 ? lcd_write_char16(4, 13, cyclesCompleted / 1000000 + 48) : lcd_write_char16(4, 13, ' ');
	cyclesCompleted >= 100000 ? lcd_write_char16(4, 14, cyclesCompleted / 100000 % 10 + 48) : lcd_write_char16(4, 14, ' ');
	cyclesCompleted >= 10000 ? lcd_write_char16(4, 15, cyclesCompleted / 10000 % 10 + 48) : lcd_write_char16(4, 15, ' ');
	cyclesCompleted >= 1000 ? lcd_write_char16(4, 16, cyclesCompleted / 1000 % 10 + 48) : lcd_write_char16(4, 16, ' ');
	cyclesCompleted >= 100 ? lcd_write_char16(4, 17, cyclesCompleted / 100 % 10 + 48) : lcd_write_char16(4, 17, ' ');
	cyclesCompleted >= 10 ? lcd_write_char16(4, 18, cyclesCompleted / 10 % 10 + 48) : lcd_write_char16(4, 18, ' ');
	lcd_write_char16(4, 19, cyclesCompleted % 10 + 48);
	lcd_write_char16(4, 20, ' ');
	lcd_write_char16(4, 21, 'C');
	lcd_write_char16(4, 22, 'Y');
	lcd_write_char16(4, 23, 'C');
	lcd_write_char16(4, 24, 'L');
	lcd_write_char16(4, 25, 'E');
	lcd_write_char16(4, 26, 'S');
	lcd_write_char16(4, 27, ' ');
	lcd_write_char16(4, 28, '<');
	lcd_write_char16(4, 29, '<');
	
	// Underline "2pos"
	lcd_write_underline(20,23,0);
	
	// Underline current option
	if(drawUnderline == 1)
		lcd_draw_current_underline();
	
	// 15px block to represent the current servo pulse length
	uint8_t offset = (currentServoPulse - 8000) / 62;
	uint8_t i;
	for(i = 0; i < 240; i++) {
		if(i < offset || i > offset + 14) {
			pixBuf[i][10] = 0;
			pixBuf[i][11] = 0;
		} else {
			pixBuf[i][10] = 255;
			pixBuf[i][11] = 255;
		}
	}
	lcd_write_pixbuf();
	*/
}


void draw1posScreen() {
	/********************************
	 *MODE:  Sweep  3pos  2pos  1pos*
	 *POS:   900.0us                *
	 *RATE:     10ms                *
	 *                              *
	 *                              *
	 ********************************/
	/*
	lcd_write_line16(0, "MODE:  Sweep  3pos  2pos  1pos");
	
	lcd_write_line16(1, "POS:  ");
	onePos_position >= 10000 ? lcd_write_char16(1, 6, onePos_position / 10000 + 48) : lcd_write_char16(1, 6, ' ');
	lcd_write_char16(1, 7, onePos_position / 1000 % 10 + 48);
	lcd_write_char16(1, 8, onePos_position / 100 % 10 + 48);
	lcd_write_char16(1, 9, onePos_position / 10 % 10 + 48);
	lcd_write_char16(1, 10, '.');
	lcd_write_char16(1, 11, onePos_position % 10 + 48);
	lcd_write_char16(1, 12, 'u');
	lcd_write_char16(1, 13, 's');
	lcd_write_char16(1, 14, ' ');
	lcd_write_char16(1, 15, ' ');
	lcd_write_char16(1, 16, ' ');
	lcd_write_char16(1, 17, ' ');
	lcd_write_char16(1, 18, ' ');
	lcd_write_char16(1, 19, ' ');
	lcd_write_char16(1, 20, ' ');
	lcd_write_char16(1, 21, ' ');
	lcd_write_char16(1, 22, ' ');
	lcd_write_char16(1, 23, ' ');
	lcd_write_char16(1, 24, ' ');
	lcd_write_char16(1, 25, ' ');
	lcd_write_char16(1, 26, ' ');
	lcd_write_char16(1, 27, ' ');
	lcd_write_char16(1, 28, ' ');
	lcd_write_char16(1, 29, ' ');
	
	lcd_write_line16(2, "RATE:     ");
	onePos_rate >= 10 ? lcd_write_char16(2, 10, onePos_rate / 10 + 48) : lcd_write_char16(2, 10, ' ');
	lcd_write_char16(2, 11, onePos_rate % 10 + 48);
	lcd_write_char16(2, 12, 'm');
	lcd_write_char16(2, 13, 's');
	lcd_write_char16(2, 14, ' ');
	lcd_write_char16(2, 15, ' ');
	lcd_write_char16(2, 16, ' ');
	lcd_write_char16(2, 17, ' ');
	lcd_write_char16(2, 18, ' ');
	lcd_write_char16(2, 19, ' ');
	lcd_write_char16(2, 20, ' ');
	lcd_write_char16(2, 21, ' ');
	lcd_write_char16(2, 22, ' ');
	lcd_write_char16(2, 23, ' ');
	lcd_write_char16(2, 24, ' ');
	lcd_write_char16(2, 25, ' ');
	lcd_write_char16(2, 26, ' ');
	lcd_write_char16(2, 27, ' ');
	lcd_write_char16(2, 28, ' ');
	lcd_write_char16(2, 29, ' ');
	
	lcd_write_line16(3, "                              ");
	
	lcd_write_line16(4, "                              ");
	
	// Underline "1pos"
	lcd_write_underline(26,29,0);
	
	// Underline current option
	if(drawUnderline == 1)
		lcd_draw_current_underline();
	
	// 15px block to represent the current servo pulse length
	uint8_t offset = (currentServoPulse - 8000) / 62;
	uint8_t i;
	for(i = 0; i < 240; i++) {
		if(i < offset || i > offset + 14) {
			pixBuf[i][10] = 0;
			pixBuf[i][11] = 0;
		} else {
			pixBuf[i][10] = 255;
			pixBuf[i][11] = 255;
		}
	}
	lcd_write_pixbuf();
	*/
}

/*********************************************************************************
 * DRAW UNDERLINE BELOW CURRENTLY SELECTED OPTION                                *
 *********************************************************************************/
void lcd_draw_current_underline() {
	/*
	if(currentlySelectedOption == MODE)
		lcd_write_underline(0,3,0);
		
	else if(currentlySelectedOption == SWEEP_START)
		lcd_write_underline(0,4,1);
	else if(currentlySelectedOption == SWEEP_STOP)
		lcd_write_underline(0,3,2);
	else if(currentlySelectedOption == SWEEP_RATE)
		lcd_write_underline(17,20,1);
	else if(currentlySelectedOption == SWEEP_STEP)
		lcd_write_underline(17,20,2);
	else if(currentlySelectedOption == SWEEP_CYCLES)
		lcd_write_underline(15,20,3);
		
	else if(currentlySelectedOption == THREEPOS_1POS)
		lcd_write_underline(0,3,1);
	else if(currentlySelectedOption == THREEPOS_2POS)
		lcd_write_underline(0,3,2);
	else if(currentlySelectedOption == THREEPOS_3POS)
		lcd_write_underline(0,3,3);
	else if(currentlySelectedOption == THREEPOS_RATE)
		lcd_write_underline(17,20,1);
	else if(currentlySelectedOption == THREEPOS_WAIT)
		lcd_write_underline(17,20,2);
	else if(currentlySelectedOption == THREEPOS_CYCLES)
		lcd_write_underline(15,20,3);
		
	else if(currentlySelectedOption == TWOPOS_1POS)
		lcd_write_underline(0,3,1);
	else if(currentlySelectedOption == TWOPOS_2POS)
		lcd_write_underline(0,3,2);
	else if(currentlySelectedOption == TWOPOS_RATE)
		lcd_write_underline(17,20,1);
	else if(currentlySelectedOption == TWOPOS_WAIT)
		lcd_write_underline(17,20,2);
	else if(currentlySelectedOption == TWOPOS_CYCLES)
		lcd_write_underline(15,20,3);
		
	else if(currentlySelectedOption == ONEPOS_POSITION)
		lcd_write_underline(0,2,1);
	else if(currentlySelectedOption == ONEPOS_RATE)
		lcd_write_underline(0,3,2);
		*/
}
