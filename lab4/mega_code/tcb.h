#ifndef STRUCTDEF
#define STRUCTDEF
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h> // TouchScreen library
#include <Adafruit_TFTLCD.h> 
#include <Adafruit_GFX.h> 
// April 23th by Kaiser Sun
// May 8th modified by Kaiser Sun, add touch screen


// pin assignments for TFT
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define BLACK   0x0000
#define GREY    0x7BEF 
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// touch screen settings
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

//Touch For New ILI9341 TP
#define TS_MINX 115
#define TS_MAXX 960

#define TS_MINY 125
#define TS_MAXY 920
// We have a status line for like, is FONA working
#define STATUS_X 65
#define STATUS_Y 10

// Set up bound for pressure
#define MINPRESSURE 5
#define MAXPRESSURE 1000

// Macro for location on screen
#define MENU(x, y) (y > 0) && (y < 240) && (x > 0) && (x < 200)
#define ANN(x, y) (y > 0) && (y < 240) && (x > 200) //&& (x < 330)
#define MEAS(x, y) 
#define QUIT1(x, y) (y > 0) && (y < 240) && (x > 0) && (x < 70)
#define T(x, y) (x < 330) && (x > 70) && (y < 300) && (y > 200)
#define BP(x, y) (x < 330) && (x > 70) && (y < 100) && (y > 0)
#define PR(x, y) (x < 330) && (x > 70) && (y < 200) && (y > 100)
#define QUIT2(x, y) (x > 0) && (y > 180) //&& (y < 240)
//Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);



// Define bool tyoe
enum myBool {FALSE = 0, TRUE = 1};
typedef enum myBool Bool;

// Declare TCB
// typedef struct 
// {  
//     void (*myTask)(void*);
//     void* taskDataPtr; 

// } TCB;

struct MyStruct
{
void (*myTask)(void*);
void* taskDataPtr;
struct MyStruct* next;
struct MyStruct* prev;
};
typedef struct MyStruct TCB;

//this link always point to first Link
TCB* front = NULL;

//this link always point to last Link
TCB* back = NULL;
void insertLast(TCB* node);
TCB* deleteNode(TCB* node);

// Declare the functions
    void Measure(void* dataPtr);
    void Compute(void* dataPtr);
    void Status(void* dataPtr);
    void WarningAlarm(void* dataPtr);
    void Display(void* dataPtr);
    void Scheduler(TCB* taskQueue);
    void sechdulerTest();
    void startUp();
typedef struct
{
    unsigned int* temperatureRawBuf;
    unsigned int* bloodPressRawBuf;
    unsigned int* pulseRateRawBuf;
    unsigned int* respirationRateRawBuf;
    unsigned short* measurementSelectionPtr;
} MeasureData;

typedef struct
{
    unsigned int* temperatureRawBuf;
    unsigned int* bloodPressRawBuf;
    unsigned int* pulseRateRawBuf;
    unsigned int* respirationRateRawBuf;
    unsigned char* tempCorrectedBuf;
    unsigned char* bloodPressCorrectedBuf;
    unsigned char* prCorrectedBuf;
    unsigned char* respirationRateCorBufPtr;
    unsigned short* measurementSelectionPtr;
} ComputeData;

typedef struct
{
    unsigned short* batteryStatePtr;
} StatusData;

typedef struct
{
  unsigned short* Mode;
  unsigned char* tempCorrectedBuf;
  unsigned char* bloodPressCorrectedBuf;
  unsigned char* prCorrectedBuf;
  unsigned char* respirationRateCorrectedBuf;
  unsigned short* batteryStatePtr;
} DisplayData;

typedef struct
{
    unsigned int* temperatureRawBuf;
    unsigned int* bloodPressRawBuf;
    unsigned int* pulseRateRawBuf;
    unsigned int* rrRawBuf;
    unsigned short* batteryStatePtr; 
} WarningAlarmData;

typedef struct
{
    unsigned short* localFunctionSelectPtr;
    unsigned short* measurementSelectionPtr;
    unsigned short* alarmAcknowledgePtr;
    unsigned short* commandPtr;
    unsigned short* remoteFunctionSelectPtr;
    unsigned short* measurementResultSelectionPtr;
} KeypadData;

typedef struct
{
    unsigned char* tempCorrectedBuf;
    unsigned char* bloodPressCorrectedBuf;
    unsigned char* prCorrectedBuf;
    unsigned char* respirationRateCorBufPtr;
} CommunicationsData;


# endif
