// Written by: Farrell Farahbod
// Last revised: 2014-09-15
// License: public domain

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   SERVO TESTER CONSTRAINTS                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define SERVO_PULSE_MIN  8000     // 800.0us
#define SERVO_PULSE_MAX  22000    // 2200.0us
#define SERVO_PERIOD_MIN 3        // 3ms (f=333Hz)
#define SERVO_PERIOD_MAX 30       // 30ms (f=33Hz)
#define SERVO_STEP_MIN   1        // 0.1us
#define SERVO_STEP_MAX   9999     // 999.9us
#define SERVO_WAIT_MIN   1        // 0.1s
#define SERVO_WAIT_MAX   999      // 99.9s
#define SERVO_CYCLES_MAX 9999999

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   GPIO USAGE AND RELATED SETTINGS                                          //
//                                                                            //
//   DON'T FORGET TO UPDATE THE ISRs IF YOU CHANGE PIN ASSIGNMENTS!           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// LCD: 320x480 16bit RGB565, with D51E5TA7601 controller IC
#define LCD_DATA_PORT        GPIOC  // DB0-15 pins
#define LCD_CS_PIN           PB8    // chip select
#define LCD_RS_PIN           PB9    // register select
#define LCD_WR_PIN           PB2    // write
#define LCD_RD_PIN           PF5    // read
#define LCD_RESET_PIN        PF4    // active low

// Pushbuttons (optional, can use touchscreen instead)
#define PUSHBUTTON_LEFT_PIN  PA11
#define PUSHBUTTON_RIGHT_PIN PA12
#define PUSHBUTTON_UP_PIN    PF6
#define PUSHBUTTON_DOWN_PIN  PF7

// PWM Timer: used to generate the four servo signals
#define PWM_TIMER_PERIPH     TIM2
#define PWM_TIMER_PRESCALER  24*OC_FACTOR       // 48MHz / 24 = 2MHz
#define PWM_TIMER_PERIOD     sweep_period*2000  // *2000 converts from milliseconds to number of clock ticks at 2MHz
#define PWM_TIMER_CH1_PIN    PA15
#define PWM_TIMER_CH2_PIN    PB3
#define PWM_TIMER_CH3_PIN    PB10
#define PWM_TIMER_CH4_PIN    PB11

// CC2500 module: used as spectrum analyzer
#define CC2500_SPI_PERIPH    SPI2
#define CC2500_CLK_PIN       PB13
#define CC2500_MISO_PIN      PB14
#define CC2500_MOSI_PIN      PB15
#define CC2500_CS_PIN        PB12

// Bluetooth module: HC05, factory config is 9600 8N1, device name = HC-05, passcode = 1234
#define HC05_USART_PERIPH    USART1
#define HC05_BAUD_RATE       9600*OC_FACTOR_RECIPRICOL
#define HC05_TX_MCU_RX_PIN   PB7    // read from this pin
#define HC05_RX_MCU_TX_PIN   PB6    // write to this pin
#define HC05_PIO11_PIN       PB4    // make high to send AT (must use \r\n) commands, make low to use as UART
#define HC05_PIO1_PIN        PA9    // status pin: low indicates disconnected, high indicates connected
#define HC05_PIO0_PIN        PA10   // make high to force disconnect

// ADC: touchscreen, voltage and current sensing
#define TOUCHSCREEN_YD_PIN   PA0
#define TOUCHSCREEN_XL_PIN   PA1
#define TOUCHSCREEN_YU_PIN   PA2
#define TOUCHSCREEN_XR_PIN   PA3
#define ADC_BREAKOUT_PIN     PA4    // not used in current firmware
#define VOLTAGE_SENSE_PIN    PA5    // voltage divider taps the battery voltage
#define CURRENT1_SENSE_PIN   PA6    // TI INA199A1 current amps: 50x gain, 10mOhm shunts
#define CURRENT2_SENSE_PIN   PA7
#define CURRENT3_SENSE_PIN   PB0
#define CURRENT4_SENSE_PIN   PB1

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   OTHER                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// overclocked by 33.3% (48 -> 64MHz)
#define OC_FACTOR 4/3
#define OC_FACTOR_RECIPRICOL 3/4

// touchscreen readouts calibrated to 0-3000 range
#define TOUCH_MAX_VALUE 3000
#define TOUCH_MIN_RAW_VALUE 100
