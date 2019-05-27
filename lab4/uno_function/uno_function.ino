// #include <Elegoo_GFX.h>    // Core graphics library
// #include <Elegoo_TFTLCD.h> // Hardware-specific library
#include "tcb.h"
const byte interruptPin = 2;   // Arbitrary pin. Fix later.
#define delayTimeSec 5
unsigned int pulseCount = 0;
unsigned int pulsePrevious = 0;
// initialize raw values
unsigned int temperatureRaw = 30;
    // there're problem of initial value of temp!
unsigned int systolicPressRaw = 80;
unsigned int diastolicPressRaw = 80;
unsigned int pulseRateRaw = 70;

// // initialize raw value pointers
// unsigned int* temperatureRawPtrr = &temperatureRaw;
// unsigned int* systolicPressRawPtrr = &systolicPressRaw;
// unsigned int* diastolicPressRawPtrr = &diastolicPressRaw;
// unsigned int* pulseRateRawPtrr = &pulseRateRaw;

bool meT = true;
bool meB = true;
bool meR = true;
bool mePR = true;


Bool trIsReverse = FALSE, prIsReverse = FALSE, isEven = TRUE;


//MeasureData meaD;

//set up data struct
// MeasureData meaD = MeasureData{temperatureRawPtrr, systolicPressRawPtrr, diastolicPressRawPtrr, pulseRateRawPtrr};
 

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(4800);
  Serial.begin(4800);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementPulseCount, RISING);
} 

char incoming;

void loop() {
  Measure();
  communications();   
}

void communications() {
    incoming = Serial.read();
    if(incoming == 's') {
      incoming = Serial.read();
      while(incoming != 'e') {
        
        if(incoming = 't') {
          // Serial.write(int(temperatureRaw));
          // Serial.println("temp");
          // Serial.println(temperatureRaw);
          // Serial.println(int(char(temperatureRaw)));
          if(meT) { meT = false; } else { meT = true; }
        }
        if(incoming = 'b') {
          Serial.write(systolicPressRaw);
          Serial.write(diastolicPressRaw);
          if(meB) { meB = false; } else { meB = true; }
        }
        if(incoming = 'p') {
          Serial.write(pulseRateRaw);
          if(mePR) { mePR = false; } else { mePR = true; }
        }
        if(incoming = 'r') {
          //TODO: Add respirrate
          Serial.write(13);
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
    // Serial.println(pulseCount);
    delay(100 * delayTimeSec);
    unsigned int temp2 = pulseCount - pulsePrevious;
    pulsePrevious = pulseCount;
    // Serial.println(temp2);
    if(temp2 != 0){
    rawPulseRate = int(float(temp2 / delayTimeSec) * 60);
    }
    return rawPulseRate;
}
// dereference the data pointer;
void Measure()
{   
    // dereference the data pointer;
    // MeasureData md = *((MeasureData*) dataPtr);  
      // When the function is executed even times;
      // Update the data;
        
      int newPR = getRawPulseRate();
      // *(md.pulseRateRawPtr) = newPR;   
      pulseRateRaw = newPR;
        
      if(isEven) {
          if(systolicPressRaw <= 100) {
              systolicPressRaw += 3;
          }
          if(diastolicPressRaw >= 40) {
              diastolicPressRaw -= 2;
          }
          if(!trIsReverse) {
              temperatureRaw += 2;
          } else {
              temperatureRaw -= 2;
          }
      } else {
          if(systolicPressRaw <= 100) {
              systolicPressRaw -= 1;
          }
          if(diastolicPressRaw >= 40) {
              diastolicPressRaw += 1;
          }
          if(!trIsReverse) {
              temperatureRaw -= 1;
          } else {
              temperatureRaw += 1;
          }
      }
      // Update isReverse;
      if(temperatureRaw > 50) {
          trIsReverse = TRUE;
      } else if (temperatureRaw < 15)
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
