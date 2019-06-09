// #include <Elegoo_GFX.h>    // Core graphics library
// #include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "tcb.h"
#include "task.h"
// #include "helpers.h"



bool startF = FALSE;

unsigned int BPindex = 0;
unsigned int PRindex = 0;
unsigned int Tindex = 0;


// initialization started!
unsigned int temperatureRawBuf[8]= {30, 0, 0, 0, 0, 0, 0, 0};
unsigned int bloodPressRawBuf[16]= {80, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int pulseRateRawBuf[8] = {70, 0, 0, 0, 0, 0, 0, 0};
unsigned int respirationRateRawBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
signed int EKGRawBuf[256];
signed int EKGImgBuf[256] = {0};

unsigned char EKGFreqBuf[16] = {0};

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
unsigned long timeRD1;
unsigned long timeRD2;
// Emergency data
unsigned long timeEmer1;
unsigned long timeEmer2;

unsigned int counterBP = 0;
unsigned int counterT = 0;
unsigned int counterPR = 0;

// initialize raw value pointers
unsigned int* temperatureRawPtrr = temperatureRawBuf;
unsigned int* bloodPressRawPtrr = bloodPressRawBuf;
unsigned int* pulseRateRawPtrr = pulseRateRawBuf;
unsigned int* respirationRateRawPtr = respirationRateRawBuf;
unsigned int* EKGRawBufPtr = EKGRawBuf;
unsigned int* EKGImgBufPtr = EKGImgBuf;
unsigned char* EKGFreqBufPtr = EKGFreqBuf;
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
unsigned short displayTM = 0;
unsigned short displayEmergency = 0;
unsigned short displayGame = 0;

unsigned short* displayTMPtr = &displayTM;
unsigned short* displayEmergencyPtr = &displayEmergency;
unsigned short* displayGamePtr = &displayGame;
unsigned short* displaySelectionPtr = &displaySelection;
unsigned short* localFunctionSelectPtr = &localFunctionSelect;
unsigned short* measurementSelectionPtr = &measurementSelection;
unsigned short* alarmAcknowledgePtr = &alarmAcknowledge;
unsigned short* commandPtr = &command;
unsigned short* remoteFunctionSelectPtr = &remoteFunctionSelect;
unsigned short* measurementResultSelectionPtr = &measurementResultSelection;

// Count the number of emergencies
int tWCounter = 0, bpWCounter = 0, rrWCounter = 0, ekgWCounter = 0, prWCounter = 0;

// initialize taskQueue and TCBs
//TCB taskQueue;
TCB meas, comp, disp, alar, stat, keyp, com, remDisp;
MeasureData meaD;
//EKGData EKGD;
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


int n = 0;

void loop()
{
    time2 = millis();
    timec = millis();
    timeRD2 = millis();
    timeEmer2 = millis();
    // Check if there is an emergency
    if(timeEmer2 - timeEmer1 > 28800000) {
      if(bpWCounter > 100 || tWCounter > 100 || rrWCounter > 100||  ekgWCounter > 100 || prWCounter > 0) {
          emergency();
      }
      bpWCounter = 0;
      tWCounter = 0;
      rrWCounter = 0;
      ekgWCounter = 0;
      timeEmer1 = timeEmer2;
    }
    // Call remote display every 5 seconds
    if(timeRD2 - timeRD1 > 5000) {
      insertLast(&remDisp);
      schedulerTest();
      timeRD1 = timeRD2;
      deleteNode(&remDisp);
    }

    if (timec - timeb > 500) {
      counterBP++; 
      counterT++;
      counterPR++;
      timeb = timec;  
    } 
    if (counterT == 5) {
      counterT = 1;
    }
    if (counterPR == 9) {
      counterPR = 1;
    }
    if (counterBP == 3) {
      counterBP = 1;
    }
    
    if (time2 - time1 > 1) {
      if(*(displayGamePtr) == 0 && *(displayEmergencyPtr) == 0 && *(displayTMPtr) == 0 && *(measurementSelectionPtr) == 0 && *(alarmAcknowledgePtr) == 0 && *(displaySelectionPtr) == 0) {
        TCB temp = {&Select, &kD};
        insertLast(&temp);
        run(&temp);
        deleteNode(&temp);
      }
      if(*(measurementSelectionPtr) == 1) {
        TCB temp2 = {&menu, &kD};
        insertLast(&temp2);
        run(&temp2);
        deleteNode(&temp2);
        // menu(&kD);
        
      } else if(*(alarmAcknowledgePtr) == 1) {
            *ModePtrr = 0;
            TCB temp3 = {&anno, &kD};
            insertLast(&temp3);
            run(&temp3);
            deleteNode(&temp3);
            // anno(&kD); 
            if (time2 - time1 > 2000) {
              (*disp.myTask)(disp.taskDataPtr);
              startF = true;
            }
            if (startF == true) {
              flash();
            }
            
   
      } else if (*(displaySelectionPtr) == 1) {
         *ModePtrr = 1;
          Measurement(&kD);
          if (time2 - time1 > 2000) {
              (*disp.myTask)(disp.taskDataPtr);
              startF = true;
          }
          if (startF == true) {
              flash();
          }
      } 

      Serial.println(*(displayTMPtr));
      
      if (*(displayTMPtr) == 1) {
          Serial.print("blank");
          blank();
          //*(displayTMPtr) = 0;
      } else if (*(displayEmergencyPtr) == 1) {
          callDoctor();
      } else if (*(displayGamePtr) == 1) {
          game();
          //*(displayGamePtr) = 0;
      }
      
    }

    if(time2 - time1 > 2000) {
      schedulerTest();
      time1 = time2; 
      
    } else {
   
    }

//    if (n % 2 == 0) {
//      generateEKG();
//      Serial.println("?");
//    }
//    Serial.println(EKGFreqBuf[0]);
}
