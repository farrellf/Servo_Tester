// Written by: Farrell Farahbod
// Last revised: 2014-09-23
// License: public domain

#include "f0lib/f0lib_lcd_tft1p4705.h"
#include "f0lib/f0lib_timers.h"
#include "externs.h"

uint32_t rowsCleared = 0;

/*
 * TIM14 is used to force the tetromino down by one row periodically.
 */
void TIM14_IRQHandler() {
	if(TIM14->SR & TIM_SR_UIF != 0) {
		TIM14->SR &= ~TIM_SR_UIF;

		// simulate a down button press
		notifyGameScreen(DOWN_BUTTON_PRESSED);

		// run again at a later time
		uint32_t waitTime = 500; // ms
		if(rowsCleared * 10 <= 490)
			waitTime = 500 - (rowsCleared * 10);
		timer_one_pulse_setup(TIM14, waitTime);
	}
}

struct {
	enum {L, J, I, S, Z, A, O} type;
	int32_t xOffset;
	int32_t yOffset;
	uint32_t rotation; // 0, 90, 180, 270. (clockwise rotations)
} tetromino;

#define SIZE_X 20
#define SIZE_Y 30
#define BLOCK_THICKNESS 16
uint8_t block[SIZE_X][SIZE_Y] = {0};
uint8_t block0x, block0y, block1x, block1y, block2x, block2y, block3x, block3y; // coordinates for the four blocks
uint8_t maxX, maxY; // used to ensure block stays on the screen
uint8_t ignoreEvents = 0;

void spawnTetromino() {
	static uint8_t number = 0;
	
	tetromino.type = number++;
	tetromino.xOffset = SIZE_X / 2 - 1;
	tetromino.yOffset = 0;
	tetromino.rotation = 0;

	if(number == 7)
		number = 0;
}

void drawGameScreen() {
	spawnTetromino();
	timer_one_pulse_setup(TIM14, 500); // ms;
	while(currentScreen == GAME);
}

void renderToLcd() {
	const uint16_t colors[8] = {
		0b0, // black
		0b1111101111100000, // orange
		0b0000000000011111, // blue
		0b0000011000011111, // cyan
		0b0000011111100000, // green
		0b1111100000000000, // red
		0b1000000000011111, // purple
		0b1111111111100000 // yellow
	};
	
	lcd_pixel_stream_start();
	for(uint32_t x = 0; x < SIZE_X; x++) {
		for(uint32_t y = 0; y < SIZE_Y; y++) {
			for(uint32_t row = 0; row < BLOCK_THICKNESS; row++) {
				lcd_pixel_stream_move_cursor(y*BLOCK_THICKNESS+row, (SIZE_X-1-x)*BLOCK_THICKNESS);
				//for(uint32_t i = 0; i < BLOCK_THICKNESS; i++)
					//if(	row == 0 || row == BLOCK_THICKNESS - 1 || i == 0 || i == BLOCK_THICKNESS -1)
						//lcd_pixel_stream_write_pixel(colors[0]);
					//else
						//lcd_pixel_stream_write_pixel(colors[block[x][y]]);
						
				// partially unrolled version of the above code
				lcd_pixel_stream_write_pixel(colors[0]);
				for(uint32_t i = 1; i < BLOCK_THICKNESS - 1; i++)
					if(	row == 0 || row == BLOCK_THICKNESS - 1)
						lcd_pixel_stream_write_pixel(colors[0]);
					else
						lcd_pixel_stream_write_pixel(colors[block[x][y]]);
				lcd_pixel_stream_write_pixel(colors[0]);
			}
		}
	}
	lcd_pixel_stream_stop();
}

void removeFullRows() {

	ignoreEvents = 1;

	// look through all rows, from the bottom up
	for(int32_t y = SIZE_Y - 1; y >= 0; y--) {
		// examine current row
		uint32_t blocksFound = SIZE_X;
		while(blocksFound == SIZE_X) {
			blocksFound = 0;
			for(uint32_t x = 0; x < SIZE_X; x++)
				if(block[x][y] != 0)
					blocksFound++;
			if(blocksFound == SIZE_X) { // full row
				for(uint32_t x = 0; x < SIZE_X; x++)
					block[x][y] = 0; // empty the full row
				renderToLcd();
				// move above rows down by 1
				for(int32_t row = y; row > 0; row--)
					for(uint32_t col = 0; col < SIZE_X; col++)
						block[col][row] = block[col][row-1];
				renderToLcd();
				rowsCleared++;
			}
		}
	}

	ignoreEvents = 0;

}

// determine relative coordinates for the current tetromino
void determineTetrominoCoordinates() {
	
	switch(tetromino.type) {

//		#
//		#
//		##
		case L:
			if(tetromino.rotation == 0) { // coords = 0,0   0,1   0,2   1,2
				block0x = 0; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 0; block2y = 2;
				block3x = 1; block3y = 2;
				maxX    = 1; maxY    = 2;
			} else if(tetromino.rotation == 90) { // coords = 0,0   1,0   2,0   0,1
				block0x = 0; block0y = 0;
				block1x = 1; block1y = 0;
				block2x = 2; block2y = 0;
				block3x = 0; block3y = 1;
				maxX    = 2; maxY    = 1;
			} else if(tetromino.rotation == 180) { // coords = 0,0   1,0   1,1   1,2
				block0x = 0; block0y = 0;
				block1x = 1; block1y = 0;
				block2x = 1; block2y = 1;
				block3x = 1; block3y = 2;
				maxX    = 1; maxY    = 2;
			} else if(tetromino.rotation == 270) { // coords = 2,0   0,1   1,1   2,1
				block0x = 2; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 1; block2y = 1;
				block3x = 2; block3y = 1;
				maxX    = 2; maxY    = 1;
			}
			break;

//		 #
//		 #
//		##
		case J:
			if(tetromino.rotation == 0) { // coords = 1,0   1,1   1,2   0,2
				block0x = 1; block0y = 0;
				block1x = 1; block1y = 1;
				block2x = 1; block2y = 2;
				block3x = 0; block3y = 2;
				maxX    = 1; maxY    = 2;
			} else if(tetromino.rotation == 90) { // coords = 0,0   0,1   1,1   2,1
				block0x = 0; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 1; block2y = 1;
				block3x = 2; block3y = 1;
				maxX    = 2; maxY    = 1;
			} else if(tetromino.rotation == 180) { // coords = 0,0   0,1   0,2   1,0
				block0x = 0; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 0; block2y = 2;
				block3x = 1; block3y = 0;
				maxX    = 1; maxY    = 2;
			} else if(tetromino.rotation == 270) { // coords = 0,0   1,0   2,0   2,1
				block0x = 0; block0y = 0;
				block1x = 1; block1y = 0;
				block2x = 2; block2y = 0;
				block3x = 2; block3y = 1;
				maxX    = 2; maxY    = 1;
			}
			break;

//		#
//		#
//		#
//		#
		case I:
			if(tetromino.rotation == 0 || tetromino.rotation == 180) { // coords = 0,0   0,1   0,2   0,3
				block0x = 0; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 0; block2y = 2;
				block3x = 0; block3y = 3;
				maxX    = 0; maxY    = 3;
			} else if(tetromino.rotation == 90 || tetromino.rotation == 270) { // coords = 0,0   1,0   2,0   3,0
				block0x = 0; block0y = 0;
				block1x = 1; block1y = 0;
				block2x = 2; block2y = 0;
				block3x = 3; block3y = 0;
				maxX    = 3; maxY    = 0;
			}
			break;

//		 ##
//		##
		case S:
			if(tetromino.rotation == 0 || tetromino.rotation == 180) { // coords = 1,0   2,0   0,1   1,1
				block0x = 1; block0y = 0;
				block1x = 2; block1y = 0;
				block2x = 0; block2y = 1;
				block3x = 1; block3y = 1;
				maxX    = 2; maxY    = 1;
			} else if(tetromino.rotation == 90 || tetromino.rotation == 270) { // coords = 0,0   0,1   1,1   1,2
				block0x = 0; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 1; block2y = 1;
				block3x = 1; block3y = 2;
				maxX    = 1; maxY    = 2;
			}
			break;

//		##
//		 ##
		case Z:
			if(tetromino.rotation == 0 || tetromino.rotation == 180) { // coords = 0,0   1,0   1,1   2,1
				block0x = 0; block0y = 0;
				block1x = 1; block1y = 0;
				block2x = 1; block2y = 1;
				block3x = 2; block3y = 1;
				maxX    = 2; maxY    = 1;
			} else if(tetromino.rotation == 90 || tetromino.rotation == 270) { // coords = 1,0   0,1   1,1   0,2
				block0x = 1; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 1; block2y = 1;
				block3x = 0; block3y = 2;
				maxX    = 1; maxY    = 2;
			}
			break;

//		 #
//		###
		case A:
			if(tetromino.rotation == 0) { // coords = 1,0   0,1   1,1   2,1
				block0x = 1; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 1; block2y = 1;
				block3x = 2; block3y = 1;
				maxX    = 2; maxY    = 1;
			} else if(tetromino.rotation == 90) { // coords = 0,0   0,1   1,1   0,2
				block0x = 0; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 1; block2y = 1;
				block3x = 0; block3y = 2;
				maxX    = 1; maxY    = 2;
			} else if(tetromino.rotation == 180) { // coords = 0,0   1,0   2,0   1,1
				block0x = 0; block0y = 0;
				block1x = 1; block1y = 0;
				block2x = 2; block2y = 0;
				block3x = 1; block3y = 1;
				maxX    = 2; maxY    = 1;
			} else if(tetromino.rotation == 270) { // coords = 1,0   0,1   1,1   1,2
				block0x = 1; block0y = 0;
				block1x = 0; block1y = 1;
				block2x = 1; block2y = 1;
				block3x = 1; block3y = 2;
				maxX    = 1; maxY    = 2;
			}
			break;

//		##
//		##
		case O:
			block0x = 0; block0y = 0; // coords = 0,0   1,0   0,1   1,1
			block1x = 1; block1y = 0;
			block2x = 0; block2y = 1;
			block3x = 1; block3y = 1;
			maxX    = 1; maxY    = 1;
			break;
			
	}
	
}

void notifyGameScreen(enum Event event) {
	uint8_t spawn = 0;
	static uint8_t gameOver = 0;

	if(gameOver == 1 || ignoreEvents == 1)
		return;
	
	determineTetrominoCoordinates();

	// vacate currently occupied blocks
	block[tetromino.xOffset + block0x][tetromino.yOffset + block0y] = 0;
	block[tetromino.xOffset + block1x][tetromino.yOffset + block1y] = 0;
	block[tetromino.xOffset + block2x][tetromino.yOffset + block2y] = 0;
	block[tetromino.xOffset + block3x][tetromino.yOffset + block3y] = 0;

	// apply change
	if(event == DOWN_BUTTON_PRESSED)
		tetromino.yOffset += 1;
	else if(event == LEFT_BUTTON_PRESSED)
		tetromino.xOffset -= 1;
	else if(event == RIGHT_BUTTON_PRESSED)
		tetromino.xOffset += 1;
	else if(event == UP_BUTTON_PRESSED)
		if(tetromino.rotation == 0)        tetromino.rotation = 90,  determineTetrominoCoordinates();
		else if(tetromino.rotation == 90)  tetromino.rotation = 180, determineTetrominoCoordinates();
		else if(tetromino.rotation == 180) tetromino.rotation = 270, determineTetrominoCoordinates();
		else if(tetromino.rotation == 270) tetromino.rotation = 0,   determineTetrominoCoordinates();
	spawn = 0;
	
	// if space is not available or moving off screen
	if(		block[tetromino.xOffset + block0x][tetromino.yOffset + block0y] != 0 ||
			block[tetromino.xOffset + block1x][tetromino.yOffset + block1y] != 0 ||
			block[tetromino.xOffset + block2x][tetromino.yOffset + block2y] != 0 ||
			block[tetromino.xOffset + block3x][tetromino.yOffset + block3y] != 0 ||
			tetromino.yOffset + maxY > SIZE_Y - 1 ||
			tetromino.xOffset + maxX > SIZE_X - 1 ||
			tetromino.xOffset < 0) {

		// spawn new tetromino if space is not available when trying to move down
		if(event == DOWN_BUTTON_PRESSED)
			if(	block[tetromino.xOffset + block0x][tetromino.yOffset + block0y] != 0 ||
				block[tetromino.xOffset + block1x][tetromino.yOffset + block1y] != 0 ||
				block[tetromino.xOffset + block2x][tetromino.yOffset + block2y] != 0 ||
				block[tetromino.xOffset + block3x][tetromino.yOffset + block3y] != 0 ||
				tetromino.yOffset + maxY > SIZE_Y - 1)
				if(tetromino.yOffset == 1)
					gameOver = 1; // no more room to spawn, game over.
				else
					spawn = 1;
				
		// revert changes
		if(event == DOWN_BUTTON_PRESSED)
			tetromino.yOffset -= 1;
		else if(event == LEFT_BUTTON_PRESSED)
			tetromino.xOffset += 1;
		else if(event == RIGHT_BUTTON_PRESSED)
			tetromino.xOffset -= 1;
		else if(event == UP_BUTTON_PRESSED)
			if(tetromino.rotation == 0)        tetromino.rotation = 270,  determineTetrominoCoordinates();
			else if(tetromino.rotation == 90)  tetromino.rotation = 0,    determineTetrominoCoordinates();
			else if(tetromino.rotation == 180) tetromino.rotation = 90,   determineTetrominoCoordinates();
			else if(tetromino.rotation == 270) tetromino.rotation = 180,  determineTetrominoCoordinates();

	}

	uint8_t color;
	if(tetromino.type == L)
		color = 1;
	else if(tetromino.type == J)
		color = 2;
	else if(tetromino.type == I)
		color = 3;
	else if(tetromino.type == S)
		color = 4;
	else if(tetromino.type == Z)
		color = 5;
	else if(tetromino.type == A)
		color = 6;
	else if(tetromino.type == O)
		color = 7;

	block[tetromino.xOffset + block0x][tetromino.yOffset + block0y] = color;
	block[tetromino.xOffset + block1x][tetromino.yOffset + block1y] = color;
	block[tetromino.xOffset + block2x][tetromino.yOffset + block2y] = color;
	block[tetromino.xOffset + block3x][tetromino.yOffset + block3y] = color;

	renderToLcd();

	if(spawn) {
		spawnTetromino();
		removeFullRows();
	}
	
}
