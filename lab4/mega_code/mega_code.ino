// #include <Elegoo_GFX.h>    // Core graphics library
// #include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "tcb.h"
#include "task.h"
//#include "helpers.h"
//
//bool dispBP = TRUE;
//bool dispT = TRUE;
//bool dispPR = TRUE;
//bool dispRR = TRUE;

//bool refSelect = TRUE;
//bool refMenu = TRUE;
//bool refAnnu = TRUE;

// initialization started!
unsigned int temperatureRawBuf[8]= {30, 0, 0, 0, 0, 0, 0, 0};
unsigned int bloodPressRawBuf[16]= {80, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int pulseRateRawBuf[8] = {70, 0, 0, 0, 0, 0, 0, 0};
unsigned int respirationRateRawBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned char tempCorrectedBuf[8]= {28, 0, 0, 0, 0, 0, 0, 0};
unsigned char bloodPressCorrectedBuf[16] = {126, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char pulseRateCorrectedBuf[8]= {218, 0, 0, 0, 0, 0, 0, 0};
unsigned char respirationRateCorBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned short Mode = 0;
unsigned short batteryState = 200;
unsigned long time1;
unsigned long time2;
unsigned long time3;
unsigned long timec;
unsigned long timeb;

unsigned int counter = 0;

// initialize raw value pointers
unsigned int* temperatureRawPtrr = temperatureRawBuf;
unsigned int* bloodPressRawPtrr = bloodPressRawBuf;
unsigned int* pulseRateRawPtrr = pulseRateRawBuf;
unsigned int* respirationRateRawPtr = respirationRateRawBuf;
//initialize corrected pointers
unsigned short* ModePtrr = &Mode;
unsigned char* tempCorrectedPtrr = tempCorrectedBuf;
unsigned char* bloodPressCorrectedPtrr = bloodPressCorrectedBuf;
unsigned char* pulseRateCorrectedPtrr = pulseRateCorrectedBuf;
unsigned short* batteryStatePtrr = &batteryState;
unsigned char* respirationRateCorPtr = respirationRateCorBuf;

//initialize keypad values and pointers
unsigned short localFunctionSelect = 0;
unsigned short measurementSelection = 0;
unsigned short alarmAcknowledge = 0;
unsigned short command = 0;
unsigned short remoteFunctionSelect = 0;
unsigned short measurementResultSelection = 0;
unsigned short displaySelection = 0;
unsigned short* displaySelectionPtr = &displaySelection;
unsigned short* localFunctionSelectPtr = &localFunctionSelect;
unsigned short* measurementSelectionPtr = &measurementSelection;
unsigned short* alarmAcknowledgePtr = &alarmAcknowledge;
unsigned short* commandPtr = &command;
unsigned short* remoteFunctionSelectPtr = &remoteFunctionSelect;
unsigned short* measurementResultSelectionPtr = &measurementResultSelection;

// initialize taskQueue and TCBs
//TCB taskQueue;
TCB meas, comp, disp, alar, stat, keyp, com;
MeasureData meaD;
ComputeData cD;
DisplayData dDa;
WarningAlarmData wAD;
StatusData sD;
KeypadData kD;
CommunicationsData comD;


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
    startUp();
}



void loop()
{
    time2 = millis();

    timec = millis();

    if (timec - timeb > 500) {
      counter++;
      timeb = timec;
    }
    
    if (time2 - time1 > 1) {
      if(*(measurementSelectionPtr) == 0 && *(alarmAcknowledgePtr) == 0 && *(displaySelectionPtr) == 0) {
        Select((void*)&kD);
      }
      if(*(measurementSelectionPtr) == 1) {
        
        menu(&kD);
        
      } else if(*(alarmAcknowledgePtr) == 1) {
            Serial.println("Announciation mode");
            *ModePtrr = 0;
            
            anno(&kD); 
            if (time2 - time1 > 2000) {
              (*disp.myTask)(disp.taskDataPtr);
            }
   
      } else if (*(displaySelectionPtr) == 1) {
          Serial.println("Display mode");
         *ModePtrr = 1;
          Measurement(&kD);
          if (time2 - time1 > 2000) {
              (*disp.myTask)(disp.taskDataPtr);
          }
   
      }
      
    }

    if(time2 - time1 > 2000) {

      sechdulerTest();
      time1 = time2; 
      
    } else {
   
    }
}
