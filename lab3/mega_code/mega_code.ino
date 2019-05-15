// #include <Elegoo_GFX.h>    // Core graphics library
// #include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "tcb.h"
#include "task.h"

// initialization started!
// initialize raw values
//unsigned int temperatureRaw = 30;
//    // there're problem of initial value of temp!
//unsigned int systolicPressRaw = 80;
//unsigned int diastolicPressRaw = 80;
//unsigned int pulseRateRaw = 70;
////initialize corrected values
//unsigned int tempCorrected;
//unsigned int systolicPressCorrected;
//unsigned int diastolicPressCorrected;
//unsigned int pulseRateCorrected;
unsigned int temperatureRawBuf[8]= {30, 0, 0, 0, 0, 0, 0, 0};
unsigned int bloodPressRawBuf[16]= {80, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int pulseRateRawBuf[8] = {70, 0, 0, 0, 0, 0, 0, 0};

unsigned char tempCorrectedBuf[8]= {28, 0, 0, 0, 0, 0, 0, 0};
unsigned char bloodPressCorrectedBuf[16] = {126, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char pulseRateCorrectedBuf[8]= {218, 0, 0, 0, 0, 0, 0, 0};

unsigned short batteryState = 200;
unsigned char bpOutOfRange = 0;
unsigned char tempOutOfRange = 0;
unsigned char pulseOutOfRange = 0;
Bool bpHigh = FALSE;
Bool tempHigh = FALSE;
Bool pulseLow = FALSE;

unsigned long time1;
unsigned long time2;

//// initialize raw value pointers
unsigned int* temperatureRawPtrr = temperatureRawBuf;
//unsigned int* systolicPressCorrectedPtrr = &bloodPressCorrectedBuf;
//unsigned int* diastolicPressCorrectedPtrr = &diastolicPressCorrected;
unsigned int* bloodPressRawPtrr = bloodPressRawBuf;
unsigned int* pulseRateRawPtrr = pulseRateRawBuf;
//
////initialize corrected pointers
unsigned char* tempCorrectedPtrr = tempCorrectedBuf;
unsigned char* bloodPressCorrectedPtrr = bloodPressCorrectedBuf;
//unsigned int* systolicPressCorrectedPtrr = &bloodPressCorrectedBuf;
//unsigned int* diastolicPressCorrectedPtrr = &diastolicPressCorrected;
unsigned char* pulseRateCorrectedPtrr = pulseRateCorrectedBuf;
unsigned short* batteryStatePtrr = &batteryState;

//initialize keypad values and pointers
unsigned short measurementSelection = 0;
unsigned short alarmAcknowledge = 0;
unsigned short* measurementSelectionPtr = &measurementSelection;
unsigned short* alarmAcknowledgePtr = &alarmAcknowledge;


// initialize taskQueue and TCBs
//TCB taskQueue;
TCB meas, comp, disp, alar, stat, keyp;
MeasureData meaD;
ComputeData cD;
DisplayData dDa;
WarningAlarmData wAD;
StatusData sD;
KeypadData kD;


void setup() {
    // Initialize the serial connection of 9600 bits per second
    Serial.begin(9600);
    Serial1.begin(4800);
    // Print
    Serial.println(F("TFT LCD test"));


  #ifdef USE_Elegoo_SHIELD_PINOUT
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
  #else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
  #endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

   //Use identifier to make sure tft works
   uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
    } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
    } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
    }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
    } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
    } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
    } else if(identifier==0x0101)
    {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }
  else if(identifier==0x1111)
    {     
      identifier=0x9328;
       Serial.println(F("Found 0x9328 LCD driver"));
    }
    else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9328;
  
    }
    tft.begin(identifier);
    tft.setRotation(1);

  // Setup the data structs
  meaD = MeasureData{temperatureRawPtrr, bloodPressRawPtrr, pulseRateRawPtrr, measurementSelectionPtr};
  cD = ComputeData{temperatureRawPtrr, bloodPressRawPtrr, pulseRateRawPtrr, tempCorrectedPtrr, bloodPressCorrectedPtrr, pulseRateCorrectedPtrr, measurementSelectionPtr};
  dDa = DisplayData{tempCorrectedPtrr, bloodPressCorrectedPtrr, pulseRateCorrectedPtrr, batteryStatePtrr};
  wAD = WarningAlarmData{temperatureRawPtrr, bloodPressRawPtrr, pulseRateRawPtrr, batteryStatePtrr};
  sD = StatusData{batteryStatePtrr};
  kD = KeypadData{measurementSelectionPtr, alarmAcknowledgePtr};
//
//  meaD = MeasureData{temperatureRawBuf, bloodPressRawBuf, pulseRateRawBuf, measurementSelectionPtr};
//  cD = ComputeData{temperatureRawBuf, bloodPressRawBuf, pulseRateRawBuf, tempCorrectedBuf, bloodPressCorrectedBuf, pulseRateCorrectedBuf, measurementSelectionPtr};
//  dDa = DisplayData{tempCorrectedBuf, bloodPressCorrectedBuf, pulseRateCorrectedBuf, batteryStatePtrr};
//  wAD = WarningAlarmData{temperatureRawBuf, bloodPressRawBuf, pulseRateRawBuf, batteryStatePtrr};
//  sD = StatusData{batteryStatePtrr};
//  kD = KeypadData{measurementSelectionPtr, alarmAcknowledgePtr};
  // Setup the TCBs
  meas = {&Measure, &meaD};
  comp = {&Compute, &cD};
  disp = {&Display, &dDa};
  alar = {&WarningAlarm, &wAD};
  stat = {&Status, &sD};
  keyp = {&Select, &kD};
  
  // Setup task queue
  insertLast(&meas);
  insertLast(&comp);
  insertLast(&stat);
  insertLast(&alar);
  //insertLast(&keyp);
  time1 = millis();
  // taskQueue[4] = disp;
  run(&keyp);
  
}




void loop()
{
    //sechdulerTest();
    time2 = millis();
   //  Serial.print(time1);
    //Serial.print(" ");
    //Serial.println(time2);
    if(time2 - time1 > 2000) {
      insertLast(&keyp);
      sechdulerTest();
      deleteNode(&keyp);
      time1 = time2;
    } else {
      // sechdulerTest();
    }
    
    
    
}
