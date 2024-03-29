// #include <Elegoo_GFX.h>    // Core graphics library
// #include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "tcb.h"

// initialization started!
// initialize values
unsigned int temperatureRaw = 30;
    // there're problem of initial value of temp!
unsigned int systolicPressRaw = 80;
unsigned int diastolicPressRaw = 80;
unsigned int pulseRateRaw = 70;
unsigned int tempCorrected;
unsigned int systolicPressCorrected;
unsigned int diastolicPressCorrected;
unsigned int pulseRateCorrected;
unsigned short batteryState = 200;
unsigned char bpOutOfRange = 0;
unsigned char tempOutOfRange = 0;
unsigned char pulseOutOfRange = 0;
Bool bpHigh = FALSE;
Bool tempHigh = FALSE;
Bool pulseLow = FALSE;

// initialize pointers
unsigned int* temperatureRawPtrr = &temperatureRaw;
unsigned int* systolicPressRawPtrr = &systolicPressRaw;
unsigned int* diastolicPressRawPtrr = &diastolicPressRaw;
unsigned int* pulseRateRawPtrr = &pulseRateRaw;
unsigned int* tempCorrectedPtrr = &tempCorrected;
unsigned int* systolicPressCorrectedPtrr = &systolicPressCorrected;
unsigned int* diastolicPressCorrectedPtrr = &diastolicPressCorrected;
unsigned int* pulseRateCorrectedPtrr = &pulseRateCorrected;
unsigned short* batteryStatePtrr = &batteryState;

// initialize taskQueue and TCBs
TCB taskQueue[5];
TCB meas, comp, disp, alar, stat;
MeasureData meaD;
ComputeData cD;
DisplayData dDa;
WarningAlarmData wAD;
StatusData sD;

// Need initialization of data and TCBs


void setup()
{
    // Initialize the serial connection of 9600 bits per second
  Serial.begin(9600);
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


  // Setup the data structs
  meaD = MeasureData{temperatureRawPtrr, systolicPressRawPtrr, diastolicPressRawPtrr, pulseRateRawPtrr};
  //cD =  ComputeData{temperatureRawPtrr, systolicPressRawPtrr, diastolicPressRawPtrr, pulseRateRawPtrr, tempCorrectedPtrr, systolicPressCorrectedPtrr diasCorrectedPtrr, prCorrectedPtrr};
  cD = ComputeData{temperatureRawPtrr, systolicPressRawPtrr, diastolicPressRawPtrr, pulseRateRawPtrr, tempCorrectedPtrr, systolicPressCorrectedPtrr, diastolicPressCorrectedPtrr, pulseRateCorrectedPtrr};
  dDa = DisplayData{tempCorrectedPtrr, systolicPressCorrectedPtrr, diastolicPressCorrectedPtrr, pulseRateCorrectedPtrr, batteryStatePtrr};
  wAD = WarningAlarmData{temperatureRawPtrr, systolicPressRawPtrr, diastolicPressRawPtrr, pulseRateRawPtrr, batteryStatePtrr};
  sD = StatusData{batteryStatePtrr};

  // Setup the TCBs
  meas = {&Measure, &meaD};
  comp = {&Compute, &cD};
  disp = {&Display, &dDa};
  alar = {&WarningAlarm, &wAD};
  stat = {&Status, &sD};

  // Setup task queue
  taskQueue[0] = meas;
  taskQueue[1] = comp;
  taskQueue[2] = stat;
  taskQueue[3] = alar;
  taskQueue[4] = disp;

}

void loop()
{
    sechdulerTest(taskQueue);
}
