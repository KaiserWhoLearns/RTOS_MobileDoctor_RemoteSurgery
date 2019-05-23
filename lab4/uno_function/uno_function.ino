// #include <Elegoo_GFX.h>    // Core graphics library
// #include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "tcb.h"
const byte interruptPin = 2;   // Arbitrary pin. Fix later.
#define delayTimeSec 2
unsigned int pulseCount = 0;
unsigned int pulsePrevious = 0;
// initialize raw values
unsigned int temperatureRaw = 30;
    // there're problem of initial value of temp!
unsigned int systolicPressRaw = 80;
unsigned int diastolicPressRaw = 80;
unsigned int pulseRateRaw = 70;

// initialize raw value pointers
unsigned int* temperatureRawPtrr = &temperatureRaw;
unsigned int* systolicPressRawPtrr = &systolicPressRaw;
unsigned int* diastolicPressRawPtrr = &diastolicPressRaw;
unsigned int* pulseRateRawPtrr = &pulseRateRaw;

bool meT = true;
bool meB = true;
bool meR = true;
bool mePR = true;


Bool trIsReverse = FALSE, prIsReverse = FALSE, isEven = TRUE;


//MeasureData meaD;

//set up data struct
MeasureData meaD = MeasureData{temperatureRawPtrr, systolicPressRawPtrr, diastolicPressRawPtrr, pulseRateRawPtrr};
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(4800);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementPulseCount, RISING);
} 

char incoming;

void loop() {
  // Serial.print(15);
  
  Measure(&meaD);
  communications();
  // incoming = Serial.read();
  // if (incoming == (char)0x03) {
  //   Serial.write(*pulseRateRawPtrr);
  //   //Serial.println(*pulseRateRawPtrr);
  //   incoming = Serial.read();
  // }
  // if (incoming == (char)0x00) {
  //   Serial.write(*temperatureRawPtrr);
  //   // Serial.println(*pulseRateRawPtrr);
  //   incoming = Serial.read();
  // } 
  // if (incoming == (char)0x01) {
  //   Serial.write(*systolicPressRawPtrr);
  //   incoming = Serial.read();
  // } 
  // if (incoming == (char)0x02) {
  //   Serial.write(*diastolicPressRawPtrr);
  //   incoming = Serial.read();
  // } 
   
}

void communications() {
    incoming = Serial.read();
    if(incoming == 's') {
      incoming = Serial.read();
      while(incoming != 'e') {
        
        if(incoming = 't') {
          Serial.write(*temperatureRawPtrr);
          if(meT) { meT = false; } else { meT = true; }
        }
        if(incoming = 'b') {
          Serial.write(*systolicPressRawPtrr);
          Serial.write(*diastolicPressRawPtrr);
          if(meB) { meB = false; } else { meB = true; }
        }
        if(incoming = 'p') {
          Serial.write(*pulseRateRawPtrr);
          if(mePR) { mePR = false; } else { mePR = true; }
        }
        if(incoming = 'r') {
          //TODO: Add respirrate
          Serial.write(100);
          if(meR) { meR = false; } else { meR = true; }
        }
        incoming = Serial.read();
      }
    }
}


void incrementPulseCount() {
    pulseCount += 1;
}
unsigned int rawPulseRate = 0;
int getRawPulseRate() {
    // pulseCount = 0;
    Serial.println(pulseCount);
    delay(1000 * delayTimeSec);
    unsigned int temp2 = pulseCount - pulsePrevious;
    pulsePrevious = pulseCount;
    // Serial.println(temp2);
    if(temp2 != 0){
    rawPulseRate = int(float(temp2 / delayTimeSec) * 60);
    }
    return rawPulseRate;
}
// dereference the data pointer;
void Measure(void* dataPtr)
{   
    // dereference the data pointer;
    MeasureData md = *((MeasureData*) dataPtr);  
      // When the function is executed even times;
      // Update the data;
        
      int newPR = getRawPulseRate();
      *(md.pulseRateRawPtr) = newPR;   
        
      if(isEven) {
          if(*(md.systolicPressRawPtr) <= 100) {
              *(md.systolicPressRawPtr) += 3;
          }
          if(*(md.diastolicPressRawPtr) >= 40) {
              *(md.diastolicPressRawPtr) -= 2;
          }
          if(!trIsReverse) {
              *(md.temperatureRawPtr) += 2;
          } else {
              *(md.temperatureRawPtr) -= 2;
          }
      } else {
          if(*(md.systolicPressRawPtr) <= 100) {
              *(md.systolicPressRawPtr) -= 1;
          }
          if(*(md.diastolicPressRawPtr) >= 40) {
              *(md.diastolicPressRawPtr) += 1;
          }
          if(!trIsReverse) {
              *(md.temperatureRawPtr) -= 1;
          } else {
              *(md.temperatureRawPtr) += 1;
          }
      }
      // Update isReverse;
      if(*(md.temperatureRawPtr) > 50) {
          trIsReverse = TRUE;
      } else if (*(md.temperatureRawPtr) < 15)
      {
          trIsReverse = FALSE;
      }

      // Update isEven;
      if(isEven) {
          isEven = FALSE;
      } else {
          isEven = TRUE;
      }
      return;
}
