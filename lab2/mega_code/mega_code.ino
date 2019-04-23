#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

#define	RED     0xF800
#define	GREEN   0x07E0

int temperatureRaw, systolicPressRaw, diastolicPressRaw, pulseRateRaw, batteryState;

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
    int* tempCorrectedPtr;
    int* sysPressCorrectedPtr;
    int* diasCorrectedPtr;
    int* prCorrectedPtr;
    int* batteryStatePtr;
} DisplayData;

typedef struct
{
    int* temperatureRawPtr;
    int* systolicPressRawPtr;
    int* diastolicPressRawPtr;
    int* pulseRateRawPtr;
    int* batteryStatePtr; 
} WarningAlarmData;

typedef struct
{
    int* batteryStatePtr;
} StatusData;

typedef struct
{

} SchedulerData;


void setup()
{
  // running on the mega - connect to tx1 and rx1 on the mega and to rx and tx on the uno
  // start serial1 port at 9600 bps and wait for serial1 port to open:
  Serial1.begin(9600);

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

}

void loop()
{
  //  send some test characters from the mega to the uno
//   Serial1.println("A");
//   delay(1000);
//   Serial1.println("B");
//   delay(1000);
//   Serial1.println("C");
//   delay(1000);

}


/*
*    @para: void* dataPtr, we assume it's MeasureData pointer; integer isEven, check if it's even;
*    isEven range: 0 or 1;
*    Increase temperatureRaw by 2 even number time, decrease by 1 odd times called before reach 50; 0 is even;
*    Then reverse the process until temperatureRaw falls below 15 
*    systolicPressRaw  - even: Increase by 3; - odd: decrease by 1; Range: no larger than 100
*    diastolicPressRaw -even: decrease by 2; - odd: increase by 1; Range: no less than 40
*    pulseRateRaw - even: decrease by 1; - odd: increase by 3; Range: 15-40
*    April, 22, 2019 by Kaiser
*/
int trIsReverse = 0, prIsReverse = 0;

void Measure(void* dataPtr, int isEven, int trIsReverse, int prIsReverse)
{
    // dereference the data pointer;
    md = *((MeasureData*) dataPtr);  
        // When the function is executed even times;
        // Update the data;
        if(isEven == 1) {
            if(*(md.systolicPressRawPtr) <= 100) {
                *(md.systolicPressRawPtr) += 3;
            }
            if(*(md.diastolicPressRawPtr) >= 40) {
                *(md.diastolicPressRawPtr) -= 2;
            }
            if(trIsReverse == 0) {
                *(md.temperatureRawPtr) += 2;
            } else {
                *(md.temperatureRawPtr) -= 2;
            }
            if(prIsReverse == 0) {
                *(md.pulseRateRawPtr) -= 1;
            } else {
                *(md.pulseRateRawPtr) += 1;
            }
        } else {
            if(*(md.systolicPressRawPtr) <= 100) {
                *(md.systolicPressRawPtr) -= 1;
            }
            if(*(md.diastolicPressRawPtr) >= 40) {
                *(md.diastolicPressRawPtr) += 1;
            }
            if(trIsReverse == 0) {
                *(md.temperatureRawPtr) -= 2;
            } else {
                *(md.temperatureRawPtr) += 2;
            }
            if(prIsReverse == 0) {
                *(md.pulseRateRawPtr) += 1;
            } else {
                *(md.pulseRateRawPtr) -= 1;
            }
        }

        // Check if should reverse the process;
        if(*(md.temperatureRawPtr) > 50) {
            trIsReverse = 1;
        } else if (*(md.temperatureRawPtr) < 15)
        {
            trIsReverse = 1;
        }

        if(*(md.pulseRateRawPtr) > 40) {
            prIsReverse = 1;
        } else if (*(md.pulseRateRawPtr) < 15)
        {
            prIsReverse = 0;
        }

        return;
}

/*
*    @para: generic pointer dataPtr;
*    Assume the data pointer is of type ComputeData
*    Compute the corrected values of data
*    April 23, 2019 by Kaiser Sun
*/
void Compute(void* dataPtr) {
    comd = *((ComputeData*) dataPtr);
    *(comd.tempCorrectedPtr) = (*(comd.temperatureRawPtr)) * 0.75 + 5;
    *(comd.sysPressCorrectedPtr) =(*(comd.systolicPressRawPtr)) * 2 + 9;
    *(comd.diasCorrectedPtr) = (*(comd.diastolicPressRawPtr)) * 1.5 + 6;
    *(comd.prCorrectedPtr) = (*(comd.pulseRateRaw)) * 3 + 8;
    return;
}

/*
*    @para: generic pointer dataPtr;
*    Assume the data pointer is of type DisplayData
*    Display the data on the TFT display
*/
void Display(void* dataPtr) {
    dd = *((DisplayData*) dataPtr);
    
}

/*
*    @param: generic pointer dataPtr;
*    assume the dataPtr is of type dataPtr;
*    if the data are out of range, diplay with red;
*/
void WarningAlarm(void* dataPtr) {
    wad = *((WarningAlarmData*) dataPtr);
    // Change all measurement value?
    tft.print("%s%d%s", "Systolic Pressure: ", *(wad.sysPressCorrectedPtr))
}

/*
*    @param: generic pointer dataPtr;
*    Assume the dataPtr is of type StatusData;
*    BatteryState shall decrease by 1 each it is called;
*/
void Status(void* dataPtr) {
    
}