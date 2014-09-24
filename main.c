// Written by: Farrell Farahbod
// Last revised: 2014-09-23
// License: public domain

#include "stm32f0xx.h"
#include "f0lib/f0lib.h"
#include "externs.h"
#include "settings.h"

/*
 * Configures all peripherals, then enters an infinite loop that calls the screen draw functions.
 * 
 * Notes: Most of the constants used in this file are defined in /settings.h
 *        The F0 is rated for 48MHz but is being overclocked to 64MHz.
 *        The peripherals currently used (GPIOs, SPI, USART, Timers and ADC) tolerate it.
 */
void main(void) {
    // setup the LCD
    lcd_tft1p4705_setup(LCD_DATA_PORT,
                        LCD_CS_PIN,
                        LCD_RS_PIN,
                        LCD_WR_PIN,
                        LCD_RD_PIN,
                        LCD_RESET_PIN);

    // setup the four push buttons, switch common = gnd
    gpio_setup(PUSHBUTTON_LEFT_PIN, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0);
    exti_setup(PUSHBUTTON_LEFT_PIN, FALLING_EDGE);

    gpio_setup(PUSHBUTTON_RIGHT_PIN, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0);
    exti_setup(PUSHBUTTON_RIGHT_PIN, FALLING_EDGE);

    gpio_setup(PUSHBUTTON_UP_PIN, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0);
    exti_setup(PUSHBUTTON_UP_PIN, FALLING_EDGE);

    gpio_setup(PUSHBUTTON_DOWN_PIN, INPUT, PUSH_PULL, FIFTY_MHZ, PULL_UP, AF0);
    exti_setup(PUSHBUTTON_DOWN_PIN, FALLING_EDGE);
	
    // setup the PWM timer
    timer_pwm_setup(PWM_TIMER_PERIPH,
                    PWM_TIMER_PRESCALER,
                    PWM_TIMER_PERIOD,
                    FOUR_CHANNELS,
                    PWM_TIMER_CH1_PIN,
                    PWM_TIMER_CH2_PIN,
                    PWM_TIMER_CH3_PIN,
                    PWM_TIMER_CH4_PIN);

    // setup the CC2500 RF module
    cc2500_setup(CC2500_SPI_PERIPH,
                 CC2500_CLK_PIN,
                 CC2500_MISO_PIN,
                 CC2500_MOSI_PIN,
                 CC2500_CS_PIN);

    // setup the Bluetooth SPP module (RS232)
    rs232_setup(HC05_USART_PERIPH,
                HC05_RX_MCU_TX_PIN,
                HC05_BAUD_RATE);

    // make HC05 PIO11 pin (UART/AT mode select pin) low to select UART mode, then write test string
    gpio_setup(HC05_PIO11_PIN, OUTPUT, PUSH_PULL, FIFTY_MHZ, NO_PULL, AF0);
    gpio_low(HC05_PIO11_PIN);
    rs232_write_string(HC05_USART_PERIPH, "test\n");

    // setup the ADC, sample at 100Hz
    timer_timebase_setup(TIM1, 48000, 10, 0); // 48MHz/48k/10 = 100Hz update events, 0 = disable interrupt
    adc_setup(CLOCK_TIM1, 10,                 // clock ADC with TIM1, using 10 ADC channels
              TOUCHSCREEN_YD_PIN,
              TOUCHSCREEN_XL_PIN,
              TOUCHSCREEN_YU_PIN,
              TOUCHSCREEN_XR_PIN,
              ADC_BREAKOUT_PIN,
              VOLTAGE_SENSE_PIN,
              CURRENT1_SENSE_PIN,
              CURRENT2_SENSE_PIN,
              CURRENT3_SENSE_PIN,
              CURRENT4_SENSE_PIN);

    // read calibration constants from flash (page 63) to RAM
    struct PersistentData* ptr = (struct PersistentData*) 0x0800FC00;
    voltageCalibrationFactor = ptr->voltageCalibrationFactor;
    current1CalibrationFactor = ptr->current1CalibrationFactor;
    current2CalibrationFactor = ptr->current2CalibrationFactor;
    current3CalibrationFactor = ptr->current3CalibrationFactor;
    current4CalibrationFactor = ptr->current4CalibrationFactor;
    if(ptr->touchMinXCalibrationFactor < 500) {
        touchMinXCalibrationFactor = ptr->touchMinXCalibrationFactor;
        touchMaxXCalibrationFactor = ptr->touchMaxXCalibrationFactor;
        touchMinYCalibrationFactor = ptr->touchMinYCalibrationFactor;
        touchMaxYCalibrationFactor = ptr->touchMaxYCalibrationFactor;
    }

    // draw the current screen
    while(1) {
        if(currentScreen == SWEEP)
            drawSweepScreen();
        else if(currentScreen == THREE_POS)
            draw3posScreen();
        else if(currentScreen == TWO_POS)
            draw2posScreen();
        else if(currentScreen == ONE_POS)
            draw1posScreen();
        else if(currentScreen == SPEC_AN_LIVE)
            drawSpecAnLiveScreen();
        else if(currentScreen == SPEC_AN_WATERFALL)
            drawSpecAnWaterfallScreen();
        else if(currentScreen == CALIBRATION)
            drawCalibrationScreen();
        else if(currentScreen == GAME)
            drawGameScreen();
    }
}
