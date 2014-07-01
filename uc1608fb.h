/*********************************************************************************
 * SMALL LIBRARY FOR WRITING DATA TO UC1608FB LCD CONTROLLERS VIA BIT BANGING    *
 *********************************************************************************
 * The UC1608FB LCD controller supports up to 240x128 pixels but the LCD used in *
 * this project is 240x90.                                                       *
 *********************************************************************************
 * A 16x8 pixel font is used, allowing for 5.625 lines, each with 30 characters. *
 *********************************************************************************/

#include "simple_gpio.h"

enum REGISTER {COMMAND, DATA};
enum GPIO_PIN d7, d6, d5, d4, d3, d2, d1, d0, cd, wr1;

// Initialize the LCD
void lcd_init(enum GPIO_PIN _cd, enum GPIO_PIN _wr1, enum GPIO_PIN _d7, enum GPIO_PIN _d6, enum GPIO_PIN _d5,
			  enum GPIO_PIN _d4, enum GPIO_PIN _d3, enum GPIO_PIN _d2, enum GPIO_PIN _d1, enum GPIO_PIN _d0);
			  
// Send a byte (a command or pixel data) to the LCD. This is used by the other functions.
void lcd_write_register(enum REGISTER reg, uint8_t data);
			  
// Write one character to the pixel buffer.  line = 0-5, charOffset = 0-29, letter= ASCII 32-122
void lcd_write_char16(uint8_t line, uint8_t charOffset, uint8_t letter);

// Write one line to the pixel buffer, padding the end with spaces if needed.  line = 0-5, string = a c-string
void lcd_write_line16(uint8_t line, char string[]);

// Draw an underline in the pixel buffer.  x1 = 0-29, x2 = 0-29, y = 0-4
void lcd_write_underline(uint8_t x1, uint8_t x2, uint8_t y);

// Send the pixel buffer data to the LCD
void lcd_write_pixbuf();
