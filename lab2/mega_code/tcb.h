#ifndef STRUCTDEF
#define STRUCTDEF
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
// April 23th by Kaiser Sun

// Define the colors
#define	RED     0xF800
#define	GREEN   0x07E0
#define	BLACK   0x0000

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