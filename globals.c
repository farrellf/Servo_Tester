// Written by: Farrell Farahbod
// Last revised: 2014-09-15
// License: public domain

#include "stm32f0xx.h"
#include "externs.h"

// screen to show at power up
volatile enum Screen currentScreen = SWEEP;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   SERVO TESTER GLOBALS                                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// general servo tester state
volatile uint32_t cyclesCompleted = 0;
volatile uint16_t currentServoPulse = 9000;

// single-position servo tester settings
volatile uint16_t onePos_position = 15000;
volatile uint16_t onePos_period = 10;

// two-position servo tester settings
volatile uint16_t twoPos_pos1 = 9000;
volatile uint16_t twoPos_pos2 = 21000;
volatile uint16_t twoPos_wait = 18;
volatile uint16_t twoPos_period = 10;
volatile uint32_t twoPos_cycles = 0;

// three-position servo tester settings
volatile uint16_t threePos_pos1 = 9000;
volatile uint16_t threePos_pos2 = 15000;
volatile uint16_t threePos_pos3 = 21000;
volatile uint16_t threePos_wait = 10;
volatile uint16_t threePos_period = 10;
volatile uint32_t threePos_cycles = 0;

// sweep servo tester settings
volatile uint16_t sweep_pos1 = 9000;
volatile uint16_t sweep_pos2 = 21000;
volatile uint16_t sweep_step = 70;
volatile uint16_t sweep_period = 10;
volatile uint32_t sweep_cycles = 0;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   ADC GLOBALS                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// rolling averages of 12-bit ADC readings. no units. (0-4095)
volatile uint32_t voltageRaw;
volatile uint32_t current1raw;
volatile uint32_t current2raw;
volatile uint32_t current3raw;
volatile uint32_t current4raw;
volatile uint32_t breakoutPinRaw;
volatile int32_t touchXraw;
volatile int32_t touchYraw;

// rolling averages of ADC readings, calibrated for 1 count = 1mV or 1mA
volatile uint32_t voltage = 0;
volatile uint32_t current1 = 0;
volatile uint32_t current2 = 0;
volatile uint32_t current3 = 0;
volatile uint32_t current4 = 0;
volatile int32_t touchX = 0;
volatile int32_t touchY = 0;

// calibration factors applied to raw values. defaults listed below are overwritten by values stored in flash.
// voltage = voltageRaw * 5000 / voltageCalibrationFactor
// currentN = currentNraw * 1000 / currentNCalibrationFactor
volatile uint32_t voltageCalibrationFactor = 5000;
volatile uint32_t current1CalibrationFactor = 1000;
volatile uint32_t current2CalibrationFactor = 1000;
volatile uint32_t current3CalibrationFactor = 1000;
volatile uint32_t current4CalibrationFactor = 1000;
volatile int32_t touchMinXCalibrationFactor = 0;
volatile int32_t touchMaxXCalibrationFactor = 3000;
volatile int32_t touchMinYCalibrationFactor = 0;
volatile int32_t touchMaxYCalibrationFactor = 3000;
