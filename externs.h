// Written by: Farrell Farahbod
// Last revised: 2014-09-15
// License: public domain

#include "stm32f0xx.h"

// screen to show at power up
extern volatile enum Screen currentScreen;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   DATA TYPES                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// possible screens
enum Screen {SWEEP, THREE_POS, TWO_POS, ONE_POS, SPEC_AN_LIVE, SPEC_AN_WATERFALL, CALIBRATION, GAME};

// possible events
enum Event {LEFT_BUTTON_PRESSED,
            RIGHT_BUTTON_PRESSED,
            UP_BUTTON_PRESSED,
            DOWN_BUTTON_PRESSED};

// calibration constants stored in flash (non-volatile)
struct PersistentData {
	volatile uint32_t voltageCalibrationFactor;
	volatile uint32_t current1CalibrationFactor;
	volatile uint32_t current2CalibrationFactor;
	volatile uint32_t current3CalibrationFactor;
	volatile uint32_t current4CalibrationFactor;
	volatile int32_t touchMinXCalibrationFactor;
	volatile int32_t touchMaxXCalibrationFactor;
	volatile int32_t touchMinYCalibrationFactor;
	volatile int32_t touchMaxYCalibrationFactor;
};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   SERVO TESTER EXTERNS                                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// general servo tester state
extern volatile uint32_t cyclesCompleted;
extern volatile uint16_t currentServoPulse;

// single-position servo tester settings
extern volatile uint16_t onePos_position;
extern volatile uint16_t onePos_period;

// two-position servo tester settings
extern volatile uint16_t twoPos_pos1;
extern volatile uint16_t twoPos_pos2;
extern volatile uint16_t twoPos_wait;
extern volatile uint16_t twoPos_period;
extern volatile uint32_t twoPos_cycles;

// three-position servo tester settings
extern volatile uint16_t threePos_pos1;
extern volatile uint16_t threePos_pos2;
extern volatile uint16_t threePos_pos3;
extern volatile uint16_t threePos_wait;
extern volatile uint16_t threePos_period;
extern volatile uint32_t threePos_cycles;

// sweep servo tester settings
extern volatile uint16_t sweep_pos1;
extern volatile uint16_t sweep_pos2;
extern volatile uint16_t sweep_step;
extern volatile uint16_t sweep_period;
extern volatile uint32_t sweep_cycles;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   ADC EXTERNS                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// rolling averages of 12-bit ADC readings. no units. (0-4095)
extern volatile uint32_t voltageRaw;
extern volatile uint32_t current1raw;
extern volatile uint32_t current2raw;
extern volatile uint32_t current3raw;
extern volatile uint32_t current4raw;
extern volatile uint32_t breakoutPinRaw;
extern volatile int32_t touchXraw;
extern volatile int32_t touchYraw;

// rolling averages of ADC readings, calibrated for 1 count = 1mV or 1mA
extern volatile uint32_t voltage;
extern volatile uint32_t current1;
extern volatile uint32_t current2;
extern volatile uint32_t current3;
extern volatile uint32_t current4;
extern volatile int32_t touchX;
extern volatile int32_t touchY;

// calibration factors applied to raw values.
// voltage = voltageRaw * 5000 / voltageCalibrationFactor
// currentN = currentNraw * 1000 / currentNCalibrationFactor
extern volatile uint32_t voltageCalibrationFactor;
extern volatile uint32_t current1CalibrationFactor;
extern volatile uint32_t current2CalibrationFactor;
extern volatile uint32_t current3CalibrationFactor;
extern volatile uint32_t current4CalibrationFactor;
extern volatile int32_t touchMinXCalibrationFactor;
extern volatile int32_t touchMaxXCalibrationFactor;
extern volatile int32_t touchMinYCalibrationFactor;
extern volatile int32_t touchMaxYCalibrationFactor;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   FUNCTION PROTOTYPES                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void drawSweepScreen(void);
void draw3posScreen(void);
void draw2posScreen(void);
void draw1posScreen(void);
void drawSpecAnLiveScreen(void);
void drawSpecAnWaterfallScreen(void);
void drawCalibrationScreen(void);
void drawGameScreen(void);

void notifySweepScreen(enum Event event);
void notify3posScreen(enum Event event);
void notify2posScreen(enum Event event);
void notify1posScreen(enum Event event);
void notifySpecAnLiveScreen(enum Event event);
void notifySpecAnWaterfallScreen(enum Event event);
void notifyCalibrationScreen(enum Event event);
void notifyGameScreen(enum Event event);
