#ifndef STRUCTDEF
#define STRUCTDEF
#include "Elegoo_GFX.h"    // Core graphics library
#include "Elegoo_TFTLCD.h" // Hardware-specific library
// April 23th by Kaiser Sun


// pin assignments for TFT
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Define bool tyoe
enum myBool {FALSE = 0, TRUE = 1};
typedef enum myBool Bool;

// Declare the functions
    void Measure(void* dataPtr);
    void Compute(void* dataPtr);
    void Status(void* dataPtr);
    void WarningAlarm(void* dataPtr);
    void Display(void* dataPtr);
    void Scheduler(TCB* taskQueue);

// Declare TCB
typedef struct
{
        void (*myTask)(void*);
        void* taskDataPtr; 
} TCB;

typedef struct
{
    int* temperatureRawPtr;
    int* systolicPressRawPtr;
    int* diastolicPressRawPtr;
    int* pulseRateRawPtr;
} MeasureData;

typedef struct
{
    int* temperatureRawPtr;
    int* systolicPressRawPtr;
    int* diastolicPressRawPtr;
    int* pulseRateRawPtr;
    int* tempCorrectedPtr;
    int* sysPressCorrectedPtr;
    int* diasCorrectedPtr;
    int* prCorrectedPtr;
} ComputeData;

typedef struct
{
    int* batteryStatePtr;
} StatusData;

typedef struct
{
  char* tempCorrectedPtr;
  char* sysPressCorrectedPtr;
  char* diastolicPressCorrectedPtr;
  char* pulseRateCorrectedPtr;
  short* batteryStatePtr;
} DisplayData;

typedef struct
{
    int* temperatureRawPtr;
    int* systolicPressRawPtr;
    int* diastolicPressRawPtr;
    int* pulseRateRawPtr;
    short* batteryStatePtr; 
} WarningAlarmData;

// typedef struct
// {

// } SchedulerData;


# endif
