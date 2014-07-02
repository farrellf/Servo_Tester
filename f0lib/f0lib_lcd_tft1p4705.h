// Written by Farrell Farahbod
// Last revised on 2014-07-01
// This file is released into the public domain

#include "stm32f0xx.h"
#include "f0lib_gpio.h"
#include "stdarg.h"

#ifndef F0LIB_LCD_TFT1P4705
#define F0LIB_LCD_TFT1P4705

// A crude way to waste time, effectively NOPing. Duration does not have units but 500,000 ~= 0.5s
void wait(uint32_t duration);

// Initialize the LCD controller
void lcd_tft1p4705_setup(	enum GPIO_PORT data_pins_port,
							enum GPIO_PIN cs_pin,
							enum GPIO_PIN rs_pin,
							enum GPIO_PIN wr_pin,
							enum GPIO_PIN rd_pin,
							enum GPIO_PIN reset_pin);

// This is a generic 8080 16bit register write function (not actually specific to this LCD)
// If porting to a faster processor, you may need to add delays.
// See datasheet pg. 159 for timing requirements.
void lcd_write_register(uint16_t reg, uint16_t val);

uint16_t lcd_read_register(uint16_t reg);

void lcd_write_pixel(uint16_t x, uint16_t y, uint16_t color);

void lcd_write_char(uint16_t col, uint16_t row, char c);

void lcd_write_string(uint16_t x, uint16_t y, char string[]);

void lcd_printf(uint16_t x, uint16_t y, char s[], ...);

#endif
